--TEST--
CParser Cursor::getChildren should return direct children, not recursive descendants
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
class Widget {
public:
    void paint(int color, int alpha) {}
};
CPP;

$file = sys_get_temp_dir() . '/cparser-children-direct-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$widget = null;
foreach ($tu->cursors(CParser\CursorKind::ClassDecl) as $c) {
    if ($c->getSpelling() === 'Widget') {
        $widget = $c;
        break;
    }
}

if (!$widget) {
    echo "failed to find class\n";
    exit(1);
}

$paramNames = [];
foreach ($widget->getChildren(CParser\CursorKind::ParmDecl) as $param) {
    $paramNames[] = $param->getSpelling();
}

var_export($paramNames);
echo "\n";

@unlink($file);
?>
--EXPECT--
array (
)

