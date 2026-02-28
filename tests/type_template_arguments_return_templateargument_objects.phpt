--TEST--
CParser Type::getTemplateArguments returns lazy Traversable of TemplateArgument objects
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
template <typename T>
struct Box {};

Box<int> global_box;
CPP;

$file = sys_get_temp_dir() . '/cparser-template-args-' . getmypid() . '.cpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);
$var = null;
foreach ($tu->cursors(CParser\CursorKind::VarDecl) as $c) {
    if ($c->getSpelling() === 'global_box') {
        $var = $c;
        break;
    }
}

if (!$var) {
    echo "failed to find var\n";
    exit(1);
}

$args = $var->getType()->getTemplateArguments();
var_dump($args instanceof Traversable);
var_dump(is_array($args));
$list = iterator_to_array($args, false);
var_dump(count($list) === 1);
var_dump($list[0] instanceof CParser\TemplateArgument);

@unlink($file);
?>
--EXPECT--
bool(true)
bool(false)
bool(true)
bool(true)
