# php-cparser

`php-cparser` is a native PHP extension for inspecting C and C++ source code with
libclang.

It exposes Clang translation units, cursors, types, diagnostics, include entries,
template arguments, and C++ class/member metadata as PHP objects under the
`CParser` namespace. The extension is useful when you want to build code
generators, API scanners, binding tools, documentation extractors, or static
analysis prototypes in PHP while relying on Clang's parser instead of text-based
parsing.

## Features

- Parse C/C++ files through `CParser\TranslationUnit::fromFile()`.
- Traverse Clang cursors lazily with optional `CParser\CursorKind::*` filtering.
- Inspect classes, structs, methods, fields, constructors, enums, aliases,
  functions, parameters, base classes, and access specifiers.
- Read C++ method traits such as `static`, `const`, `virtual`, `override`,
  `final`, deleted/defaulted constructors, copy/move/default constructors, and
  explicit constructors.
- Inspect `Type` objects, canonical types, pointer/reference/const qualifiers,
  template specializations, and type/non-type template arguments.
- Read parser diagnostics and include graph entries.
- Read direct `clang::annotate` / `__attribute__((annotate()))` annotations.

## Requirements

- PHP development headers and `phpize`.
- A C++ compiler supported by the PHP extension build system.
- libclang headers and libraries.
- `llvm-config`, `llvm-config-19`, `llvm-config-18`, or `pkg-config libclang`
  available on `PATH`.

This repository is currently developed against PHP 8.4 and LLVM/libclang 19.
Other recent PHP/libclang versions may work, but should be verified with the
test suite before publishing support claims.

## Build

From the repository root:

```sh
phpize
./configure --enable-cparser
make -j"$(nproc)"
```

Verify that PHP can load the built extension:

```sh
php -d extension=$PWD/.libs/cparse.so --ri cparser
```

Install it into the active PHP installation:

```sh
sudo make install
```

Then enable it in your PHP configuration, for example:

```ini
extension=cparser.so
```

If `./configure` cannot find libclang, make sure an LLVM config binary is on
`PATH`:

```sh
export PATH="/usr/lib/llvm-19/bin:$PATH"
./configure --enable-cparser
```

The configure script first looks for `llvm-config`, `llvm-config-19`, and
`llvm-config-18`; if none are found it falls back to `pkg-config libclang`.

## Run Tests

The test suite uses PHP's PHPT runner:

```sh
make test TESTS=tests
```

You can also run one file directly:

```sh
php -d extension=$PWD/.libs/cparse.so run-tests.php tests/cursor_core_methods.phpt
```

## Quick Start

```php
<?php

$source = <<<'CPP'
struct Base {
    virtual int id() const = 0;
};

class User final : public Base {
public:
    explicit User(int id);
    int id() const override;

private:
    int score;
};
CPP;

$file = sys_get_temp_dir() . '/demo.cpp';
file_put_contents($file, $source);

$tu = CParser\TranslationUnit::fromFile($file, [
    '-x', 'c++',
    '-std=c++20',
]);

foreach ($tu->diagnostics() as $diagnostic) {
    printf(
        "%d:%d %s\n",
        $diagnostic->getLine(),
        $diagnostic->getColumn(),
        $diagnostic->getMessage(),
    );
}

foreach ($tu->classes() as $class) {
    printf("%s\n", $class->getSpelling());

    foreach ($class->getBaseSpecifiers() as $base) {
        printf("  base: %s\n", $base->getType()?->toString() ?? '<unknown>');
    }

    foreach ($class->getMethods() as $method) {
        printf(
            "  method: %s %s%s\n",
            $method->getReturnType()->toString(),
            $method->getSpelling(),
            $method->isConst() ? ' const' : '',
        );
    }

    foreach ($class->getFields() as $field) {
        printf(
            "  field: %s %s\n",
            $field->getType()?->toString() ?? '<unknown>',
            $field->getSpelling(),
        );
    }
}
```

## API Overview

### Translation Units

```php
$tu = CParser\TranslationUnit::fromFile(
    filename: 'include/example.hpp',
    args: ['-x', 'c++', '-std=c++20', '-Iinclude'],
    flags: CParser\TranslationUnitFlags::None,
);
```

`TranslationUnit` exposes lazy traversables:

- `cursors(int $kind = -1): iterable<Cursor>` recursively walks the translation
  unit in source order.
- `classes(): iterable<ClassCursor>` yields top-level classes and structs.
- `enums(): iterable<EnumCursor>` yields top-level enums.
- `aliases(): iterable<TypeAliasCursor>` yields `using` and `typedef` aliases.
- `diagnostics(): iterable<Diagnostic>` yields parser warnings and errors.
- `includes(): iterable<Inclusion>` yields discovered include entries.

### Cursors

Every AST node is represented by `CParser\Cursor` or one of its subclasses.

Common cursor methods:

- `getKind(): int`
- `getSpelling(): string`
- `getDisplayName(): string`
- `getLocation(): array`
- `getType(): ?Type`
- `isDefinition(): bool`
- `getCanonical(): ?Cursor`
- `getParent(): ?Cursor`
- `getChildren(int $kind = -1): iterable<Cursor>`
- `getAnnotations(): list<string>`

Use `CParser\CursorKind::*` constants to filter traversal:

```php
foreach ($tu->cursors(CParser\CursorKind::FunctionDecl) as $function) {
    echo $function->getSpelling(), PHP_EOL;
}
```

### Classes and Members

`ClassCursor` provides direct-child accessors:

- `getBases(): iterable<ClassCursor>`
- `getBaseSpecifiers(): iterable<BaseSpecifier>`
- `getMethods(): iterable<MethodCursor>`
- `getFields(): iterable<FieldCursor>`
- `getInnerClasses(): iterable<ClassCursor>`
- `getEnums(): iterable<EnumCursor>`
- `isAbstract(): bool`
- `isStruct(): bool`

`MethodCursor` and `FunctionCursor` expose return types and parameters.
`MethodCursor` also exposes C++ member-function traits:

- `isStatic()`, `isConst()`, `isOverride()`, `isVirtual()`, `isPureVirtual()`
- `isDeleted()`, `isDefaulted()`, `isExplicit()`, `isFinal()`
- `isCopyConstructor()`, `isMoveConstructor()`, `isDefaultConstructor()`
- `getBaseMethods(): array<int, MethodCursor>`
- `getAccessSpecifier(): ?int`

Access specifiers use `CParser\Access::Public`,
`CParser\Access::Protected`, and `CParser\Access::Private`.

### Types and Templates

`Type` exposes:

- `toString(): string`
- `isConstQualified(): bool`
- `isPointer(): bool`
- `isReference(): bool`
- `getPointeeType(): ?Type`
- `getCanonicalType(): Type`
- `isTemplateSpecialization(): bool`
- `getTemplateArguments(): iterable<TemplateArgument>`

Template arguments can represent both type and non-type arguments:

```php
foreach ($type->getTemplateArguments() as $argument) {
    if ($argument->getKind() === 'type') {
        echo $argument->getType()?->toString(), PHP_EOL;
    } else {
        var_dump($argument->getValue());
    }
}
```

## Repository Layout

- `cparser.stub.php` defines the PHP API surface and generated arginfo source.
- `cparser_arginfo.h` is generated from the stub and committed for extension
  builds.
- `cparser.cpp` registers the extension, classes, object handlers, and
  `CursorKind` constants.
- `php_cparser.h` contains shared native object declarations and helpers.
- `src/` contains the implementation for translation units, cursors, types,
  diagnostics, template metadata, and iterators.
- `tests/` contains PHPT coverage for parser behavior and public API contracts.
- `config.m4` and `config.w32` contain Unix and Windows extension build
  configuration.

## Development Notes

When changing the public PHP API, update `cparser.stub.php` first and regenerate
`cparser_arginfo.h` with PHP's stub generator for the PHP version you target.
Then rebuild and run the PHPT suite.

Typical local loop:

```sh
phpize
./configure --enable-cparser
make -j"$(nproc)"
make test TESTS=tests
```

Build outputs such as `configure`, `Makefile`, `.libs/`, `modules/`,
`config.h`, and test result files are ignored by `.gitignore` and should not be
committed.

## License

MIT, see [LICENSE](/LICENSE)
