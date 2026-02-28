--TEST--
CParser ClassCursor member accessors return direct children only
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
class Outer {
public:
    int f1;
    void m1();
    enum E1 { A };
    class Inner {
    public:
        int f2;
        void m2();
        enum E2 { B };
        class Nested {};
    };
};
CPP;

$file = sys_get_temp_dir() . '/cparser-class-direct-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$outer = null;

foreach ($tu->classes() as $class) {
    if ($class instanceof CParser\ClassCursor && $class->getSpelling() === 'Outer') {
        $outer = $class;
        break;
    }
}

if (!$outer) {
    echo "failed to find Outer\n";
    exit(1);
}

$methods = [];
foreach ($outer->getMethods() as $m) {
    $methods[] = $m->getSpelling();
}
sort($methods);

$fields = [];
foreach ($outer->getFields() as $f) {
    $fields[] = $f->getSpelling();
}
sort($fields);

$enums = [];
foreach ($outer->getEnums() as $e) {
    $enums[] = $e->getSpelling();
}
sort($enums);

$inners = [];
foreach ($outer->getInnerClasses() as $c) {
    $inners[] = $c->getSpelling();
}
sort($inners);

var_export($methods);
echo "\n";
var_export($fields);
echo "\n";
var_export($enums);
echo "\n";
var_export($inners);
echo "\n";

@unlink($file);
?>
--EXPECT--
array (
  0 => 'm1',
)
array (
  0 => 'f1',
)
array (
  0 => 'E1',
)
array (
  0 => 'Inner',
)

