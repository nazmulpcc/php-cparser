--TEST--
CParser Cursor::getAnnotations returns direct annotate attributes
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
class [[clang::annotate("class.level")]] Thing {
public:
    [[clang::annotate("method.level")]]
    void alpha();

    [[clang::annotate("slot")]]
    [[clang::annotate("callback")]]
    void beta(int value);
};
CPP;

$file = sys_get_temp_dir() . '/cparser-annotations-' . getmypid() . '.hpp';
file_put_contents($file, $src);

$tu = CParser\TranslationUnit::fromFile(
    $file,
    ['-x', 'c++', '-std=c++20'],
    CParser\TranslationUnitFlags::KeepGoing | CParser\TranslationUnitFlags::VisitImplicitAttributes
);

$class = null;
foreach ($tu->classes() as $cursor) {
    if ($cursor->getSpelling() === 'Thing') {
        $class = $cursor;
        break;
    }
}

$methods = [];
foreach ($class->getMethods() as $method) {
    $methods[$method->getSpelling()] = $method;
}

var_dump($class instanceof CParser\ClassCursor);
var_dump($methods['alpha'] instanceof CParser\MethodCursor);
var_dump($methods['beta'] instanceof CParser\MethodCursor);
var_dump($class->getAnnotations());
var_dump($methods['alpha']->getAnnotations());
var_dump($methods['beta']->getAnnotations());

@unlink($file);
?>
--EXPECT--
bool(true)
bool(true)
bool(true)
array(1) {
  [0]=>
  string(11) "class.level"
}
array(1) {
  [0]=>
  string(12) "method.level"
}
array(2) {
  [0]=>
  string(4) "slot"
  [1]=>
  string(8) "callback"
}
