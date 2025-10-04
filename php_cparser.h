#ifndef PHP_CPARSER_H
#define PHP_CPARSER_H

extern zend_module_entry cparser_module_entry;
#define phpext_cparser_ptr &cparser_module_entry
#define PHP_CPARSER_VERSION "0.1.0"

// ---------------------------
// Class entries
// ---------------------------
extern zend_class_entry *cparser_translationunit_ce;
extern zend_class_entry *cparser_classdecl_ce;
extern zend_class_entry *cparser_enumconstant_ce;
extern zend_class_entry *cparser_methoddecl_ce;
extern zend_class_entry *cparser_paramdecl_ce;
extern zend_class_entry *cparser_type_ce;
extern zend_class_entry *cparser_templatedecl_ce;
extern zend_class_entry *cparser_templateparameter_ce;
extern zend_class_entry *cparser_templateargument_ce;
extern zend_class_entry *cparser_enumdecl_ce;
extern zend_class_entry *cparser_diagnostic_ce;
extern zend_class_entry *cparser_classiterator_ce;

#ifdef __cplusplus

#include <clang-c/Index.h>
template <typename T>
struct cparser_obj
{
    T native;
    zend_object std;
};

template <typename T>
inline cparser_obj<T> *php_cparser_fetch(zend_object *obj)
{
    return (cparser_obj<T> *)((char *)(obj)-XtOffsetOf(cparser_obj<T>, std));
}

template <typename T>
zend_object *cparser_object_create(zend_class_entry *ce)
{
    cparser_obj<T> *intern = (cparser_obj<T> *)ecalloc(
        1,
        sizeof(cparser_obj<T>) + zend_object_properties_size(ce));
    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);
    intern->std.handlers = &std_object_handlers;
    return &intern->std;
}

template <typename T>
void cparser_object_free(zend_object *obj)
{
    // If T requires manual cleanup, specialize this template
    zend_object_std_dtor(obj);
}

typedef enum
{
    AST_IT_CLASSES,
    AST_IT_ENUMS
} ast_it_kind;
typedef struct _ast_cursor_iterator
{
    zend_object std;

    /* Copy of CXCursor entries collected at creation time */
    CXCursor *items;
    size_t count;
    size_t pos;

    /* Keep a reference to the TranslationUnit PHP object so TU lives as long as iterator */
    zval tu_obj; /* GC-refcounted zval containing the TU object */
    ast_it_kind kind = AST_IT_CLASSES;
} ast_cursor_iterator;

static inline ast_cursor_iterator *Z_AST_IT_P(zend_object *obj)
{
    return (ast_cursor_iterator *)((char *)obj - XtOffsetOf(ast_cursor_iterator, std));
}

static void ast_cursor_iterator_free(zend_object *object)
{
    ast_cursor_iterator *it = Z_AST_IT_P(object);

    if (it->items)
    {
        efree(it->items);
        it->items = NULL;
    }

    /* release reference to TU object */
    if (!Z_ISUNDEF(it->tu_obj))
    {
        zval_ptr_dtor(&it->tu_obj);
    }

    /* call default free handler for zend_object */
    zend_object_std_dtor(&it->std);
}

static zend_object *ast_cursor_iterator_create(zend_class_entry *ce)
{
    ast_cursor_iterator *it = (ast_cursor_iterator *)ecalloc(1, sizeof(ast_cursor_iterator));
    zend_object_std_init(&it->std, ce);
    object_properties_init(&it->std, ce);

    it->std.handlers = zend_get_std_object_handlers();
    it->items = NULL;
    it->count = 0;
    it->pos = 0;
    ZVAL_UNDEF(&it->tu_obj);

    return &it->std;
}

#include <vector>
struct collect_data
{
    std::vector<CXCursor> vec;
};

/* visitor callback for classes */
static enum CXChildVisitResult collect_classes_cb(CXCursor c, CXCursor parent, CXClientData client_data)
{
    collect_data *d = (collect_data *)client_data;
    enum CXCursorKind kind = clang_getCursorKind(c);

    if (kind == CXCursor_ClassDecl || kind == CXCursor_StructDecl)
    {
        if (clang_isCursorDefinition(c))
        {
            d->vec.push_back(c);
        }
    }

    return CXChildVisit_Recurse;
}

/* visitor callback for enums */
static enum CXChildVisitResult collect_enums_cb(CXCursor c, CXCursor parent, CXClientData client_data)
{
    collect_data *d = (collect_data *)client_data;
    enum CXCursorKind kind = clang_getCursorKind(c);

    if (kind == CXCursor_EnumDecl)
    {
        if (clang_isCursorDefinition(c))
        {
            d->vec.push_back(c);
        }
    }

    return CXChildVisit_Recurse;
}

using cparser_tu = cparser_obj<CXTranslationUnit>;

/* create and populate an iterator object for classes */
static inline zval ast_create_iterator_from_tu(zval *tu_zv, ast_it_kind kind)
{
    zval it_obj;
    object_init_ex(&it_obj, cparser_classiterator_ce);

    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ(it_obj));

    /* Defensive init in case create_object didn't */
    if (!it)
    {
        // Should not happen: if object_init_ex failed, return null zval to be safe.
        ZVAL_NULL(&it_obj);
        return it_obj;
    }

    it->items = NULL;
    it->count = 0;
    it->pos = 0;
    it->kind = kind;
    ZVAL_UNDEF(&it->tu_obj);

    /* copy the TU zval so it remains alive while iterator exists */
    ZVAL_COPY(&it->tu_obj, tu_zv);

    /* fetch native TU intern */
    cparser_tu *tu_intern = php_cparser_fetch<CXTranslationUnit>(Z_OBJ_P(tu_zv));
    if (!tu_intern || !tu_intern->native)
    {
        // empty iterator object, TU invalid
        return it_obj;
    }

    CXCursor root = clang_getTranslationUnitCursor(tu_intern->native);

    collect_data cd;

    switch (kind)
    {
    case AST_IT_CLASSES:
        clang_visitChildren(root, collect_classes_cb, &cd);
        break;
    case AST_IT_ENUMS:
        clang_visitChildren(root, collect_enums_cb, &cd);
        break;
    // add more cases here...
    default:
        break;
    }

    if (!cd.vec.empty())
    {
        size_t n = cd.vec.size();
        it->items = (CXCursor *)emalloc(n * sizeof(CXCursor));
        if (!it->items)
        {
            // emalloc should either not return NULL or throw in PHP; guard anyway
            it->count = 0;
            it->pos = 0;
            return it_obj;
        }
        it->count = n;
        for (size_t i = 0; i < n; ++i)
        {
            it->items[i] = cd.vec[i]; // CXCursor is POD; copying is safe
        }
    }
    else
    {
        it->items = NULL;
        it->count = 0;
    }

    it->pos = 0;

    return it_obj;
}

#endif

#endif /* PHP_CPARSER_H */
