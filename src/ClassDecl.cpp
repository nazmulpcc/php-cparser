extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}

#include "php_cparser.h"

using classdecl_obj = cparser_obj<CXCursor>;

ZEND_METHOD(CParser_ClassDecl, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    classdecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXString spelling = clang_getCursorSpelling(intern->native);
    RETVAL_STRING(clang_getCString(spelling));
    clang_disposeString(spelling);
}

struct BasesData
{
    zval *return_value;
};

static enum CXChildVisitResult collect_bases(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    BasesData *data = (BasesData *)client_data;
    if (clang_getCursorKind(cursor) == CXCursor_CXXBaseSpecifier)
    {
        CXType t = clang_getCursorType(cursor);
        CXString s = clang_getTypeSpelling(t);
        add_next_index_string(data->return_value, clang_getCString(s));
        clang_disposeString(s);
    }
    return CXChildVisit_Continue;
}

ZEND_METHOD(CParser_ClassDecl, getBases)
{
    ZEND_PARSE_PARAMETERS_NONE();
    classdecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    array_init(return_value);

    BasesData data{return_value};
    clang_visitChildren(intern->native, collect_bases, &data);
}

ZEND_METHOD(CParser_ClassDecl, methods)
{
    ZEND_PARSE_PARAMETERS_NONE();
    array_init(return_value);
}

ZEND_METHOD(CParser_ClassDecl, innerClasses)
{
    ZEND_PARSE_PARAMETERS_NONE();
    array_init(return_value);
}

ZEND_METHOD(CParser_ClassDecl, enums)
{
    ZEND_PARSE_PARAMETERS_NONE();
    array_init(return_value);
}

ZEND_METHOD(CParser_ClassDecl, templates)
{
    ZEND_PARSE_PARAMETERS_NONE();
    array_init(return_value);
}

ZEND_METHOD(CParser_ClassDecl, isAbstract)
{
    ZEND_PARSE_PARAMETERS_NONE();
    classdecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_CXXRecord_isAbstract(intern->native));
}
