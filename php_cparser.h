#ifndef PHP_CPARSER_H
#define PHP_CPARSER_H

extern zend_module_entry cparser_module_entry;
#define phpext_cparser_ptr &cparser_module_entry
#define PHP_CPARSER_VERSION "0.1.0"

// ---------------------------
// Class entries
// ---------------------------
extern zend_class_entry *cparser_translationunit_ce;
extern zend_class_entry *cparser_cursor_ce;
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
#include <stack>
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
    size_t sz = sizeof(cparser_obj<T>) + zend_object_properties_size(ce);
    cparser_obj<T> *intern = (cparser_obj<T> *)ecalloc(1, sz);

    zend_object_std_init(&intern->std, ce);
    object_properties_init(&intern->std, ce);

    intern->std.handlers = ce->default_object_handlers;

    return &intern->std;
}

template <typename T>
void cparser_object_free(zend_object *object)
{
    if (!object)
        return;
    cparser_obj<T> *intern = (cparser_obj<T> *)((char *)object - XtOffsetOf(cparser_obj<T>, std));
    zend_object_std_dtor(object);
}

typedef struct _ast_cursor_iterator
{
    std::stack<CXCursor> stack; // Lazy traversal stack
    CXCursor current;           // Current cursor
    CXCursor root;              // Root cursor (from TU or parent Cursor)
    bool done;                  // Whether traversal is complete
    zval source_obj;            // Reference to TranslationUnit or Cursor
    int filter_kind;            // Optional CXCursorKind filter
    zend_object std;            // Zend object header
} ast_cursor_iterator;

static inline ast_cursor_iterator *Z_AST_IT_P(zend_object *obj)
{
    return (ast_cursor_iterator *)((char *)obj - XtOffsetOf(ast_cursor_iterator, std));
}

static void ast_cursor_iterator_free(zend_object *object)
{
    ast_cursor_iterator *it = Z_AST_IT_P(object);

    // Clear the stack safely
    while (!it->stack.empty())
    {
        it->stack.pop();
    }

    /* release reference to TU object */
    if (!Z_ISUNDEF(it->source_obj))
    {
        zval_ptr_dtor(&it->source_obj);
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
    it->done = false;
    ZVAL_UNDEF(&it->source_obj);

    return &it->std;
}

#include <vector>
struct collect_data
{
    std::vector<CXCursor> vec;
};

using cparser_tu = cparser_obj<CXTranslationUnit>;

/* create and populate an iterator object for classes */
static inline zval ast_create_iterator_from_tu(zval *tu_zv, int filter_kind)
{
    zval it_obj;
    object_init_ex(&it_obj, cparser_classiterator_ce);

    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ(it_obj));
    if (!it)
    {
        ZVAL_NULL(&it_obj);
        return it_obj;
    }

    it->filter_kind = filter_kind;

    ZVAL_UNDEF(&it->source_obj);
    ZVAL_COPY(&it->source_obj, tu_zv);

    // Fetch the native TU
    cparser_tu *tu_intern = php_cparser_fetch<CXTranslationUnit>(Z_OBJ_P(tu_zv));
    if (!tu_intern || !tu_intern->native)
    {
        return it_obj;
    }

    CXCursor root = clang_getTranslationUnitCursor(tu_intern->native);

    // Initialize stack with the root cursor for lazy iteration
    while (!it->stack.empty())
    {
        it->stack.pop();
    }
    it->stack.push(root);
    it->done = false;

    return it_obj;
}

#endif

#endif /* PHP_CPARSER_H */
