--TEST--
CParser reflective extraction of field/parameter types requires subclass getType registration
--SKIPIF--
<?php
if (!extension_loaded('cparser')) {
    die('skip cparser extension not loaded');
}
?>
--FILE--
<?php
function extract_type_with_declared_api(object $cursor): ?string {
    $rc = new ReflectionClass($cursor);
    $declaresGetType = false;
    foreach ($rc->getMethods(ReflectionMethod::IS_PUBLIC) as $method) {
        if (
            $method->getName() === 'getType'
            && $method->getDeclaringClass()->getName() === $rc->getName()
        ) {
            $declaresGetType = true;
            break;
        }
    }

    // Representative reflection-driven extractor behavior:
    // only use class-declared API for class-specific features.
    if (!$declaresGetType) {
        return null;
    }

    $type = $cursor->getType();
    return $type ? $type->toString() : null;
}

$src = <<<'CPP'
struct S { int count; };
void f(double value) {}
CPP;

$file = sys_get_temp_dir() . '/cparser-field-param-extract-' . getmypid() . '.cpp';
file_put_contents($file, $src);
$tu = CParser\TranslationUnit::fromFile($file, ['-std=c++17']);

$field = null;
$param = null;
foreach ($tu->cursors() as $cursor) {
    if ($cursor instanceof CParser\FieldCursor && $cursor->getSpelling() === 'count') {
        $field = $cursor;
    }
    if ($cursor instanceof CParser\ParameterCursor && $cursor->getSpelling() === 'value') {
        $param = $cursor;
    }
}

var_dump($field instanceof CParser\FieldCursor);
var_dump($param instanceof CParser\ParameterCursor);
var_dump(extract_type_with_declared_api($field));
var_dump(extract_type_with_declared_api($param));

@unlink($file);
?>
--EXPECT--
bool(true)
bool(true)
string(3) "int"
string(6) "double"
