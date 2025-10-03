extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}
#include <clang-c/Index.h>
#include "php_cparser.h"

using enumdecl_obj = cparser_obj<CXCursor>;

ZEND_METHOD(CParser_EnumDecl, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    enumdecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXString name = clang_getCursorSpelling(intern->native);
    RETVAL_STRING(clang_getCString(name));
    clang_disposeString(name);
}

struct EnumConstCollectData
{
    zval *retval;
    zend_class_entry *ce;
};

static enum CXChildVisitResult collect_enum_constants(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    EnumConstCollectData *d = reinterpret_cast<EnumConstCollectData *>(client_data);

    if (clang_getCursorKind(cursor) == CXCursor_EnumConstantDecl)
    {
        zval zv;
        object_init_ex(&zv, d->ce);

        auto *const_intern = php_cparser_fetch<CXCursor>(Z_OBJ(zv));
        const_intern->native = cursor;

        add_next_index_zval(d->retval, &zv);
    }

    return CXChildVisit_Continue;
}

ZEND_METHOD(CParser_EnumDecl, getConstants)
{
    ZEND_PARSE_PARAMETERS_NONE();
    enumdecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    array_init(return_value);

    EnumConstCollectData data;
    data.retval = return_value;
    data.ce = cparser_enumconstant_ce; // extern declared in php_cparser.h

    clang_visitChildren(intern->native, collect_enum_constants, &data);
}

ZEND_METHOD(CParser_EnumDecl, getIntegerType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    enumdecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXType intType = clang_getEnumDeclIntegerType(intern->native);

    object_init_ex(return_value, cparser_type_ce);
    php_cparser_fetch<CXType>(Z_OBJ_P(return_value))->native = intType;
}