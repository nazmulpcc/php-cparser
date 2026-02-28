#ifndef NATIVE_ITERATORS_H
#define NATIVE_ITERATORS_H

#ifdef __cplusplus
#include <clang-c/Index.h>

struct NativeDiagnosticIterator
{
    CXTranslationUnit tu;
    unsigned count;
    unsigned index;
    zval owner;
};

struct NativeTemplateArgumentIterator
{
    CXType source_type;
    CXCursor source_cursor;
    zend_bool use_cursor;
    int count;
    int index;
    zval owner;
};

#endif

#endif // NATIVE_ITERATORS_H
