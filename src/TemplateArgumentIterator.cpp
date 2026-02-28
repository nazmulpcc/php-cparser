extern "C"
{
#include "php.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"
#include "NativeIterators.h"

using cparser_type = cparser_obj<CXType>;
using cparser_targ_it = cparser_obj<NativeTemplateArgumentIterator>;

template <>
void cparser_object_free<NativeTemplateArgumentIterator>(zend_object *obj)
{
    cparser_targ_it *intern = php_cparser_fetch<NativeTemplateArgumentIterator>(obj);
    if (!Z_ISUNDEF(intern->native.owner))
    {
        zval_ptr_dtor(&intern->native.owner);
        ZVAL_UNDEF(&intern->native.owner);
    }
    zend_object_std_dtor(obj);
}

ZEND_METHOD(CParser_TemplateArgumentIterator, __construct)
{
    zval *type_zv;
    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_OBJECT_OF_CLASS(type_zv, cparser_type_ce)
    ZEND_PARSE_PARAMETERS_END();

    cparser_type *type = php_cparser_fetch<CXType>(Z_OBJ_P(type_zv));
    cparser_targ_it *it = php_cparser_fetch<NativeTemplateArgumentIterator>(Z_OBJ_P(getThis()));

    it->native.source = type->native;
    it->native.count = clang_Type_getNumTemplateArguments(type->native);
    if (it->native.count < 0)
    {
        it->native.count = 0;
    }
    it->native.index = 0;
    ZVAL_COPY(&it->native.owner, type_zv);
}

ZEND_METHOD(CParser_TemplateArgumentIterator, current)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_targ_it *it = php_cparser_fetch<NativeTemplateArgumentIterator>(Z_OBJ_P(getThis()));
    if (it->native.index < 0 || it->native.index >= it->native.count)
    {
        RETURN_NULL();
    }

    CXType argType = clang_Type_getTemplateArgumentAsType(it->native.source, it->native.index);
    object_init_ex(return_value, cparser_templateargument_ce);
    php_cparser_fetch<CXType>(Z_OBJ_P(return_value))->native = argType;
}

ZEND_METHOD(CParser_TemplateArgumentIterator, key)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_targ_it *it = php_cparser_fetch<NativeTemplateArgumentIterator>(Z_OBJ_P(getThis()));
    RETURN_LONG((zend_long)it->native.index);
}

ZEND_METHOD(CParser_TemplateArgumentIterator, next)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_targ_it *it = php_cparser_fetch<NativeTemplateArgumentIterator>(Z_OBJ_P(getThis()));
    if (it->native.index < it->native.count)
    {
        ++it->native.index;
    }
    RETURN_NULL();
}

ZEND_METHOD(CParser_TemplateArgumentIterator, rewind)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_targ_it *it = php_cparser_fetch<NativeTemplateArgumentIterator>(Z_OBJ_P(getThis()));
    it->native.index = 0;
    RETURN_NULL();
}

ZEND_METHOD(CParser_TemplateArgumentIterator, valid)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_targ_it *it = php_cparser_fetch<NativeTemplateArgumentIterator>(Z_OBJ_P(getThis()));
    RETURN_BOOL(it->native.index >= 0 && it->native.index < it->native.count);
}
