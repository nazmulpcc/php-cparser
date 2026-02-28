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
extern zend_class_entry *cparser_diagnosticiterator_ce;
extern zend_class_entry *cparser_templateargumentiterator_ce;

#ifdef __cplusplus

#include <clang-c/Index.h>
#include <stack>

struct cparser_native_translation_unit
{
    CXIndex index;
    CXTranslationUnit tu;
};

struct cparser_native_type
{
    CXType type;
    CXCursor origin_cursor;
    zend_bool has_origin_cursor;
};

struct cparser_native_template_argument
{
    CXCursor source_cursor;
    CXType source_type;
    unsigned index;
    zend_bool use_cursor;
};

static inline bool cparser_try_template_argument_cursor(CXCursor cursor, int *count_out)
{
    if (clang_Cursor_isNull(cursor))
    {
        return false;
    }
    int count = clang_Cursor_getNumTemplateArguments(cursor);
    if (count < 0)
    {
        return false;
    }
    *count_out = count;
    return true;
}

static inline bool cparser_resolve_template_argument_cursor(const cparser_native_type *type_native, CXCursor *cursor_out, int *count_out)
{
    if (type_native->has_origin_cursor)
    {
        if (cparser_try_template_argument_cursor(type_native->origin_cursor, count_out))
        {
            *cursor_out = type_native->origin_cursor;
            return true;
        }

        CXCursor canonical_origin = clang_getCanonicalCursor(type_native->origin_cursor);
        if (cparser_try_template_argument_cursor(canonical_origin, count_out))
        {
            *cursor_out = canonical_origin;
            return true;
        }
    }

    CXCursor decl = clang_getTypeDeclaration(type_native->type);
    if (cparser_try_template_argument_cursor(decl, count_out))
    {
        *cursor_out = decl;
        return true;
    }

    CXCursor canonical_decl = clang_getTypeDeclaration(clang_getCanonicalType(type_native->type));
    if (cparser_try_template_argument_cursor(canonical_decl, count_out))
    {
        *cursor_out = canonical_decl;
        return true;
    }

    return false;
}

class NativeCXCursorIterator;

void cparser_create_cursor(CXCursor *cursor, zval *return_value);

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

template <>
void cparser_object_free<NativeCXCursorIterator>(zend_object *object);

static inline int cparser_compare_cursors(zval *object1, zval *object2)
{
    cparser_obj<CXCursor> *intern1 = php_cparser_fetch<CXCursor>(Z_OBJ_P(object1));
    cparser_obj<CXCursor> *intern2 = php_cparser_fetch<CXCursor>(Z_OBJ_P(object2));
    if (clang_equalCursors(intern1->native, intern2->native))
        return 0;
    return 1;
}

#endif

#endif /* PHP_CPARSER_H */
