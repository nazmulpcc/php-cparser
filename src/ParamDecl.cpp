extern "C"
{
#include "php.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"

using paramdecl_obj = cparser_obj<CXCursor>;

ZEND_METHOD(CParser_ParamDecl, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    paramdecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXString spelling = clang_getCursorSpelling(intern->native);
    RETVAL_STRING(clang_getCString(spelling));
    clang_disposeString(spelling);
}

ZEND_METHOD(CParser_ParamDecl, getType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    paramdecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXType type = clang_getCursorType(intern->native);
    CXString typeSpelling = clang_getTypeSpelling(type);
    RETVAL_STRING(clang_getCString(typeSpelling));
    clang_disposeString(typeSpelling);
}