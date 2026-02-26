extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"
#include "NativeCXCursorIterator.h"

using cparser_tu = cparser_obj<CXTranslationUnit>;
using cparser_iterator = cparser_obj<NativeCXCursorIterator>;

ZEND_METHOD(CParser_CursorIterator, __construct)
{
    zval *source;
    zend_long filter_kind = 0;
    ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_OBJECT(source)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(filter_kind)
    ZEND_PARSE_PARAMETERS_END();

    cparser_iterator *it = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(getThis()));

    CXCursor root;
    if (instanceof_function(Z_OBJCE_P(source), cparser_translationunit_ce))
    {
        cparser_tu *tu_intern = php_cparser_fetch<CXTranslationUnit>(Z_OBJ_P(source));
        if (!tu_intern || !tu_intern->native)
        {
            RETURN_NULL();
        }
        root = clang_getTranslationUnitCursor(tu_intern->native);
    }
    else if (instanceof_function(Z_OBJCE_P(source), cparser_cursor_ce))
    {
        auto *cursor_intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(source));
        root = cursor_intern->native;
    }
    else
    {
        zend_throw_exception(zend_ce_exception, "Invalid source object for CursorIterator", 0);
        RETURN_NULL();
    }

    // Initialize native iterator directly
    it->native = NativeCXCursorIterator(root, (int)filter_kind, false);
}

ZEND_METHOD(CParser_CursorIterator, current)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_iterator *it = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(getThis()));

    CXCursor cur = it->native.currentCursor();
    if (clang_Cursor_isNull(cur))
    {
        RETURN_NULL();
    }

    cparser_create_cursor(&cur, return_value);
}

ZEND_METHOD(CParser_CursorIterator, key)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_iterator *it = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(getThis()));
    RETURN_LONG(it->native.currentIndex());
}

ZEND_METHOD(CParser_CursorIterator, next)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_iterator *it = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(getThis()));
    it->native.nextWithIndex();
    RETURN_NULL();
}

ZEND_METHOD(CParser_CursorIterator, rewind)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_iterator *it = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(getThis()));
    it->native.rewind();
    RETURN_NULL();
}

ZEND_METHOD(CParser_CursorIterator, valid)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_iterator *it = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(getThis()));
    RETURN_BOOL(it->native.valid());
}
