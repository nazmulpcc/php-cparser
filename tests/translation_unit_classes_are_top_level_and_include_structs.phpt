--TEST--
CParser TranslationUnit::classes returns top-level classes and structs only
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
struct TopStruct {};

class Outer {
public:
    class InnerClass {};
    struct InnerStruct {};
};

class TopClass {};
CPP;

$file = sys_get_temp_dir() . '/cparser-classes-top-level-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);

$names = [];
foreach ($tu->classes() as $class) {
    $names[] = $class->getSpelling();
}
sort($names);
var_export($names);
echo "\n";

@unlink($file);
?>
--EXPECT--
array (
  0 => 'Outer',
  1 => 'TopClass',
  2 => 'TopStruct',
)

