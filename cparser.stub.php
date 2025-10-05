<?php

/** @generate-class-entries */

namespace CParser;

/**
 * @template T of ClassDecl
 * @implements \Iterator<int, T>
 */
final class ClassIterator implements \Iterator
{
    public function __construct(TranslationUnit $tu) {}
    public function current(): ClassDecl|EnumDecl|null {}
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

/**
 * A class/struct declaration.
 */
final class ClassDecl
{
    public function getName(): string {}

    /** @return iterable<string> Fully qualified base class names */
    public function getBases(): iterable {}

    /** @return iterable<MethodDecl> */
    public function methods(): iterable {}

    /** @return iterable<ClassDecl> Inner classes */
    public function innerClasses(): iterable {}

    /** @return iterable<EnumDecl> Enums declared inside the class */
    public function enums(): iterable {}

    /** @return iterable<TemplateDecl> Templates declared inside the class */
    public function templates(): iterable {}

    public function isAbstract(): bool {}
}

/**
 * A function or method declaration.
 */
final class MethodDecl
{
    public function getName(): string {}
    public function getAccess(): int {}   // CParser\Access::Public, etc.
    public function getReturnType(): Type {}

    /** @return iterable<ParamDecl> */
    public function getParameters(): iterable {}

    public function isStatic(): bool {}
    public function isConst(): bool {}
    public function isVirtual(): bool {}
    public function isPureVirtual(): bool {}
    public function isConstructor(): bool {}
    public function isDestructor(): bool {}

    /** Fully formatted signature string */
    public function getSignature(): string {}
}

/**
 * A function or method parameter.
 */
final class ParamDecl
{
    public function getName(): string {}
    public function getType(): Type {}
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
 * An enum declaration.
 */
final class EnumDecl
{
    public function getName(): string {}

    /** @return iterable<EnumConstant> */
    public function getConstants(): iterable {}

    public function getIntegerType(): Type {}
}

final class EnumConstant
{
    public function getName(): string {}
    public function getValue(): int {}
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