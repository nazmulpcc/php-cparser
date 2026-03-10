extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}

#include "php_cparser.h"
#include "NativeCXCursorIterator.h"

using cparser_cursor = cparser_obj<CXCursor>;

#if defined(HAVE_DECL_CLANG_CXXMETHOD_ISDELETED) && HAVE_DECL_CLANG_CXXMETHOD_ISDELETED
#define HAVE_CLANG_CXXMETHOD_ISDELETED 1
#endif

#if defined(HAVE_DECL_CLANG_CXXMETHOD_ISDEFAULTED) && HAVE_DECL_CLANG_CXXMETHOD_ISDEFAULTED
#define HAVE_CLANG_CXXMETHOD_ISDEFAULTED 1
#endif

#if defined(HAVE_DECL_CLANG_CXXCONSTRUCTOR_ISCOPYCONSTRUCTOR) && HAVE_DECL_CLANG_CXXCONSTRUCTOR_ISCOPYCONSTRUCTOR
#define HAVE_CLANG_CXXCONSTRUCTOR_ISCOPYCONSTRUCTOR 1
#endif

#if defined(HAVE_DECL_CLANG_CXXCONSTRUCTOR_ISMOVECONSTRUCTOR) && HAVE_DECL_CLANG_CXXCONSTRUCTOR_ISMOVECONSTRUCTOR
#define HAVE_CLANG_CXXCONSTRUCTOR_ISMOVECONSTRUCTOR 1
#endif

#if defined(HAVE_DECL_CLANG_CXXCONSTRUCTOR_ISDEFAULTCONSTRUCTOR) && HAVE_DECL_CLANG_CXXCONSTRUCTOR_ISDEFAULTCONSTRUCTOR
#define HAVE_CLANG_CXXCONSTRUCTOR_ISDEFAULTCONSTRUCTOR 1
#endif

#if defined(HAVE_DECL_CLANG_CXXMETHOD_ISEXPLICIT) && HAVE_DECL_CLANG_CXXMETHOD_ISEXPLICIT
#define HAVE_CLANG_CXXMETHOD_ISEXPLICIT 1
#endif

#if defined(HAVE_DECL_CLANG_ISVIRTUALBASE) && HAVE_DECL_CLANG_ISVIRTUALBASE
#define HAVE_CLANG_ISVIRTUALBASE 1
#endif

#ifndef HAVE_CLANG_CXXMETHOD_ISDELETED
#if defined(CINDEX_VERSION_MINOR) && CINDEX_VERSION_MINOR >= 32
#define HAVE_CLANG_CXXMETHOD_ISDELETED 1
#endif
#endif

#ifndef HAVE_CLANG_CXXMETHOD_ISDEFAULTED
#if defined(CINDEX_VERSION_MINOR) && CINDEX_VERSION_MINOR >= 32
#define HAVE_CLANG_CXXMETHOD_ISDEFAULTED 1
#endif
#endif

#ifndef HAVE_CLANG_CXXCONSTRUCTOR_ISCOPYCONSTRUCTOR
#if defined(CINDEX_VERSION_MINOR) && CINDEX_VERSION_MINOR >= 32
#define HAVE_CLANG_CXXCONSTRUCTOR_ISCOPYCONSTRUCTOR 1
#endif
#endif

#ifndef HAVE_CLANG_CXXCONSTRUCTOR_ISMOVECONSTRUCTOR
#if defined(CINDEX_VERSION_MINOR) && CINDEX_VERSION_MINOR >= 32
#define HAVE_CLANG_CXXCONSTRUCTOR_ISMOVECONSTRUCTOR 1
#endif
#endif

#ifndef HAVE_CLANG_CXXCONSTRUCTOR_ISDEFAULTCONSTRUCTOR
#if defined(CINDEX_VERSION_MINOR) && CINDEX_VERSION_MINOR >= 32
#define HAVE_CLANG_CXXCONSTRUCTOR_ISDEFAULTCONSTRUCTOR 1
#endif
#endif

#ifndef HAVE_CLANG_CXXMETHOD_ISEXPLICIT
#if defined(CINDEX_VERSION_MINOR) && CINDEX_VERSION_MINOR >= 64
#define HAVE_CLANG_CXXMETHOD_ISEXPLICIT 1
#endif
#endif

#ifndef HAVE_CLANG_ISVIRTUALBASE
#if defined(CINDEX_VERSION_MINOR) && CINDEX_VERSION_MINOR >= 32
#define HAVE_CLANG_ISVIRTUALBASE 1
#endif
#endif

static inline void cparser_type_init_object(zval *zv, CXType type, const CXCursor *origin_cursor = nullptr)
{
    object_init_ex(zv, cparser_type_ce);
    auto *type_intern = php_cparser_fetch<cparser_native_type>(Z_OBJ_P(zv));
    type_intern->native.type = type;
    if (origin_cursor && !clang_Cursor_isNull(*origin_cursor))
    {
        type_intern->native.origin_cursor = *origin_cursor;
        type_intern->native.has_origin_cursor = 1;
    }
    else
    {
        type_intern->native.origin_cursor = clang_getNullCursor();
        type_intern->native.has_origin_cursor = 0;
    }
}

struct BaseClassCollectData
{
    zval *retval;
};

static CXCursor cparser_resolve_base_class_cursor(CXCursor base_specifier, bool class_only = true)
{
    CXType base_type = clang_getCursorType(base_specifier);
    if (base_type.kind == CXType_Invalid)
    {
        return clang_getNullCursor();
    }

    // Prefer canonical type so aliases/typedefs resolve to the record declaration.
    CXCursor base_decl = clang_getTypeDeclaration(clang_getCanonicalType(base_type));
    if (clang_Cursor_isNull(base_decl))
    {
        base_decl = clang_getTypeDeclaration(base_type);
    }
    if (clang_Cursor_isNull(base_decl))
    {
        base_decl = clang_getCursorReferenced(base_specifier);
    }

    if (clang_Cursor_isNull(base_decl))
    {
        return base_decl;
    }

    if (!class_only)
    {
        return base_decl;
    }

    CXCursorKind kind = clang_getCursorKind(base_decl);
    if (kind != CXCursor_ClassDecl && kind != CXCursor_StructDecl && kind != CXCursor_ClassTemplate)
    {
        return clang_getNullCursor();
    }

    return base_decl;
}

// get specific cursor object given a CXCursor
void cparser_create_cursor(CXCursor *cursor, zval *return_value)
{
    zend_class_entry *ce = nullptr;
    CXCursorKind kind = clang_getCursorKind(*cursor);

    switch (kind)
    {
    case CXCursor_ClassDecl:
    case CXCursor_StructDecl:
        ce = cparser_classcursor_ce;
        break;
    case CXCursor_CXXMethod:
    case CXCursor_Constructor:
    case CXCursor_Destructor:
    case CXCursor_ConversionFunction:
        ce = cparser_methodcursor_ce;
        break;
    case CXCursor_FunctionDecl:
        ce = cparser_functioncursor_ce;
        break;
    case CXCursor_FieldDecl:
        ce = cparser_fieldcursor_ce;
        break;
    case CXCursor_EnumDecl:
        ce = cparser_enumcursor_ce;
        break;
    case CXCursor_EnumConstantDecl:
        ce = cparser_enumconstantcursor_ce;
        break;
    case CXCursor_ParmDecl:
        ce = cparser_parametercursor_ce;
        break;
    case CXCursor_Namespace:
        ce = cparser_namespacecursor_ce;
        break;
    case CXCursor_TypeAliasDecl:
    case CXCursor_TypedefDecl:
        ce = cparser_typealiascursor_ce;
        break;
    case CXCursor_CXXBaseSpecifier:
        ce = cparser_basespecifier_ce;
        break;
    default:
        ce = cparser_cursor_ce;
        break;
    }

    object_init_ex(return_value, ce);
    php_cparser_fetch<CXCursor>(Z_OBJ_P(return_value))->native = *cursor;
}

#define RETURN_CURSOR_WITH_FILTER(kind)                                              \
    do                                                                               \
    {                                                                                \
        object_init_ex(return_value, cparser_cursoriterator_ce);                     \
        auto *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));              \
        auto *it = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value)); \
        it->native.clearOwner();                                                      \
        it->native = NativeCXCursorIterator(intern->native, kind, false);            \
        ZVAL_COPY(&it->native.owner, getThis());                                      \
    } while (0);

#define RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(kind)                                 \
    do                                                                               \
    {                                                                                \
        object_init_ex(return_value, cparser_cursoriterator_ce);                     \
        auto *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));              \
        auto *it = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value)); \
        it->native.clearOwner();                                                      \
        it->native = NativeCXCursorIterator(intern->native, kind, false, false);     \
        ZVAL_COPY(&it->native.owner, getThis());                                      \
    } while (0);

ZEND_METHOD(CParser_Cursor, getKind)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    RETURN_LONG((zend_long)clang_getCursorKind(intern->native));
}

ZEND_METHOD(CParser_Cursor, getSpelling)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXString cxstr = clang_getCursorSpelling(intern->native);
    const char *cstr = clang_getCString(cxstr);
    if (cstr)
        RETVAL_STRING(cstr);
    else
        RETVAL_EMPTY_STRING();
    clang_disposeString(cxstr);
}

ZEND_METHOD(CParser_Cursor, getDisplayName)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXString cxstr = clang_getCursorDisplayName(intern->native);
    const char *cstr = clang_getCString(cxstr);
    if (cstr)
        RETVAL_STRING(cstr);
    else
        RETVAL_EMPTY_STRING();
    clang_disposeString(cxstr);
}

ZEND_METHOD(CParser_Cursor, getLocation)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXSourceLocation loc = clang_getCursorLocation(intern->native);
    CXFile file;
    unsigned line = 0, column = 0, offset = 0;
    clang_getSpellingLocation(loc, &file, &line, &column, &offset);

    array_init(return_value);

    if (file)
    {
        CXString fname = clang_getFileName(file);
        const char *fstr = clang_getCString(fname);
        if (fstr)
            add_assoc_string(return_value, "file", (char *)fstr);
        else
            add_assoc_null(return_value, "file");
        clang_disposeString(fname);
    }
    else
    {
        add_assoc_null(return_value, "file");
    }

    add_assoc_long(return_value, "line", line);
    add_assoc_long(return_value, "column", column);
    add_assoc_long(return_value, "offset", offset);
}

ZEND_METHOD(CParser_Cursor, getType)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXType t = clang_getCursorType(intern->native);
    if (t.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }
    cparser_type_init_object(return_value, t, &intern->native);
}

ZEND_METHOD(CParser_Cursor, isDefinition)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    bool is_def = clang_isCursorDefinition(intern->native);
    RETURN_BOOL(is_def);
}

ZEND_METHOD(CParser_Cursor, getCanonical)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXCursor canonical = clang_getCanonicalCursor(intern->native);
    if (clang_Cursor_isNull(canonical))
    {
        RETURN_NULL();
    }
    cparser_create_cursor(&canonical, return_value);
}

ZEND_METHOD(CParser_Cursor, getParent)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXCursor parent = clang_getCursorSemanticParent(intern->native);
    if (clang_Cursor_isNull(parent))
    {
        RETURN_NULL();
    }
    cparser_create_cursor(&parent, return_value);
}

ZEND_METHOD(CParser_Cursor, getChildren)
{
    zend_long filter_kind = 0;

    ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(filter_kind)
    ZEND_PARSE_PARAMETERS_END();

    RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(filter_kind);
}

struct AnnotationCollectData
{
    zval *retval;
};

ZEND_METHOD(CParser_Cursor, getAnnotations)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    array_init(return_value);

    AnnotationCollectData data = {return_value};
    clang_visitChildren(
        intern->native,
        [](CXCursor c, CXCursor /*parent*/, CXClientData client_data)
        {
            if (clang_getCursorKind(c) != CXCursor_AnnotateAttr)
            {
                return CXChildVisit_Continue;
            }

            CXString spelling = clang_getCursorSpelling(c);
            const char *text = clang_getCString(spelling);
            if (text && text[0] != '\0')
            {
                add_next_index_string(static_cast<AnnotationCollectData *>(client_data)->retval, text);
            }
            clang_disposeString(spelling);

            return CXChildVisit_Continue;
        },
        &data);
}

ZEND_METHOD(CParser_ClassCursor, getBases)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    array_init(return_value);

    BaseClassCollectData data = {return_value};
    clang_visitChildren(
        intern->native,
        [](CXCursor c, CXCursor /*parent*/, CXClientData client_data)
        {
            if (clang_getCursorKind(c) != CXCursor_CXXBaseSpecifier)
            {
                return CXChildVisit_Continue;
            }

            CXCursor base_decl = cparser_resolve_base_class_cursor(c, true);
            if (clang_Cursor_isNull(base_decl))
            {
                return CXChildVisit_Continue;
            }

            zval zcursor;
            cparser_create_cursor(&base_decl, &zcursor);
            add_next_index_zval(static_cast<BaseClassCollectData *>(client_data)->retval, &zcursor);

            return CXChildVisit_Continue;
        },
        &data);
}

ZEND_METHOD(CParser_ClassCursor, getBaseSpecifiers)
{
    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(CXCursor_CXXBaseSpecifier);
}

ZEND_METHOD(CParser_ClassCursor, getMethods)
{
    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(CXCursor_CXXMethod);
}

ZEND_METHOD(CParser_ClassCursor, getFields)
{
    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(CXCursor_FieldDecl);
}

ZEND_METHOD(CParser_ClassCursor, getInnerClasses)
{
    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(CXCursor_ClassDecl);
}

ZEND_METHOD(CParser_ClassCursor, getEnums)
{
    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(CXCursor_EnumDecl);
}

ZEND_METHOD(CParser_ClassCursor, isAbstract)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_BOOL(clang_CXXRecord_isAbstract(intern->native));
}

ZEND_METHOD(CParser_ClassCursor, isStruct)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_BOOL(clang_getCursorKind(intern->native) == CXCursor_StructDecl);
}

ZEND_METHOD(CParser_MethodCursor, getReturnType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    CXType t = clang_getCursorResultType(intern->native);
    cparser_type_init_object(return_value, t, &intern->native);
}

ZEND_METHOD(CParser_MethodCursor, getParameters)
{
    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(CXCursor_ParmDecl);
}

ZEND_METHOD(CParser_MethodCursor, getBaseMethods)
{
    ZEND_PARSE_PARAMETERS_NONE();

    auto *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXCursor *overridden = nullptr;
    unsigned num = 0;
    clang_getOverriddenCursors(intern->native, &overridden, &num);

    array_init(return_value);

    for (unsigned i = 0; i < num; ++i)
    {
        zval zcursor;
        cparser_create_cursor(&overridden[i], &zcursor);
        add_next_index_zval(return_value, &zcursor);
    }

    clang_disposeOverriddenCursors(overridden);
}

ZEND_METHOD(CParser_MethodCursor, isStatic)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_BOOL(clang_CXXMethod_isStatic(intern->native));
}

ZEND_METHOD(CParser_MethodCursor, isConst)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_BOOL(clang_CXXMethod_isConst(intern->native));
}

ZEND_METHOD(CParser_MethodCursor, isOverride)
{
    ZEND_PARSE_PARAMETERS_NONE();
    CXCursor *overridden = nullptr;
    unsigned num = 0;
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    clang_getOverriddenCursors(intern->native, &overridden, &num);
    clang_disposeOverriddenCursors(overridden);
    RETURN_BOOL(num > 0);
}

ZEND_METHOD(CParser_MethodCursor, isVirtual)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_BOOL(clang_CXXMethod_isVirtual(intern->native));
}

ZEND_METHOD(CParser_MethodCursor, isPureVirtual)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_BOOL(clang_CXXMethod_isPureVirtual(intern->native));
}

ZEND_METHOD(CParser_MethodCursor, getAccessSpecifier)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_LONG((zend_long)clang_getCXXAccessSpecifier(intern->native));
}

ZEND_METHOD(CParser_MethodCursor, isDeleted)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
    {
        RETURN_FALSE;
    }
#ifdef HAVE_CLANG_CXXMETHOD_ISDELETED
    RETURN_BOOL(clang_CXXMethod_isDeleted(intern->native));
#else
    RETURN_FALSE;
#endif
}

ZEND_METHOD(CParser_MethodCursor, isDefaulted)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
    {
        RETURN_FALSE;
    }
#ifdef HAVE_CLANG_CXXMETHOD_ISDEFAULTED
    RETURN_BOOL(clang_CXXMethod_isDefaulted(intern->native));
#else
    RETURN_FALSE;
#endif
}

ZEND_METHOD(CParser_MethodCursor, isExplicit)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
    {
        RETURN_FALSE;
    }
#ifdef HAVE_CLANG_CXXMETHOD_ISEXPLICIT
    RETURN_BOOL(clang_CXXMethod_isExplicit(intern->native));
#else
    RETURN_FALSE;
#endif
}

ZEND_METHOD(CParser_MethodCursor, isCopyConstructor)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern || clang_getCursorKind(intern->native) != CXCursor_Constructor)
    {
        RETURN_FALSE;
    }
#ifdef HAVE_CLANG_CXXCONSTRUCTOR_ISCOPYCONSTRUCTOR
    RETURN_BOOL(clang_CXXConstructor_isCopyConstructor(intern->native));
#else
    RETURN_FALSE;
#endif
}

ZEND_METHOD(CParser_MethodCursor, isMoveConstructor)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern || clang_getCursorKind(intern->native) != CXCursor_Constructor)
    {
        RETURN_FALSE;
    }
#ifdef HAVE_CLANG_CXXCONSTRUCTOR_ISMOVECONSTRUCTOR
    RETURN_BOOL(clang_CXXConstructor_isMoveConstructor(intern->native));
#else
    RETURN_FALSE;
#endif
}

ZEND_METHOD(CParser_MethodCursor, isDefaultConstructor)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern || clang_getCursorKind(intern->native) != CXCursor_Constructor)
    {
        RETURN_FALSE;
    }
#ifdef HAVE_CLANG_CXXCONSTRUCTOR_ISDEFAULTCONSTRUCTOR
    RETURN_BOOL(clang_CXXConstructor_isDefaultConstructor(intern->native));
#else
    RETURN_FALSE;
#endif
}

ZEND_METHOD(CParser_MethodCursor, isFinal)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
    {
        RETURN_FALSE;
    }
    bool is_final = false;
    clang_visitChildren(
        intern->native,
        [](CXCursor c, CXCursor /*parent*/, CXClientData client_data)
        {
            bool *state = reinterpret_cast<bool *>(client_data);
            if (clang_getCursorKind(c) == CXCursor_CXXFinalAttr)
            {
                *state = true;
                return CXChildVisit_Break;
            }
            return CXChildVisit_Continue;
        },
        &is_final);
    RETURN_BOOL(is_final);
}

ZEND_METHOD(CParser_FunctionCursor, getReturnType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    CXType t = clang_getCursorResultType(intern->native);
    cparser_type_init_object(return_value, t, &intern->native);
}

ZEND_METHOD(CParser_FunctionCursor, getParameters)
{
    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_DIRECT_CHILD_CURSOR_WITH_FILTER(CXCursor_ParmDecl);
}

ZEND_METHOD(CParser_FunctionCursor, getNumArguments)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    int n = clang_Cursor_getNumArguments(intern->native);
    RETURN_LONG(n);
}

ZEND_METHOD(CParser_FieldCursor, getType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    CXType t = clang_getCursorType(intern->native);
    if (t.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }
    cparser_type_init_object(return_value, t, &intern->native);
}

ZEND_METHOD(CParser_FieldCursor, getAccessSpecifier)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_LONG((zend_long)clang_getCXXAccessSpecifier(intern->native));
}

ZEND_METHOD(CParser_FieldCursor, isStatic)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    RETURN_BOOL(clang_Cursor_getStorageClass(intern->native) == CX_SC_Static);
}

ZEND_METHOD(CParser_EnumCursor, getConstants)
{
    ZEND_PARSE_PARAMETERS_NONE();

    RETURN_CURSOR_WITH_FILTER(CXCursor_EnumConstantDecl);
}

ZEND_METHOD(CParser_EnumCursor, getIntegerType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    CXType t = clang_getEnumDeclIntegerType(intern->native);
    cparser_type_init_object(return_value, t, &intern->native);
}

ZEND_METHOD(CParser_EnumConstantCursor, getValue)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    long long val = clang_getEnumConstantDeclValue(intern->native);
    RETURN_LONG(val);
}

ZEND_METHOD(CParser_ParameterCursor, getType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    CXType t = clang_getCursorType(intern->native);
    if (t.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }
    cparser_type_init_object(return_value, t, &intern->native);
}

ZEND_METHOD(CParser_ParameterCursor, isConstQualified)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();
    CXType t = clang_getCursorType(intern->native);
    RETURN_BOOL(clang_isConstQualifiedType(t));
}

ZEND_METHOD(CParser_ParameterCursor, hasDefaultValue)
{
    ZEND_PARSE_PARAMETERS_NONE();

    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXCursor cur = intern->native;
    CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cur);

    // Quick heuristic: if the parameter cursor has any expression child, it
    // very likely represents a default argument (e.g. IntegerLiteral, CallExpr...).
    bool has_expr_child = false;
    clang_visitChildren(
        cur,
        [](CXCursor c, CXCursor parent, CXClientData client_data)
        {
            bool *found = reinterpret_cast<bool *>(client_data);
            CXCursorKind k = clang_getCursorKind(c);
            if (clang_isExpression(k))
            {
                *found = true;
                return CXChildVisit_Break;
            }
            return CXChildVisit_Continue;
        },
        &has_expr_child);

    if (has_expr_child)
    {
        RETURN_BOOL(true);
    }

    // Fallback: tokenize the source range for the parameter and look for an
    // '=' punctuation token which indicates a default value was written.
    CXSourceRange range = clang_getCursorExtent(cur);
    CXToken *tokens = nullptr;
    unsigned numTokens = 0;

    clang_tokenize(tu, range, &tokens, &numTokens);
    bool has_default = false;
    for (unsigned i = 0; i < numTokens; ++i)
    {
        CXString ts = clang_getTokenSpelling(tu, tokens[i]);
        const char *tstr = clang_getCString(ts);
        if (tstr && strcmp(tstr, "=") == 0)
        {
            has_default = true;
            clang_disposeString(ts);
            break;
        }
        clang_disposeString(ts);
    }

    if (tokens)
    {
        clang_disposeTokens(tu, tokens, numTokens);
    }

    RETURN_BOOL(has_default);
}

ZEND_METHOD(CParser_TypeAliasCursor, getUnderlyingType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
        RETURN_NULL();

    CXType t = clang_getTypedefDeclUnderlyingType(intern->native);
    if (t.kind == CXType_Invalid)
    {
        t = clang_getCanonicalType(clang_getCursorType(intern->native));
    }
    if (t.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }

    cparser_type_init_object(return_value, t, &intern->native);
}

ZEND_METHOD(CParser_BaseSpecifier, getType)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
    {
        RETURN_NULL();
    }
    CXType t = clang_getCursorType(intern->native);
    if (t.kind == CXType_Invalid)
    {
        RETURN_NULL();
    }
    cparser_type_init_object(return_value, t, &intern->native);
}

ZEND_METHOD(CParser_BaseSpecifier, getAccessSpecifier)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
    {
        RETURN_NULL();
    }

    CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(intern->native);
    if (access == CX_CXXInvalidAccessSpecifier)
    {
        RETURN_NULL();
    }
    RETURN_LONG((zend_long)access);
}

ZEND_METHOD(CParser_BaseSpecifier, isVirtual)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
    {
        RETURN_FALSE;
    }
#ifdef HAVE_CLANG_ISVIRTUALBASE
    RETURN_BOOL(clang_isVirtualBase(intern->native));
#else
    RETURN_FALSE;
#endif
}

ZEND_METHOD(CParser_BaseSpecifier, getReferenced)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_cursor *intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(getThis()));
    if (!intern)
    {
        RETURN_NULL();
    }

    CXCursor referenced = cparser_resolve_base_class_cursor(intern->native, false);
    if (clang_Cursor_isNull(referenced))
    {
        RETURN_NULL();
    }
    cparser_create_cursor(&referenced, return_value);
}
