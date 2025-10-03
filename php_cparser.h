#ifndef PHP_CPARSER_H
#define PHP_CPARSER_H

extern zend_module_entry cparser_module_entry;
#define phpext_cparser_ptr &cparser_module_entry
#define PHP_CPARSER_VERSION "0.1.0"

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
#endif

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

#endif /* PHP_CPARSER_H */
