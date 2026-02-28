--TEST--
CParser CursorKind constants cover stmt, preprocessing, extra decl and translation unit kinds
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$constants = (new ReflectionClass(CParser\CursorKind::class))->getConstants();
var_dump(isset($constants['ClassDecl']));
var_dump(isset($constants['CompoundStmt']));
var_dump(isset($constants['TranslationUnit']));
var_dump(isset($constants['macro definition']));
var_dump(isset($constants['ModuleImport']));
var_dump(isset($constants['OverloadCandidate']));
var_dump(count($constants) > 150);

$src = <<<'CPP'
int f() { return 42; }
CPP;

$file = sys_get_temp_dir() . '/cparser-cursorkind-constants-' . getmypid() . '.cpp';
file_put_contents($file, $src);
$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$root = $tu->cursors()->current();
var_dump($root->getKind() === CParser\CursorKind::TranslationUnit);

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
