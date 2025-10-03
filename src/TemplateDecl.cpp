extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}
#include <clang-c/Index.h>
#include "php_cparser.h"

// TemplateDecl wraps a CXCursor
using templatedecl_obj = cparser_obj<CXCursor>;

ZEND_METHOD(CParser_TemplateDecl, getName)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templatedecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    CXString name = clang_getCursorSpelling(intern->native);
    RETVAL_STRING(clang_getCString(name));
    clang_disposeString(name);
}

struct ParamCollectData
{
    zval *retval;
    zend_class_entry *ce;
};

static enum CXChildVisitResult collect_template_params(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    ParamCollectData *d = reinterpret_cast<ParamCollectData *>(client_data);
    CXCursorKind k = clang_getCursorKind(cursor);

    if (k == CXCursor_TemplateTypeParameter ||
        k == CXCursor_NonTypeTemplateParameter ||
        k == CXCursor_TemplateTemplateParameter)
    {

        zval zv;
        object_init_ex(&zv, d->ce);

        // assign the native CXCursor to the PHP object
        templatedecl_obj *param_intern = php_cparser_fetch<CXCursor>(Z_OBJ(zv));
        param_intern->native = cursor;

        add_next_index_zval(d->retval, &zv);
    }

    return CXChildVisit_Continue;
}

ZEND_METHOD(CParser_TemplateDecl, getParameters)
{
    ZEND_PARSE_PARAMETERS_NONE();
    templatedecl_obj *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));

    array_init(return_value);

    ParamCollectData data;
    data.retval = return_value;
    data.ce = cparser_templateparameter_ce;

    clang_visitChildren(intern->native, collect_template_params, &data);
}
