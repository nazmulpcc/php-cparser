extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}
#include <clang-c/Index.h>
#include "php_cparser.h"

// TemplateArgument wraps a CXType (we use CXType for type template args)
using templarg_obj = cparser_obj<CXType>;

ZEND_METHOD(CParser_TemplateArgument, getKind)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templarg_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));

    // For now we only support type template arguments (returned as CXType)
    // and we conservatively return "type" when clang_Type_getNumTemplateArguments
    // is not relevant: but since this object wraps a CXType that is a template arg,
    // treat it as "type".
    RETVAL_STRING("type");
}

ZEND_METHOD(CParser_TemplateArgument, getType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templarg_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));

    // If the wrapped CXType is invalid, return null
    if (intern->native.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }

    object_init_ex(return_value, cparser_type_ce);
    cparser_obj<CXType> *tv = php_cparser_fetch<CXType>(Z_OBJ_P(return_value));
    tv->native = intern->native;
}

ZEND_METHOD(CParser_TemplateArgument, getValue)
{
    ZEND_PARSE_PARAMETERS_NONE();
    // We do not currently extract non-type template argument values (integral/etc.).
    // Return NULL for now.
    RETURN_NULL();
}
