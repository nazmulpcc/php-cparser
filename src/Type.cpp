extern "C"
{
#include "php.h"
#include "zend_exceptions.h"
}
#include "php_cparser.h"

using type_obj = cparser_obj<CXType>;

ZEND_METHOD(CParser_Type, toString)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));

    CXString spelling = clang_getTypeSpelling(intern->native);
    RETVAL_STRING(clang_getCString(spelling));
    clang_disposeString(spelling);
}

ZEND_METHOD(CParser_Type, isConstQualified)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_isConstQualifiedType(intern->native));
}

ZEND_METHOD(CParser_Type, isPointer)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(intern->native.kind == CXType_Pointer);
}

ZEND_METHOD(CParser_Type, isReference)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(
        intern->native.kind == CXType_LValueReference ||
        intern->native.kind == CXType_RValueReference);
}

ZEND_METHOD(CParser_Type, getPointeeType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));

    CXType pointee = clang_getPointeeType(intern->native);
    if (pointee.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }

    object_init_ex(return_value, cparser_type_ce);
    php_cparser_fetch<CXType>(Z_OBJ_P(return_value))->native = pointee;
}

ZEND_METHOD(CParser_Type, getCanonicalType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));

    CXType canon = clang_getCanonicalType(intern->native);
    object_init_ex(return_value, cparser_type_ce);
    php_cparser_fetch<CXType>(Z_OBJ_P(return_value))->native = canon;
}

ZEND_METHOD(CParser_Type, isTemplateSpecialization)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));

    int nargs = clang_Type_getNumTemplateArguments(intern->native);
    RETVAL_BOOL(nargs > 0);
}

ZEND_METHOD(CParser_Type, getTemplateArguments)
{
    ZEND_PARSE_PARAMETERS_NONE();
    type_obj *intern = php_cparser_fetch<CXType>(Z_OBJ_P(getThis()));

    int num = clang_Type_getNumTemplateArguments(intern->native);
    if (num <= 0)
    {
        array_init(return_value);
        return;
    }

    array_init_size(return_value, num);

    for (int i = 0; i < num; i++)
    {
        CXType argType = clang_Type_getTemplateArgumentAsType(intern->native, i);

        if (argType.kind != CXType_Invalid)
        {
            zval zv;
            object_init_ex(&zv, cparser_type_ce);
            php_cparser_fetch<CXType>(Z_OBJ(zv))->native = argType;
            add_next_index_zval(return_value, &zv);
        }
        else
        {
            add_next_index_null(return_value);
        }
    }
}
