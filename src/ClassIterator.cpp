extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}

#include "php_cparser.h"

ZEND_METHOD(CParser_ClassIterator, __construct)
{
    zval *tu;
    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_OBJECT(tu)
    ZEND_PARSE_PARAMETERS_END();

    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));

    /* Store a persistent zval reference to TranslationUnit so TU won't be GC'd */
    ZVAL_COPY(&it->tu_obj, tu);
}

ZEND_METHOD(CParser_ClassIterator, current)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));

    if (it->pos >= it->count)
    {
        RETURN_NULL();
    }

    CXCursor cur = it->items[it->pos];

    /* Instantiate a ClassDecl PHP object and attach the cursor */
    zval obj;
    object_init_ex(&obj, cparser_cursor_ce);

    /* fetch intern and set native cursor */
    auto *intern = php_cparser_fetch<CXCursor>(Z_OBJ(obj));
    intern->native = cur;

    RETURN_ZVAL(&obj, 1, 1);
}

ZEND_METHOD(CParser_ClassIterator, key)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));
    if (it->pos >= it->count)
    {
        RETURN_NULL();
    }
    RETURN_LONG((zend_long)it->pos);
}

ZEND_METHOD(CParser_ClassIterator, next)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));
    if (it->pos < it->count)
        ++it->pos;
    RETURN_NULL();
}

ZEND_METHOD(CParser_ClassIterator, rewind)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));
    it->pos = 0;
    RETURN_NULL();
}

ZEND_METHOD(CParser_ClassIterator, valid)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));
    RETURN_BOOL(it->pos < it->count);
}
