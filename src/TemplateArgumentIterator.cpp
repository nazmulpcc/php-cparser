extern "C"
{
#include "php.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"
#include "NativeIterators.h"

using cparser_type = cparser_obj<cparser_native_type>;
using cparser_targ_it = cparser_obj<NativeTemplateArgumentIterator>;

static inline void cparser_init_template_argument_iterator(NativeTemplateArgumentIterator *native, const cparser_native_type *type_native)
{
    native->source_type = type_native->type;
    native->source_cursor = clang_getNullCursor();
    native->use_cursor = 0;

    int count = -1;
    CXCursor template_arg_cursor = clang_getNullCursor();
    if (cparser_resolve_template_argument_cursor(type_native, &template_arg_cursor, &count))
    {
        native->use_cursor = 1;
        native->source_cursor = template_arg_cursor;
    }
    else
    {
        count = clang_Type_getNumTemplateArguments(type_native->type);
    }

    native->count = count;
    if (native->count < 0)
    {
        native->count = 0;
    }
    native->index = 0;
}

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

    cparser_type *type = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(type_zv));
    cparser_targ_it *it = php_cparser_fetch<NativeTemplateArgumentIterator>(Z_OBJ_P(getThis()));

    cparser_init_template_argument_iterator(&it->native, &type->native);
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

    object_init_ex(return_value, cparser_templateargument_ce);
    auto *arg = php_cparser_fetch<cparser_native_template_argument>(Z_OBJ_P(return_value));
    arg->native.source_type = it->native.source_type;
    arg->native.source_cursor = it->native.source_cursor;
    arg->native.index = (unsigned)it->native.index;
    arg->native.use_cursor = it->native.use_cursor;
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
