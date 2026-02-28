--TEST--
CParser MethodCursor::getParameters returns direct parameters only
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
struct S {
    void m(int x) {
        auto lam = [](int y) { return y; };
        (void)lam;
    }
};
CPP;

$file = sys_get_temp_dir() . '/cparser-method-params-direct-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$m = null;
foreach ($tu->cursors(CParser\CursorKind::CXXMethod) as $cursor) {
    if ($cursor->getSpelling() === 'm') {
        $m = $cursor;
        break;
    }
}

if (!$m) {
    echo "failed to find method\n";
    exit(1);
}

$names = [];
foreach ($m->getParameters() as $param) {
    $names[] = $param->getSpelling();
}

var_export($names);
echo "\n";

@unlink($file);
?>
--EXPECT--
array (
  0 => 'x',
)

