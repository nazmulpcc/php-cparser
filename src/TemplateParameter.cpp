extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}
#include <clang-c/Index.h>
#include "php_cparser.h"

// TemplateParameter wraps a CXCursor
using templateparam_obj = cparser_obj<CXCursor>;

ZEND_METHOD(CParser_TemplateParameter, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templateparam_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXString name = clang_getCursorSpelling(intern->native);
    RETVAL_STRING(clang_getCString(name));
    clang_disposeString(name);
}

ZEND_METHOD(CParser_TemplateParameter, getKind)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templateparam_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXCursorKind k = clang_getCursorKind(intern->native);
    const char *kindstr = "unknown";

    switch (k)
    {
    case CXCursor_TemplateTypeParameter:
        kindstr = "type";
        break;
    case CXCursor_NonTypeTemplateParameter:
        kindstr = "non-type";
        break;
    case CXCursor_TemplateTemplateParameter:
        kindstr = "template";
        break;
    default:
        kindstr = "unknown";
        break;
    }

    RETVAL_STRING(kindstr);
}
