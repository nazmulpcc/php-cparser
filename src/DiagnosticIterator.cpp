extern "C"
{
#include "php.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"
#include "NativeIterators.h"

using cparser_tu = cparser_obj<cparser_native_translation_unit>;
using cparser_diag_it = cparser_obj<NativeDiagnosticIterator>;

template <>
void cparser_object_free<NativeDiagnosticIterator>(zend_object *obj)
{
    cparser_diag_it *intern = php_cparser_fetch<NativeDiagnosticIterator>(obj);
    if (!Z_ISUNDEF(intern->native.owner))
    {
        zval_ptr_dtor(&intern->native.owner);
        ZVAL_UNDEF(&intern->native.owner);
    }
    zend_object_std_dtor(obj);
}

ZEND_METHOD(CParser_DiagnosticIterator, __construct)
{
    zval *tu_zv;
    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_OBJECT_OF_CLASS(tu_zv, cparser_translationunit_ce)
    ZEND_PARSE_PARAMETERS_END();

    cparser_tu *tu = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(tu_zv));
    cparser_diag_it *it = php_cparser_fetch<NativeDiagnosticIterator>(Z_OBJ_P(getThis()));

    it->native.tu = tu->native.tu;
    it->native.count = tu->native.tu ? clang_getNumDiagnostics(tu->native.tu) : 0;
    it->native.index = 0;
    ZVAL_COPY(&it->native.owner, tu_zv);
}

ZEND_METHOD(CParser_DiagnosticIterator, current)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_diag_it *it = php_cparser_fetch<NativeDiagnosticIterator>(Z_OBJ_P(getThis()));
    if (!it->native.tu || it->native.index >= it->native.count)
    {
        RETURN_NULL();
    }

    CXDiagnostic diag = clang_getDiagnostic(it->native.tu, it->native.index);
    if (!diag)
    {
        RETURN_NULL();
    }

    object_init_ex(return_value, cparser_diagnostic_ce);
    php_cparser_fetch<CXDiagnostic>(Z_OBJ_P(return_value))->native = diag;
}

ZEND_METHOD(CParser_DiagnosticIterator, key)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_diag_it *it = php_cparser_fetch<NativeDiagnosticIterator>(Z_OBJ_P(getThis()));
    RETURN_LONG((zend_long)it->native.index);
}

ZEND_METHOD(CParser_DiagnosticIterator, next)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_diag_it *it = php_cparser_fetch<NativeDiagnosticIterator>(Z_OBJ_P(getThis()));
    if (it->native.index < it->native.count)
    {
        ++it->native.index;
    }
    RETURN_NULL();
}

ZEND_METHOD(CParser_DiagnosticIterator, rewind)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_diag_it *it = php_cparser_fetch<NativeDiagnosticIterator>(Z_OBJ_P(getThis()));
    it->native.index = 0;
    RETURN_NULL();
}

ZEND_METHOD(CParser_DiagnosticIterator, valid)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_diag_it *it = php_cparser_fetch<NativeDiagnosticIterator>(Z_OBJ_P(getThis()));
    RETURN_BOOL(it->native.tu && it->native.index < it->native.count);
}
