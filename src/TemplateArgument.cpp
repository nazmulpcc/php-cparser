extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}
#include <clang-c/Index.h>
#include "php_cparser.h"
#include <limits>

using templarg_obj = cparser_obj<cparser_native_template_argument>;

static inline const char *cparser_template_argument_kind_to_string(enum CXTemplateArgumentKind kind)
{
    switch (kind)
    {
    case CXTemplateArgumentKind_Null:
        return "null";
    case CXTemplateArgumentKind_Type:
        return "type";
    case CXTemplateArgumentKind_Declaration:
        return "declaration";
    case CXTemplateArgumentKind_NullPtr:
        return "null_ptr";
    case CXTemplateArgumentKind_Integral:
        return "integral";
    case CXTemplateArgumentKind_Template:
        return "template";
    case CXTemplateArgumentKind_TemplateExpansion:
        return "template_expansion";
    case CXTemplateArgumentKind_Expression:
        return "expression";
    case CXTemplateArgumentKind_Pack:
        return "pack";
    case CXTemplateArgumentKind_Invalid:
    default:
        return "invalid";
    }
}

static inline void cparser_type_init_object(zval *zv, CXType type)
{
    object_init_ex(zv, cparser_type_ce);
    auto *type_intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(zv));
    type_intern->native.type = type;
    type_intern->native.origin_cursor = clang_getNullCursor();
    type_intern->native.has_origin_cursor = 0;
}

ZEND_METHOD(CParser_TemplateArgument, getKind)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templarg_obj *intern = php_cparser_fetch<cparser_native_template_argument>(Z_OBJ_P(getThis()));

    if (intern->native.use_cursor && !clang_Cursor_isNull(intern->native.source_cursor))
    {
        enum CXTemplateArgumentKind kind =
            clang_Cursor_getTemplateArgumentKind(intern->native.source_cursor, intern->native.index);
        RETVAL_STRING(cparser_template_argument_kind_to_string(kind));
        return;
    }

    CXType arg_type = clang_Type_getTemplateArgumentAsType(intern->native.source_type, intern->native.index);
    RETVAL_STRING(arg_type.kind == CXType_Invalid ? "invalid" : "type");
}

ZEND_METHOD(CParser_TemplateArgument, getType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templarg_obj *intern = php_cparser_fetch<cparser_native_template_argument>(Z_OBJ_P(getThis()));

    CXType arg_type = {};
    arg_type.kind = CXType_Invalid;
    if (intern->native.use_cursor && !clang_Cursor_isNull(intern->native.source_cursor))
    {
        enum CXTemplateArgumentKind kind =
            clang_Cursor_getTemplateArgumentKind(intern->native.source_cursor, intern->native.index);
        if (kind != CXTemplateArgumentKind_Type)
        {
            RETURN_NULL();
        }
        arg_type = clang_Cursor_getTemplateArgumentType(intern->native.source_cursor, intern->native.index);
    }
    else
    {
        arg_type = clang_Type_getTemplateArgumentAsType(intern->native.source_type, intern->native.index);
    }

    if (arg_type.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }

    cparser_type_init_object(return_value, arg_type);
}

ZEND_METHOD(CParser_TemplateArgument, getValue)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templarg_obj *intern = php_cparser_fetch<cparser_native_template_argument>(Z_OBJ_P(getThis()));

    if (!intern->native.use_cursor || clang_Cursor_isNull(intern->native.source_cursor))
    {
        RETURN_NULL();
    }

    enum CXTemplateArgumentKind kind =
        clang_Cursor_getTemplateArgumentKind(intern->native.source_cursor, intern->native.index);
    if (kind != CXTemplateArgumentKind_Integral)
    {
        RETURN_NULL();
    }

    long long signed_value =
        clang_Cursor_getTemplateArgumentValue(intern->native.source_cursor, intern->native.index);
    unsigned long long unsigned_value =
        clang_Cursor_getTemplateArgumentUnsignedValue(intern->native.source_cursor, intern->native.index);

    const long long zl_min = (long long)std::numeric_limits<zend_long>::min();
    const long long zl_max = (long long)std::numeric_limits<zend_long>::max();
    const unsigned long long zl_umax = (unsigned long long)std::numeric_limits<zend_long>::max();

    if (signed_value < 0)
    {
        if (signed_value < zl_min)
        {
            RETURN_STR(strpprintf(0, "%lld", signed_value));
        }
        RETURN_LONG((zend_long)signed_value);
    }

    if (unsigned_value > zl_umax || signed_value > zl_max)
    {
        RETURN_STR(strpprintf(0, "%llu", unsigned_value));
    }

    RETURN_LONG((zend_long)unsigned_value);
}
