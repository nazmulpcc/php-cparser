extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"

#ifdef __cplusplus
#include "NativeCXCursorIterator.h"
#include "NativeIterators.h"
#include <clang-c/Index.h>
#include <string>
#include <vector>
#endif

using cparser_tu = cparser_obj<cparser_native_translation_unit>;

template <>
void cparser_object_free<cparser_native_translation_unit>(zend_object *obj)
{
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(obj);
    if (intern->native.tu)
    {
        clang_disposeTranslationUnit(intern->native.tu);
        intern->native.tu = nullptr;
    }
    if (intern->native.index)
    {
        clang_disposeIndex(intern->native.index);
        intern->native.index = nullptr;
    }
    zend_object_std_dtor(obj);
}

#define RETURN_CURSOR_IT_WITH_FILTER(kind)                                                                           \
    do                                                                                                               \
    {                                                                                                                \
        cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));                 \
        object_init_ex(return_value, cparser_cursoriterator_ce);                                                     \
        auto *it_intern = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value));                          \
        it_intern->native.clearOwner();                                                                               \
        it_intern->native = NativeCXCursorIterator(clang_getTranslationUnitCursor(intern->native.tu), (int)kind, true); \
        ZVAL_COPY(&it_intern->native.owner, getThis());                                                               \
        return;                                                                                                      \
    } while (0)

ZEND_METHOD(CParser_TranslationUnit, fromFile)
{
    char *filename;
    size_t filename_len;
    zval *args = NULL;
    zend_long flags = 0;

    ZEND_PARSE_PARAMETERS_START(1, 3)
    Z_PARAM_PATH(filename, filename_len)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(args)
    Z_PARAM_LONG(flags)
    ZEND_PARSE_PARAMETERS_END();

    // Extract args into C array of const char*
    std::vector<const char *> cargs;
    if (args)
    {
        HashTable *arr_hash = Z_ARRVAL_P(args);
        zval *val;
        ZEND_HASH_FOREACH_VAL(arr_hash, val)
        {
            if (Z_TYPE_P(val) == IS_STRING)
            {
                cargs.push_back(Z_STRVAL_P(val));
            }
        }
        ZEND_HASH_FOREACH_END();
    }

    CXIndex idx = clang_createIndex(0, 0);
    CXTranslationUnit tu = clang_parseTranslationUnit(
        idx,
        filename,
        cargs.empty() ? nullptr : cargs.data(),
        (int)cargs.size(),
        nullptr, 0,
        flags);

    if (!tu)
    {
        if (idx)
        {
            clang_disposeIndex(idx);
        }
        zend_throw_exception(zend_ce_exception, "Unable to parse translation unit", 0);
        return;
    }

    // Allocate PHP object
    object_init_ex(return_value, cparser_translationunit_ce);
    auto *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(return_value));
    intern->native.index = idx;
    intern->native.tu = tu;
}

ZEND_METHOD(CParser_TranslationUnit, cursors)
{
    zend_long kind = 0;
    ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(kind)
    ZEND_PARSE_PARAMETERS_END();

    RETURN_CURSOR_IT_WITH_FILTER(kind);
}

ZEND_METHOD(CParser_TranslationUnit, classes)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));
    object_init_ex(return_value, cparser_cursoriterator_ce);
    auto *it_intern = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value));
    it_intern->native.clearOwner();
    it_intern->native = NativeCXCursorIterator(
        clang_getTranslationUnitCursor(intern->native.tu),
        (int)CXCursor_ClassDecl,
        false,
        false,
        (int)CXCursor_StructDecl);
    ZVAL_COPY(&it_intern->native.owner, getThis());
}

ZEND_METHOD(CParser_TranslationUnit, enums)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));
    object_init_ex(return_value, cparser_cursoriterator_ce);
    auto *it_intern = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value));
    it_intern->native.clearOwner();
    it_intern->native = NativeCXCursorIterator(
        clang_getTranslationUnitCursor(intern->native.tu),
        (int)CXCursor_EnumDecl,
        false,
        false);
    ZVAL_COPY(&it_intern->native.owner, getThis());
}

ZEND_METHOD(CParser_TranslationUnit, diagnostics)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));

    object_init_ex(return_value, cparser_diagnosticiterator_ce);
    auto *it = php_cparser_fetch<NativeDiagnosticIterator>(Z_OBJ_P(return_value));
    it->native.tu = intern->native.tu;
    it->native.count = intern->native.tu ? clang_getNumDiagnostics(intern->native.tu) : 0;
    it->native.index = 0;
    ZVAL_COPY(&it->native.owner, getThis());
}
