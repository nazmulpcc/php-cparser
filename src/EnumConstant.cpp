extern "C"
{
#include "php.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"
#include <clang-c/Index.h>

using enumconstant_obj = cparser_obj<CXCursor>;

ZEND_METHOD(CParser_EnumConstant, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    enumconstant_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXString spelling = clang_getCursorSpelling(intern->native);
    RETVAL_STRING(clang_getCString(spelling));
    clang_disposeString(spelling);
}

ZEND_METHOD(CParser_EnumConstant, getValue)
{
    ZEND_PARSE_PARAMETERS_NONE();
    enumconstant_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    long long val = clang_getEnumConstantDeclValue(intern->native);
    RETVAL_LONG(val);
}