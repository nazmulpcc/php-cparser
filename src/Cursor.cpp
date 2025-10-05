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

ZEND_METHOD(CParser_Cursor, getCanonicalType)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXType t = clang_getCanonicalType(clang_getCursorType(intern->native));
    object_init_ex(return_value, cparser_type_ce);
    php_cparser_fetch<CXType>(Z_OBJ_P(return_value))->native = t;
}

ZEND_METHOD(CParser_Cursor, getEnumConstantValue)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    if (clang_getCursorKind(intern->native) != CXCursor_EnumConstantDecl)
        RETURN_NULL();

    CXEvalResult eval = clang_Cursor_Evaluate(intern->native);
    if (!eval)
        RETURN_NULL();

    double val = clang_EvalResult_getAsDouble(eval);
    clang_EvalResult_dispose(eval);

    RETURN_DOUBLE(val);
}

ZEND_METHOD(CParser_Cursor, getNumArguments)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    int num = clang_Cursor_getNumArguments(intern->native);
    if (num < 0)
        RETURN_NULL();
    RETURN_LONG(num);
}

ZEND_METHOD(CParser_Cursor, getArgumentType)
{
    zend_long index;
    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_LONG(index)
    ZEND_PARSE_PARAMETERS_END();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXCursor arg = clang_Cursor_getArgument(intern->native, (unsigned)index);
    if (clang_Cursor_isNull(arg))
        RETURN_NULL();

    object_init_ex(return_value, cparser_cursor_ce);
    php_cparser_fetch<CXCursor>(Z_OBJ_P(return_value))->native = arg;
}

ZEND_METHOD(CParser_Cursor, isDefinition)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_FALSE;
    RETURN_BOOL(clang_isCursorDefinition(intern->native));
}

ZEND_METHOD(CParser_Cursor, isConstQualified)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_FALSE;
    RETURN_BOOL(clang_isConstQualifiedType(clang_getCursorType(intern->native)));
}

ZEND_METHOD(CParser_Cursor, isVolatileQualified)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_FALSE;
    RETURN_BOOL(clang_isVolatileQualifiedType(clang_getCursorType(intern->native)));
}

ZEND_METHOD(CParser_Cursor, isRestrictQualified)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_FALSE;
    RETURN_BOOL(clang_isRestrictQualifiedType(clang_getCursorType(intern->native)));
}
