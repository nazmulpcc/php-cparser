--TEST--
CParser Cursor core methods
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
namespace N {
    class C;
    class C {
    public:
        int f;
        int m(int p);
    };
    using Alias = C;
}
CPP;

$file = sys_get_temp_dir() . '/cparser-cursor-core-' . getmypid() . '.cpp';
file_put_contents($file, $src);
$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);

$iter = $tu->cursors();
$root = $iter->current();
var_dump($root instanceof CParser\Cursor);
var_dump(is_int($root->getKind()));

$classes = [];
foreach ($tu->cursors(CParser\CursorKind::ClassDecl) as $c) {
    $classes[$c->getSpelling()][] = $c;
}
var_dump(isset($classes['C']) && count($classes['C']) >= 2);

$decl = null;
$def = null;
foreach ($classes['C'] as $cand) {
    if ($cand->isDefinition()) {
        $def = $cand;
    } else {
        $decl = $cand;
    }
}

var_dump($def instanceof CParser\ClassCursor);
var_dump($decl instanceof CParser\ClassCursor);

$canon = $def->getCanonical();
var_dump($canon instanceof CParser\ClassCursor);
var_dump($canon->getSpelling() === 'C');

$alias = null;
foreach ($tu->cursors(CParser\CursorKind::TypeAliasDecl) as $c) {
    if ($c->getSpelling() === 'Alias') {
        $alias = $c;
        break;
    }
}
var_dump($alias instanceof CParser\TypeAliasCursor);
var_dump($alias->getDisplayName() !== '');
$loc = $alias->getLocation();
var_dump(is_array($loc) && isset($loc['file'], $loc['line'], $loc['column'], $loc['offset']));
var_dump(str_ends_with((string)$loc['file'], '.cpp'));
var_dump($loc['line'] > 0 && $loc['column'] > 0);

$aliasCanon = $alias->getCanonical();
var_dump($aliasCanon instanceof CParser\Cursor);

$parent = $alias->getParent();
var_dump($parent instanceof CParser\NamespaceCursor);
var_dump($parent->getSpelling() === 'N');

$children = iterator_to_array($parent->getChildren(), false);
var_dump(count($children) >= 3);

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
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
bool(true)
