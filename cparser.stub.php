<?php

/** @generate-class-entries */

namespace CParser;

/**
 * @template T of ClassDecl
 * @implements \Iterator<int, T>
 */
final class CursorIterator implements \Iterator
{
    public function __construct(TranslationUnit $tu) {}
    public function current(): Cursor|null {}
    public function key(): int {}
    public function next(): void {}
    public function rewind(): void {}
    public function valid(): bool {}
}

/**
 * Represents a parsed translation unit (a source or header file).
 */
final class TranslationUnit
{
    /**
     * Parse a file and return a TranslationUnit.
     *
     * @param string $filename  Path to the C-family source/header file
     * @param array $args  Compiler flags (e.g. ["-std=c11", "-Iinclude"])
     */
    public static function fromFile(string $filename, array $args = []): TranslationUnit {}

    /**
     * @return iterable<Cursor>
     */
    public function cursors(int $kind = -1): iterable {}

    /**
     * Lazily yield all top-level classes (structs/classes) in the translation unit.
     *
     * @return iterable<ClassDecl>
     */
    public function classes(): iterable {}

    /**
     * Lazily yield all top-level enums in the translation unit.
     *
     * @return iterable<EnumDecl>
     */
    public function enums(): iterable {}

    /**
     * Lazily yield diagnostics (parse warnings/errors).
     *
     * @return iterable<Diagnostic>
     */
    public function diagnostics(): iterable {}
}

final class Cursor {
    /** Kind of cursor (e.g. function, class, field, etc.) */
    public function getKind(): int {}  // CParser\CursorKind::*

    /** The display name or spelling of the cursor */
    public function getSpelling(): string {}

    /** Source location of this cursor */
    public function getLocation(): array {} // ['file' => string|null, 'line' => int, 'column' => int]

    /** The type associated with this cursor, if any */
    public function getType(): ?Type {}

    /** True if this cursor represents a definition, not just a declaration */
    public function isDefinition(): bool {}

    /** The canonical cursor this one refers to (e.g. from typedef or alias) */
    public function getCanonical(): ?Cursor {}

    /** The parent cursor (e.g. the class that contains a method) */
    public function getParent(): ?Cursor {}

    /** Get immediate child cursors (lazy, depth = 1) */
    public function getChildren(int $kind = -1): iterable {}
}

/**
 * Represents a C-family type.
 */
final class Type
{
    public function toString(): string {}

    public function isConstQualified(): bool {}
    public function isPointer(): bool {}
    public function isReference(): bool {}

    public function getPointeeType(): ?Type {}
    public function getCanonicalType(): Type {}

    /** For template specializations: e.g. QVector<int> */
    public function isTemplateSpecialization(): bool {}

    /** @return iterable<TemplateArgument> */
    public function getTemplateArguments(): iterable {}
}

/**
 * A template declaration.
 */
final class TemplateDecl
{
    public function getName(): string {}

    /** @return iterable<TemplateParameter> */
    public function getParameters(): iterable {}
}

final class TemplateParameter
{
    public function getName(): string {}
    public function getKind(): string {}
}

final class TemplateArgument
{
    public function getKind(): string {}
    public function getType(): ?Type {}
    public function getValue(): int|string|float|null {}
}

/**
 * A compiler diagnostic.
 */
final class Diagnostic
{
    public function getMessage(): string {}
    public function getSeverity(): int {}
    public function getLine(): int {}
    public function getColumn(): int {}
}

/**
 * Access specifiers for class members.
 */
final class Access
{
    public const int Public = 1;
    public const int Protected = 2;
    public const int Private = 3;
}

final class CursorKind {
    //
}