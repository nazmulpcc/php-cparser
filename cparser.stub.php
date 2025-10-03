<?php

/** @generate-class-entries */

namespace CParser;

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