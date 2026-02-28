--TEST--
CParser TranslationUnit::fromFile supports success and failure paths
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$okFile = sys_get_temp_dir() . '/cparser-fromfile-ok-' . getmypid() . '.cpp';
file_put_contents($okFile, "int ok_value = 1;\n");

$tu = CParser\TranslationUnit::fromFile($okFile, ['-std=c++17']);
var_dump($tu instanceof CParser\TranslationUnit);

$missing = $okFile . '.missing';
try {
    CParser\TranslationUnit::fromFile($missing, ['-std=c++17']);
    echo "no-exception\n";
} catch (Exception $e) {
    var_dump(str_contains($e->getMessage(), 'Unable to parse translation unit'));
}

@unlink($okFile);
?>
--EXPECT--
bool(true)
bool(true)

