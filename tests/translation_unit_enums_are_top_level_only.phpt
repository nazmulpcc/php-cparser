--TEST--
CParser TranslationUnit::enums returns top-level enums only
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
enum TopEnum { A };

class Holder {
public:
    enum InnerEnum { B };
};
CPP;

$file = sys_get_temp_dir() . '/cparser-enums-top-level-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);

$names = [];
foreach ($tu->enums() as $enum) {
    $names[] = $enum->getSpelling();
}
sort($names);
var_export($names);
echo "\n";

@unlink($file);
?>
--EXPECT--
array (
  0 => 'TopEnum',
)

