--TEST--
CParser CursorIterator accepts Cursor source and yields incrementing keys
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
class Box {
    int a;
    int b;
};
CPP;

$file = sys_get_temp_dir() . '/cparser-iter-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$class = null;
foreach ($tu->cursors(CParser\CursorKind::ClassDecl) as $cursor) {
    if ($cursor instanceof CParser\ClassCursor && $cursor->getSpelling() === 'Box') {
        $class = $cursor;
        break;
    }
}

if (!$class) {
    echo "failed to find class\n";
    exit(1);
}

$iter = new CParser\CursorIterator($class, CParser\CursorKind::FieldDecl);
$keys = [];
$names = [];
foreach ($iter as $k => $field) {
    $keys[] = $k;
    $names[] = $field->getSpelling();
}

var_export($keys);
echo "\n";
var_export(count($keys) === 2);
echo "\n";
var_export($iter instanceof Iterator);
echo "\n";

@unlink($file);
?>
--EXPECT--
array (
  0 => 0,
  1 => 1,
)
true
true

