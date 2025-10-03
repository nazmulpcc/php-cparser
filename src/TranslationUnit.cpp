extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
}

#include <clang-c/Index.h>
#include <string>
#include <vector>

// Each TranslationUnit object will wrap a CXTranslationUnit pointer
typedef struct _cparser_translation_unit
{
    CXTranslationUnit tu;
    zend_object std;
} cparser_translation_unit;

static inline cparser_translation_unit *php_cparser_translation_unit_fetch(zend_object *obj)
{
    return (cparser_translation_unit *)((char *)(obj)-XtOffsetOf(cparser_translation_unit, std));
}

// Class entry
extern zend_class_entry *cparser_translationunit_ce;

// Object handler
static zend_object *cparser_translation_unit_create(zend_class_entry *ce)
{
    cparser_translation_unit *intern = (cparser_translation_unit *)ecalloc(1, sizeof(cparser_translation_unit) + zend_object_properties_size(ce));
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    intern->std.handlers = &std_object_handlers;
    intern->tu = nullptr;
    return &intern->std;
}

static void cparser_translation_unit_free(zend_object *obj)
{
    cparser_translation_unit *intern = php_cparser_translation_unit_fetch(obj);
    if (intern->tu)
    {
        clang_disposeTranslationUnit(intern->tu);
        intern->tu = nullptr;
    }
    zend_object_std_dtor(&intern->std);
}

/* {{{ CParser\TranslationUnit::fromFile(string $filename, array $args = []): TranslationUnit */
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
    cparser_translation_unit *intern = php_cparser_translation_unit_fetch(Z_OBJ_P(return_value));
    intern->tu = tu;

    // Note: index freed automatically with TU
    clang_disposeIndex(idx);
}
/* }}} */

/* {{{ CParser\TranslationUnit::classes(): iterable<ClassDecl> */
ZEND_METHOD(CParser_TranslationUnit, classes)
{
    ZEND_PARSE_PARAMETERS_NONE();

    // TODO: yield classes lazily
    // For now, return empty array
    array_init(return_value);
}
/* }}} */

/* {{{ CParser\TranslationUnit::enums(): iterable<EnumDecl> */
ZEND_METHOD(CParser_TranslationUnit, enums)
{
    ZEND_PARSE_PARAMETERS_NONE();

    // TODO: yield enums lazily
    array_init(return_value);
}
/* }}} */

/* {{{ CParser\TranslationUnit::diagnostics(): iterable<Diagnostic> */
ZEND_METHOD(CParser_TranslationUnit, diagnostics)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_translation_unit *intern = php_cparser_translation_unit_fetch(Z_OBJ_P(getThis()));
    if (!intern->tu)
    {
        array_init(return_value);
        return;
    }

    int num = clang_getNumDiagnostics(intern->tu);
    array_init_size(return_value, num);

    for (int i = 0; i < num; i++)
    {
        CXDiagnostic diag = clang_getDiagnostic(intern->tu, i);
        CXString msg = clang_formatDiagnostic(diag, clang_defaultDiagnosticDisplayOptions());
        add_next_index_string(return_value, clang_getCString(msg));
        clang_disposeString(msg);
        clang_disposeDiagnostic(diag);
    }
}
/* }}} */
