extern "C"
{
#include "php.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"

using methoddecl_obj = cparser_obj<CXCursor>;

ZEND_METHOD(CParser_MethodDecl, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXString spelling = clang_getCursorSpelling(intern->native);
    RETVAL_STRING(clang_getCString(spelling));
    clang_disposeString(spelling);
}

ZEND_METHOD(CParser_MethodDecl, getAccess)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    RETVAL_LONG(clang_getCXXAccessSpecifier(intern->native));
}

ZEND_METHOD(CParser_MethodDecl, isStatic)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_CXXMethod_isStatic(intern->native));
}

ZEND_METHOD(CParser_MethodDecl, isConst)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_CXXMethod_isConst(intern->native));
}

ZEND_METHOD(CParser_MethodDecl, isVirtual)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_CXXMethod_isVirtual(intern->native));
}

ZEND_METHOD(CParser_MethodDecl, isPureVirtual)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_CXXMethod_isPureVirtual(intern->native));
}

ZEND_METHOD(CParser_MethodDecl, isConstructor)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_getCursorKind(intern->native) == CXCursor_Constructor);
}

ZEND_METHOD(CParser_MethodDecl, isDestructor)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    RETVAL_BOOL(clang_getCursorKind(intern->native) == CXCursor_Destructor);
}

ZEND_METHOD(CParser_MethodDecl, getSignature)
{
    ZEND_PARSE_PARAMETERS_NONE();
    methoddecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXType funcType = clang_getCursorType(intern->native);
    CXString typeSpelling = clang_getTypeSpelling(funcType);
    RETVAL_STRING(clang_getCString(typeSpelling));
    clang_disposeString(typeSpelling);
}
