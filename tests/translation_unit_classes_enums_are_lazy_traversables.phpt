--TEST--
CParser TranslationUnit classes() and enums() return lazy Traversable iterators
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
struct S {};
class C {};
enum E { A };
CPP;

$file = sys_get_temp_dir() . '/cparser-tu-lazy-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);

$classes = $tu->classes();
$enums = $tu->enums();

var_dump($classes instanceof Traversable);
var_dump(is_array($classes));
var_dump($enums instanceof Traversable);
var_dump(is_array($enums));

@unlink($file);
?>
--EXPECT--
bool(true)
bool(false)
bool(true)
bool(false)

