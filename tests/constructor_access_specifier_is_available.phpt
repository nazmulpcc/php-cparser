--TEST--
CParser constructor cursors expose MethodCursor access specifier API
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
class CtorAccess {
public:
    CtorAccess();
    CtorAccess(int v);
protected:
    CtorAccess(double v);
private:
    CtorAccess(const char *name);
};
CPP;

$file = sys_get_temp_dir() . '/cparser-ctor-access-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$class = null;
foreach ($tu->cursors(CParser\CursorKind::ClassDecl) as $cursor) {
    if ($cursor instanceof CParser\ClassCursor && $cursor->getSpelling() === 'CtorAccess') {
        $class = $cursor;
        break;
    }
}

var_dump($class instanceof CParser\ClassCursor);

$allMethodCursors = true;
$allHaveAccessMethod = true;
$accesses = [];
$count = 0;
foreach ($class->getChildren(CParser\CursorKind::CXXConstructor) as $ctor) {
    $count++;
    $allMethodCursors = $allMethodCursors && ($ctor instanceof CParser\MethodCursor);
    $allHaveAccessMethod = $allHaveAccessMethod && method_exists($ctor, 'getAccessSpecifier');
    $accesses[] = $ctor->getAccessSpecifier();
}

sort($accesses);
$expected = [CParser\Access::Public, CParser\Access::Public, CParser\Access::Protected, CParser\Access::Private];
sort($expected);

var_dump($count === 4);
var_dump($allMethodCursors);
var_dump($allHaveAccessMethod);
var_dump($accesses === $expected);

@unlink($file);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
