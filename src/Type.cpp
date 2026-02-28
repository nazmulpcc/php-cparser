extern "C"
{
#include "php.h"
#include "zend_exceptions.h"
}
#include "php_cparser.h"
#include "NativeIterators.h"

using type_obj = cparser_obj<cparser_native_type>;

static inline void cparser_assign_native_type(cparser_native_type *native, CXType type, const CXCursor *origin_cursor = nullptr)
{
    native->type = type;
    if (origin_cursor && !clang_Cursor_isNull(*origin_cursor))
    {
        native->origin_cursor = *origin_cursor;
        native->has_origin_cursor = 1;
    }
    else
    {
        native->origin_cursor = clang_getNullCursor();
        native->has_origin_cursor = 0;
    }
}

static inline void cparser_type_init_object(zval *zv, CXType type, const CXCursor *origin_cursor = nullptr)
{
    object_init_ex(zv, cparser_type_ce);
    auto *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(zv));
    cparser_assign_native_type(&intern->native, type, origin_cursor);
}

ZEND_METHOD(CParser_Type, toString)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(getThis()));

    CXString spelling = clang_getTypeSpelling(intern->native.type);
    const char *cspelling = clang_getCString(spelling);
    if (cspelling)
        RETVAL_STRING(cspelling);
    else
        RETVAL_EMPTY_STRING();
    clang_disposeString(spelling);
}

ZEND_METHOD(CParser_Type, isConstQualified)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_isConstQualifiedType(intern->native.type));
}

ZEND_METHOD(CParser_Type, isPointer)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(intern->native.type.kind == CXType_Pointer);
}

ZEND_METHOD(CParser_Type, isReference)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(
        intern->native.type.kind == CXType_LValueReference ||
        intern->native.type.kind == CXType_RValueReference);
}

ZEND_METHOD(CParser_Type, getPointeeType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(getThis()));

    CXType pointee = clang_getPointeeType(intern->native.type);
    if (pointee.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }

    cparser_type_init_object(return_value, pointee);
}

ZEND_METHOD(CParser_Type, getCanonicalType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(getThis()));

    CXType canon = clang_getCanonicalType(intern->native.type);
    cparser_type_init_object(return_value, canon);
}

ZEND_METHOD(CParser_Type, isTemplateSpecialization)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(getThis()));

    int nargs = -1;
    CXCursor template_arg_cursor = clang_getNullCursor();
    if (cparser_resolve_template_argument_cursor(&intern->native, &template_arg_cursor, &nargs))
    {
        RETVAL_BOOL(nargs > 0);
        return;
    }

    nargs = clang_Type_getNumTemplateArguments(intern->native.type);
    if (nargs < 0)
    {
        nargs = 0;
    }
    RETVAL_BOOL(nargs > 0);
}

ZEND_METHOD(CParser_Type, getTemplateArguments)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(getThis()));

    object_init_ex(return_value, cparser_templateargumentiterator_ce);
    auto *it = php_cparser_fetch<NativeTemplateArgumentIterator>(Z_OBJ_P(return_value));

    it->native.source_type = intern->native.type;
    it->native.source_cursor = clang_getNullCursor();
    it->native.use_cursor = 0;

    int count = -1;
    CXCursor template_arg_cursor = clang_getNullCursor();
    if (cparser_resolve_template_argument_cursor(&intern->native, &template_arg_cursor, &count))
    {
        it->native.use_cursor = 1;
        it->native.source_cursor = template_arg_cursor;
    }
    else
    {
        count = clang_Type_getNumTemplateArguments(intern->native.type);
    }

    it->native.count = count;
    if (it->native.count < 0)
    {
        it->native.count = 0;
    }
    it->native.index = 0;
    ZVAL_COPY(&it->native.owner, getThis());
}
