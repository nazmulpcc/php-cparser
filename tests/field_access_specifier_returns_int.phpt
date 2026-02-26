--TEST--
CParser FieldCursor getAccessSpecifier returns int compatible with CParser Access constants
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
class Demo {
    int hidden;
public:
    int shown;
};
CPP;

$file = sys_get_temp_dir() . '/cparser-field-access-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$class = null;
foreach ($tu->cursors(CParser\CursorKind::ClassDecl) as $cursor) {
    if ($cursor instanceof CParser\ClassCursor && $cursor->getSpelling() === 'Demo') {
        $class = $cursor;
        break;
    }
}

$vals = [];
foreach ($class->getFields() as $field) {
    $vals[$field->getSpelling()] = $field->getAccessSpecifier();
}

ksort($vals);

var_dump(is_int($vals['hidden']));
var_dump(is_int($vals['shown']));
var_dump($vals['hidden'] === CParser\Access::Private);
var_dump($vals['shown'] === CParser\Access::Public);

@unlink($file);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)

