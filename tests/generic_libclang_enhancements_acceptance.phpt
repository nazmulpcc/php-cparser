--TEST--
CParser generic libclang enhancements acceptance
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$dir = sys_get_temp_dir() . '/cparser-enh-' . getmypid();
@mkdir($dir);
$dep = $dir . '/dep.h';
$cpp = $dir . '/main.cpp';

file_put_contents($dep, "struct DepType {};\n");

$src = <<<'CPP'
#include <vector>
#include "dep.h"

namespace N {
template <class T> class Vec {};
class B {};
class A : public virtual Vec<int>, private B {
public:
    A() = default;
    A(const A&) = delete;
    A(A&&) = default;
    explicit A(int);
    virtual void f() final;
};
using IntVec = Vec<int>;
typedef Vec<double> DoubleVec;
}
CPP;

file_put_contents($cpp, $src);

$tu = CParser\TranslationUnit::fromFile($cpp, ['-x', 'c++', '-std=c++17', '-I' . $dir]);
$classA = null;
foreach ($tu->cursors() as $cursor) {
    if ($cursor instanceof CParser\ClassCursor && $cursor->getSpelling() === 'A') {
        $classA = $cursor;
        break;
    }
}
var_dump($classA instanceof CParser\ClassCursor);

$ctors = iterator_to_array($classA->getChildren(CParser\CursorKind::CXXConstructor), false);
var_dump(count($ctors) === 4);

$allMethod = true;
$deleted = 0;
$defaulted = 0;
$explicit = 0;
$copy = 0;
$move = 0;
$defaultCtor = 0;
foreach ($ctors as $ctor) {
    $allMethod = $allMethod && ($ctor instanceof CParser\MethodCursor);
    $deleted += $ctor->isDeleted() ? 1 : 0;
    $defaulted += $ctor->isDefaulted() ? 1 : 0;
    $explicit += $ctor->isExplicit() ? 1 : 0;
    $copy += $ctor->isCopyConstructor() ? 1 : 0;
    $move += $ctor->isMoveConstructor() ? 1 : 0;
    $defaultCtor += $ctor->isDefaultConstructor() ? 1 : 0;
}

var_dump($allMethod);
var_dump($deleted === 1);
var_dump($defaulted >= 2);
var_dump($explicit === 1);
var_dump($copy === 1);
var_dump($move === 1);
var_dump($defaultCtor === 1);

$methodF = null;
foreach ($classA->getMethods() as $method) {
    if ($method->getSpelling() === 'f') {
        $methodF = $method;
        break;
    }
}
var_dump($methodF instanceof CParser\MethodCursor);
var_dump($methodF->isFinal());
var_dump($methodF->isCopyConstructor() === false);

$baseSpecs = iterator_to_array($classA->getBaseSpecifiers(), false);
var_dump(count($baseSpecs) === 2);

$vecOk = false;
$baseBOk = false;
$baseBRefOk = false;
foreach ($baseSpecs as $base) {
    if (!$base instanceof CParser\BaseSpecifier) {
        continue;
    }
    $type = $base->getType();
    $typeText = $type instanceof CParser\Type ? $type->toString() : '';
    $ref = $base->getReferenced();

    if (str_contains($typeText, 'Vec')) {
        $vecOk = ($base->getAccessSpecifier() === CParser\Access::Public) && $base->isVirtual();
    }
    if (preg_match('/(^|[^A-Za-z0-9_])B($|[^A-Za-z0-9_])/', $typeText) === 1) {
        $baseBOk = ($base->getAccessSpecifier() === CParser\Access::Private) && !$base->isVirtual();
        $baseBRefOk = $ref instanceof CParser\Cursor;
    }
}
var_dump($vecOk);
var_dump($baseBOk);
var_dump($baseBRefOk);

$includes = iterator_to_array($tu->includes(), false);
$hasVector = false;
$hasDep = false;
$includeMetaOk = true;
foreach ($includes as $inc) {
    if (!$inc instanceof CParser\Inclusion) {
        continue;
    }

    $includeMetaOk = $includeMetaOk
        && is_int($inc->getLine())
        && is_int($inc->getColumn())
        && $inc->getLine() >= 0
        && $inc->getColumn() >= 0;

    if ($inc->getSpelling() === '<vector>') {
        $hasVector = $inc->isAngled() && str_ends_with((string)$inc->getSourceFile(), 'main.cpp');
    }
    if ($inc->getSpelling() === '"dep.h"') {
        $hasDep = !$inc->isAngled() && str_ends_with((string)$inc->getSourceFile(), 'main.cpp');
    }
}
var_dump($hasVector);
var_dump($hasDep);
var_dump($includeMetaOk);

$includesAgain = iterator_to_array($tu->includes(), false);
var_dump(count($includesAgain) === count($includes));

$aliases = iterator_to_array($tu->aliases(), false);
$aliasMap = [];
foreach ($aliases as $alias) {
    if (!$alias instanceof CParser\TypeAliasCursor) {
        continue;
    }
    $ut = $alias->getUnderlyingType();
    $aliasMap[$alias->getSpelling()] = $ut instanceof CParser\Type ? $ut->toString() : '';
}

var_dump(isset($aliasMap['IntVec']));
var_dump(isset($aliasMap['DoubleVec']));
var_dump(str_contains($aliasMap['IntVec'], 'Vec'));
var_dump(str_contains($aliasMap['DoubleVec'], 'Vec'));

$kindValues = array_values((new ReflectionClass(CParser\CursorKind::class))->getConstants());
var_dump(in_array($ctors[0]->getKind(), $kindValues, true));
var_dump(in_array($baseSpecs[0]->getKind(), $kindValues, true));

@unlink($cpp);
@unlink($dep);
@rmdir($dir);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
