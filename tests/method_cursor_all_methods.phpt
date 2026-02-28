--TEST--
CParser MethodCursor and FunctionCursor methods
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
$src = <<<'CPP'
struct Base {
    virtual int ov(int x) { return x; }
    virtual int pure(int y) = 0;
};

struct Derived : Base {
    int field = 0;
    static int st(int z) { return z; }
    int ov(int x) override { return x + 1; }
    int cc(int k) const { return k; }
    int pure(int y) override { return y; }
};

int free_fn(int a, const int b = 2) { return a + b; }
CPP;

$file = sys_get_temp_dir() . '/cparser-methods-' . getmypid() . '.cpp';
file_put_contents($file, $src);
$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);

$derived = null;
$base = null;
$free = null;
foreach ($tu->cursors() as $c) {
    if ($c instanceof CParser\ClassCursor && $c->getSpelling() === 'Derived') {
        $derived = $c;
    }
    if ($c instanceof CParser\ClassCursor && $c->getSpelling() === 'Base') {
        $base = $c;
    }
    if ($c instanceof CParser\FunctionCursor && $c->getSpelling() === 'free_fn') {
        $free = $c;
    }
}

var_dump($derived instanceof CParser\ClassCursor);
var_dump($base instanceof CParser\ClassCursor);
var_dump($free instanceof CParser\FunctionCursor);

$m = [];
foreach ($derived->getMethods() as $method) {
    $m[$method->getSpelling()] = $method;
}
var_dump(isset($m['st'], $m['ov'], $m['cc'], $m['pure']));

var_dump($m['st']->isStatic());
var_dump(!$m['ov']->isStatic());
var_dump($m['cc']->isConst());
var_dump(!$m['ov']->isConst());
var_dump($m['ov']->isOverride());
var_dump($m['ov']->isVirtual());
var_dump(!$m['ov']->isPureVirtual());
var_dump($m['pure']->isOverride());

$baseMethods = $m['ov']->getBaseMethods();
var_dump(is_array($baseMethods));
var_dump(count($baseMethods) === 1);
var_dump($baseMethods[0] instanceof CParser\MethodCursor);
var_dump($baseMethods[0]->getSpelling() === 'ov');

$ret = $m['ov']->getReturnType();
var_dump($ret instanceof CParser\Type);
var_dump(str_contains($ret->toString(), 'int'));
var_dump($m['ov']->getAccessSpecifier() === CParser\Access::Public);

$params = iterator_to_array($m['ov']->getParameters(), false);
var_dump(count($params) === 1);
var_dump($params[0] instanceof CParser\ParameterCursor);

$fret = $free->getReturnType();
var_dump($fret instanceof CParser\Type);
var_dump(str_contains($fret->toString(), 'int'));
var_dump($free->getNumArguments() === 2);
$fparams = iterator_to_array($free->getParameters(), false);
var_dump(count($fparams) === 2);
var_dump($fparams[0] instanceof CParser\ParameterCursor);

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
