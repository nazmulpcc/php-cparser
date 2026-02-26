--TEST--
CParser root cursor nullability for getParent() and getType()
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
int answer();
CPP;

$file = sys_get_temp_dir() . '/cparser-null-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$it = $tu->cursors();
$root = $it->current();

var_dump($root instanceof CParser\Cursor);
var_dump($root->getParent());
var_dump($root->getType());

@unlink($file);
?>
--EXPECT--
bool(true)
NULL
NULL

