--TEST--
CParser cursor iterators preserve source order for siblings
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
public:
    int a;
    int b;
    int c;
};
CPP;

$file = sys_get_temp_dir() . '/cparser-order-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$box = null;
foreach ($tu->classes() as $class) {
    if ($class->getSpelling() === 'Box') {
        $box = $class;
        break;
    }
}

if (!$box) {
    echo "failed to find class\n";
    exit(1);
}

$fields = [];
foreach ($box->getFields() as $field) {
    $fields[] = $field->getSpelling();
}

var_export($fields);
echo "\n";

@unlink($file);
?>
--EXPECT--
array (
  0 => 'a',
  1 => 'b',
  2 => 'c',
)

