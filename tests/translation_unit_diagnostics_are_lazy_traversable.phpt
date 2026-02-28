--TEST--
CParser TranslationUnit::diagnostics returns lazy Traversable of Diagnostic objects
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
int bad = "x";
CPP;

$file = sys_get_temp_dir() . '/cparser-diags-lazy-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$diags = $tu->diagnostics();

var_dump($diags instanceof Traversable);
var_dump(is_array($diags));

$list = iterator_to_array($diags, false);
var_dump(count($list) >= 1);
var_dump($list[0] instanceof CParser\Diagnostic);

@unlink($file);
?>
--EXPECT--
bool(true)
bool(false)
bool(true)
bool(true)

