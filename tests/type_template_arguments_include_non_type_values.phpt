--TEST--
CParser Type::getTemplateArguments includes non-type template arguments with kind/value
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
template <typename T, int N, bool B>
struct Box {};

Box<int, 42, true> global_box;
CPP;

$file = sys_get_temp_dir() . '/cparser-template-args-non-type-' . getmypid() . '.cpp';
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

$args = iterator_to_array($var->getType()->getTemplateArguments(), false);
var_dump(count($args) === 3);

var_dump($args[0]->getKind() === 'type');
var_dump($args[0]->getType() instanceof CParser\Type);
var_dump(str_contains($args[0]->getType()->toString(), 'int'));
var_dump($args[0]->getValue() === null);

var_dump($args[1]->getKind() === 'integral');
var_dump($args[1]->getType() === null);
var_dump($args[1]->getValue() === 42);

var_dump($args[2]->getKind() === 'integral');
var_dump($args[2]->getType() === null);
$boolArgValue = $args[2]->getValue();
var_dump(is_int($boolArgValue));
var_dump($boolArgValue !== 0);

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
