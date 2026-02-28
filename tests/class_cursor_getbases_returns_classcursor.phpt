--TEST--
CParser ClassCursor::getBases returns ClassCursor entries
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
struct Base {};
struct Derived : Base {};
CPP;

$file = sys_get_temp_dir() . '/cparser-class-bases-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$derived = null;
foreach ($tu->classes() as $class) {
    if ($class->getSpelling() === 'Derived') {
        $derived = $class;
        break;
    }
}

if (!$derived) {
    echo "failed to find Derived\n";
    exit(1);
}

$bases = iterator_to_array($derived->getBases(), false);
var_dump(count($bases) === 1);
var_dump($bases[0] instanceof CParser\ClassCursor);
var_dump($bases[0]->getSpelling() === 'Base');

@unlink($file);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)

