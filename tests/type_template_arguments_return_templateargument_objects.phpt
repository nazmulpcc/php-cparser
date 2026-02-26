--TEST--
CParser Type::getTemplateArguments returns TemplateArgument objects (not Type)
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
var_dump(is_array($args));
var_dump(count($args) === 1);
var_dump($args[0] instanceof CParser\TemplateArgument);

@unlink($file);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)

