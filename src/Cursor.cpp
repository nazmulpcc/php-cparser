extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}

#include "php_cparser.h"

using cparser_cursor = cparser_obj<CXCursor>;

ZEND_METHOD(CParser_Cursor, getKind)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    RETURN_LONG((zend_long)clang_getCursorKind(intern->native));
}

ZEND_METHOD(CParser_Cursor, getSpelling)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXString cxstr = clang_getCursorSpelling(intern->native);
    const char *cstr = clang_getCString(cxstr);
    if (cstr)
        RETVAL_STRING(cstr);
    else
        RETVAL_EMPTY_STRING();
    clang_disposeString(cxstr);
}

ZEND_METHOD(CParser_Cursor, getLocation)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXSourceLocation loc = clang_getCursorLocation(intern->native);
    CXFile file;
    unsigned line = 0, column = 0, offset = 0;
    clang_getSpellingLocation(loc, &file, &line, &column, &offset);

    array_init(return_value);

    if (file)
    {
        CXString fname = clang_getFileName(file);
        const char *fstr = clang_getCString(fname);
        if (fstr)
            add_assoc_string(return_value, "file", (char *)fstr);
        else
            add_assoc_null(return_value, "file");
        clang_disposeString(fname);
    }
    else
    {
        add_assoc_null(return_value, "file");
    }

    add_assoc_long(return_value, "line", line);
    add_assoc_long(return_value, "column", column);
    add_assoc_long(return_value, "offset", offset);
}

ZEND_METHOD(CParser_Cursor, getType)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXType t = clang_getCursorType(intern->native);
    object_init_ex(return_value, cparser_type_ce);
    php_cparser_fetch<CXType>(Z_OBJ_P(return_value))->native = t;
}

ZEND_METHOD(CParser_Cursor, isDefinition)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    bool is_def = clang_isCursorDefinition(intern->native);
    RETURN_BOOL(is_def);
}

ZEND_METHOD(CParser_Cursor, getCanonical)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXCursor canonical = clang_getCanonicalCursor(intern->native);
    object_init_ex(return_value, cparser_cursor_ce);
    php_cparser_fetch<CXCursor>(Z_OBJ_P(return_value))->native = canonical;
}

ZEND_METHOD(CParser_Cursor, getParent)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXCursor parent = clang_getCursorSemanticParent(intern->native);
    object_init_ex(return_value, cparser_cursor_ce);
    php_cparser_fetch<CXCursor>(Z_OBJ_P(return_value))->native = parent;
}

ZEND_METHOD(CParser_Cursor, getChildren)
{
    zend_long filter_kind = 0;

    ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(filter_kind)
    ZEND_PARSE_PARAMETERS_END();

    zval it_obj;
    object_init_ex(&it_obj, cparser_classiterator_ce);
    auto it = Z_AST_IT_P(Z_OBJ(it_obj));
    ZVAL_OBJ_COPY(&it->source_obj, Z_OBJ_P(getThis()));

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    // Initialize iterator root and state
    it->root = intern->native;
    while (!it->stack.empty())
        it->stack.pop();
    it->stack.push(it->root);

    it->done = false;
    it->filter_kind = (int)filter_kind;

    RETURN_ZVAL(&it_obj, 0, 1);
}