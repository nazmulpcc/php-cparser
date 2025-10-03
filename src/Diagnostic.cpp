extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}

#include "php_cparser.h"

using diagnostic_obj = cparser_obj<CXDiagnostic>;

template <>
void cparser_object_free<CXDiagnostic>(zend_object *obj)
{
    diagnostic_obj *intern = php_cparser_fetch<CXDiagnostic>(obj);
    if (intern->native)
    {
        clang_disposeDiagnostic(intern->native);
        intern->native = nullptr;
    }
    zend_object_std_dtor(obj);
}

ZEND_METHOD(CParser_Diagnostic, getMessage)
{
    ZEND_PARSE_PARAMETERS_NONE();
    diagnostic_obj *intern = php_cparser_fetch<CXDiagnostic>(Z_OBJ_P(getThis()));

    CXString msg = clang_formatDiagnostic(intern->native, clang_defaultDiagnosticDisplayOptions());
    RETVAL_STRING(clang_getCString(msg));
    clang_disposeString(msg);
}

ZEND_METHOD(CParser_Diagnostic, getSeverity)
{
    ZEND_PARSE_PARAMETERS_NONE();
    diagnostic_obj *intern = php_cparser_fetch<CXDiagnostic>(Z_OBJ_P(getThis()));

    unsigned sev = clang_getDiagnosticSeverity(intern->native);
    RETVAL_LONG(sev);
}

ZEND_METHOD(CParser_Diagnostic, getLine)
{
    ZEND_PARSE_PARAMETERS_NONE();
    diagnostic_obj *intern = php_cparser_fetch<CXDiagnostic>(Z_OBJ_P(getThis()));

    CXSourceLocation loc = clang_getDiagnosticLocation(intern->native);
    CXFile file;
    unsigned line, col, offset;
    clang_getExpansionLocation(loc, &file, &line, &col, &offset);

    RETVAL_LONG(line);
}

ZEND_METHOD(CParser_Diagnostic, getColumn)
{
    ZEND_PARSE_PARAMETERS_NONE();
    diagnostic_obj *intern = php_cparser_fetch<CXDiagnostic>(Z_OBJ_P(getThis()));

    CXSourceLocation loc = clang_getDiagnosticLocation(intern->native);
    CXFile file;
    unsigned line, col, offset;
    clang_getExpansionLocation(loc, &file, &line, &col, &offset);

    RETVAL_LONG(col);
}
