--TEST--
CParser Field/Enum/Parameter/TypeAlias methods
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
typedef unsigned long ULong;

enum E : unsigned int {
    One = 1,
    Two = 2
};

struct S {
    static int sf;
    const int cf = 3;
    int m(const int p, int q = 42) { return p + q; }
};
CPP;

$file = sys_get_temp_dir() . '/cparser-remain-' . getmypid() . '.cpp';
file_put_contents($file, $src);
$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);

$alias = null;
$enum = null;
$struct = null;
foreach ($tu->cursors() as $c) {
    if ($c instanceof CParser\TypeAliasCursor && $c->getSpelling() === 'ULong') {
        $alias = $c;
    }
    if ($c instanceof CParser\EnumCursor && $c->getSpelling() === 'E') {
        $enum = $c;
    }
    if ($c instanceof CParser\ClassCursor && $c->getSpelling() === 'S') {
        $struct = $c;
    }
}

var_dump($alias instanceof CParser\TypeAliasCursor);
$under = $alias->getUnderlyingType();
var_dump($under instanceof CParser\Type);
var_dump(str_contains($under->toString(), 'unsigned'));

var_dump($enum instanceof CParser\EnumCursor);
$enumInt = $enum->getIntegerType();
var_dump($enumInt instanceof CParser\Type);
var_dump(str_contains($enumInt->toString(), 'unsigned'));

$consts = iterator_to_array($enum->getConstants(), false);
var_dump(count($consts) >= 2);
var_dump($consts[0] instanceof CParser\EnumConstantCursor);
$vals = [];
foreach ($consts as $ec) {
    $vals[$ec->getSpelling()] = $ec->getValue();
}
var_dump($vals['One'] === 1);
var_dump($vals['Two'] === 2);

$fields = [];
foreach ($struct->getFields() as $f) {
    $name = $f->getSpelling();
    if ($name !== '') {
        $fields[$name] = $f;
    }
}
var_dump(isset($fields['cf']));
var_dump(!$fields['cf']->isStatic());
var_dump($fields['cf']->getAccessSpecifier() === CParser\Access::Public);
$ft = $fields['cf']->getType();
var_dump($ft instanceof CParser\Type);
var_dump($ft->toString() !== '');

$m = null;
foreach ($struct->getMethods() as $method) {
    if ($method->getSpelling() === 'm') {
        $m = $method;
        break;
    }
}
var_dump($m instanceof CParser\MethodCursor);
$params = iterator_to_array($m->getParameters(), false);
var_dump(count($params) === 2);
var_dump($params[0] instanceof CParser\ParameterCursor);
var_dump($params[1] instanceof CParser\ParameterCursor);
$pt = $params[0]->getType();
var_dump($pt instanceof CParser\Type);
var_dump(is_bool($params[0]->isConstQualified()));
var_dump(is_bool($params[0]->hasDefaultValue()));
var_dump(is_bool($params[1]->hasDefaultValue()));

@unlink($file);
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
