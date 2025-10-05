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
extern zend_class_entry *cparser_classcursor_ce;
extern zend_class_entry *cparser_methodcursor_ce;
extern zend_class_entry *cparser_functioncursor_ce;
extern zend_class_entry *cparser_fieldcursor_ce;
extern zend_class_entry *cparser_enumcursor_ce;
extern zend_class_entry *cparser_enumconstantcursor_ce;
extern zend_class_entry *cparser_parametercursor_ce;
extern zend_class_entry *cparser_namespacecursor_ce;
extern zend_class_entry *cparser_typealiascursor_ce;
extern zend_class_entry *cparser_type_ce;
extern zend_class_entry *cparser_templatedecl_ce;
extern zend_class_entry *cparser_templateparameter_ce;
extern zend_class_entry *cparser_templateargument_ce;
extern zend_class_entry *cparser_enumdecl_ce;
extern zend_class_entry *cparser_diagnostic_ce;
extern zend_class_entry *cparser_cursoriterator_ce;

#ifdef __cplusplus

#include <clang-c/Index.h>
#include <stack>

void cparser_create_cursor(CXCursor *cursor, zval *return_value);
CXCursor cparser_cursor_get_first_child(CXCursor parent);

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

    zend_object std; // Zend object header
} ast_cursor_iterator;

static inline ast_cursor_iterator *Z_AST_IT_P(zend_object *obj)
{
    return (ast_cursor_iterator *)((char *)obj - XtOffsetOf(ast_cursor_iterator, std));
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
    object_init_ex(&it_obj, cparser_cursoriterator_ce);
    auto *it = Z_AST_IT_P(Z_OBJ(it_obj));
    // @todo
    return it_obj;
}

#endif

#endif /* PHP_CPARSER_H */
