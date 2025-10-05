extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"

#ifdef __cplusplus
#include <clang-c/Index.h>
#include <string>
#include <vector>
#endif

using cparser_tu = cparser_obj<CXTranslationUnit>;

ZEND_METHOD(CParser_TranslationUnit, fromFile)
{
    char *filename;
    size_t filename_len;
    zval *args = NULL;

    ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_PATH(filename, filename_len)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(args)
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
        CXTranslationUnit_None);

    if (!tu)
    {
        zend_throw_exception(zend_ce_exception, "Unable to parse translation unit", 0);
        return;
    }

    // Allocate PHP object
    object_init_ex(return_value, cparser_translationunit_ce);
    cparser_tu *intern = php_cparser_fetch<CXTranslationUnit>(Z_OBJ_P(return_value));
    intern->native = tu;

    clang_disposeIndex(idx);
}

ZEND_METHOD(CParser_TranslationUnit, cursors)
{
    zend_long kind = -1;
    ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(kind)
    ZEND_PARSE_PARAMETERS_END();

    // Fetch TU intern
    cparser_tu *intern = php_cparser_fetch<CXTranslationUnit>(Z_OBJ_P(getThis()));
    if (!intern || !intern->native)
    {
        RETURN_FALSE;
    }

    // Create iterator object
    zval it = ast_create_iterator_from_tu(getThis(), kind);

    ast_cursor_iterator *ait = Z_AST_IT_P(Z_OBJ(it));
    if (!ait)
    {
        RETURN_FALSE;
    }

    Z_TRY_ADDREF_P(getThis());
    ZVAL_OBJ_COPY(&ait->source_obj, Z_OBJ_P(getThis()));

    if (kind >= 0)
    {
        ait->filter_kind = (CXCursorKind)kind;
    }

    RETURN_ZVAL(&it, 0, 1);
}

ZEND_METHOD(CParser_TranslationUnit, classes)
{
    //
}

ZEND_METHOD(CParser_TranslationUnit, enums)
{
    ZEND_PARSE_PARAMETERS_NONE();
    zval it = ast_create_iterator_from_tu(getThis(), (int)CXCursor_EnumDecl);
    RETURN_ZVAL(&it, 0, 1);
}

ZEND_METHOD(CParser_TranslationUnit, diagnostics)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<CXTranslationUnit>(Z_OBJ_P(getThis()));

    if (!intern->native)
    {
        array_init(return_value);
        return;
    }

    unsigned count = clang_getNumDiagnostics(intern->native);
    array_init_size(return_value, count);

    for (unsigned i = 0; i < count; i++)
    {
        CXDiagnostic diag = clang_getDiagnostic(intern->native, i);

        if (!diag)
        {
            add_next_index_null(return_value);
            continue;
        }

        zval zv;
        object_init_ex(&zv, cparser_diagnostic_ce);

        auto *diag_intern = php_cparser_fetch<CXDiagnostic>(Z_OBJ(zv));
        diag_intern->native = diag;

        add_next_index_zval(return_value, &zv);
    }
}