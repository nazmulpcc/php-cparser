extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
#include "zend_exceptions.h"
}

#include "php_cparser.h"

#ifdef __cplusplus
#include "NativeCXCursorIterator.h"
#include "NativeIterators.h"
#include <clang-c/Index.h>
#include <cctype>
#include <fstream>
#include <string>
#include <vector>
#endif

using cparser_tu = cparser_obj<cparser_native_translation_unit>;
using cparser_inclusion = cparser_obj<cparser_native_inclusion>;

static inline zend_string *cparser_file_to_zend_string(CXFile file)
{
    if (!file)
    {
        return nullptr;
    }

    CXString cxname = clang_getFileName(file);
    const char *name = clang_getCString(cxname);
    zend_string *out = nullptr;
    if (name && name[0] != '\0')
    {
        out = zend_string_init(name, strlen(name), 0);
    }
    clang_disposeString(cxname);
    return out;
}

static inline bool cparser_read_line_from_file(const char *path, unsigned line_no, std::string *out_line)
{
    if (!path || !out_line || line_no == 0)
    {
        return false;
    }

    std::ifstream in(path);
    if (!in.is_open())
    {
        return false;
    }

    std::string line;
    for (unsigned i = 1; i <= line_no; ++i)
    {
        if (!std::getline(in, line))
        {
            return false;
        }
    }

    *out_line = line;
    return true;
}

static inline bool cparser_parse_include_spelling(const std::string &line, std::string *spelling, bool *is_angled)
{
    if (!spelling || !is_angled)
    {
        return false;
    }

    size_t include_pos = line.find("#include");
    if (include_pos == std::string::npos)
    {
        return false;
    }

    size_t pos = include_pos + 8;
    while (pos < line.size() && std::isspace(static_cast<unsigned char>(line[pos])))
    {
        ++pos;
    }
    if (pos >= line.size())
    {
        return false;
    }

    if (line[pos] == '<')
    {
        size_t end = line.find('>', pos + 1);
        if (end != std::string::npos)
        {
            *spelling = line.substr(pos, end - pos + 1);
            *is_angled = true;
            return true;
        }
    }
    else if (line[pos] == '"')
    {
        size_t end = line.find('"', pos + 1);
        if (end != std::string::npos)
        {
            *spelling = line.substr(pos, end - pos + 1);
            *is_angled = false;
            return true;
        }
    }

    size_t end = pos;
    while (end < line.size() && !std::isspace(static_cast<unsigned char>(line[end])))
    {
        ++end;
    }
    *spelling = line.substr(pos, end - pos);
    *is_angled = (!spelling->empty() && (*spelling)[0] == '<');
    return !spelling->empty();
}

static void cparser_append_inclusion_from_cursor(CXCursor cursor, zval *target)
{
    if (!target || Z_TYPE_P(target) != IS_ARRAY)
    {
        return;
    }

    CXSourceLocation loc = clang_getCursorLocation(cursor);
    CXFile source_file = nullptr;
    unsigned uline = 0;
    unsigned ucol = 0;
    unsigned uoffset = 0;
    clang_getSpellingLocation(loc, &source_file, &uline, &ucol, &uoffset);

    zend_string *source_path = cparser_file_to_zend_string(source_file);
    zend_string *included_path = cparser_file_to_zend_string(clang_getIncludedFile(cursor));
    zend_long line = static_cast<zend_long>(uline);
    zend_long column = static_cast<zend_long>(ucol);
    std::string spelling;
    bool is_angled = false;

    if (source_path && uline > 0)
    {
        std::string source_line;
        if (cparser_read_line_from_file(ZSTR_VAL(source_path), uline, &source_line))
        {
            cparser_parse_include_spelling(source_line, &spelling, &is_angled);
        }
    }

    if (spelling.empty())
    {
        CXString raw = clang_getCursorSpelling(cursor);
        const char *raw_text = clang_getCString(raw);
        if (raw_text && raw_text[0] != '\0')
        {
            spelling = raw_text;
        }
        clang_disposeString(raw);
    }

    if (spelling.empty())
    {
        if (included_path)
        {
            spelling.assign(ZSTR_VAL(included_path), ZSTR_LEN(included_path));
        }
        else
        {
            spelling = "";
        }
    }

    zval zentry;
    object_init_ex(&zentry, cparser_inclusion_ce);
    cparser_inclusion *intern = php_cparser_fetch<cparser_native_inclusion>(Z_OBJ(zentry));
    intern->native.source_file = source_path;
    intern->native.included_file = included_path;
    intern->native.spelling = zend_string_init(spelling.c_str(), spelling.size(), 0);
    intern->native.line = line;
    intern->native.column = column;
    intern->native.angled = is_angled ? 1 : 0;

    add_next_index_zval(target, &zentry);
}

static CXChildVisitResult cparser_collect_inclusion_directives(CXCursor cursor, CXCursor /*parent*/, CXClientData client_data)
{
    if (clang_getCursorKind(cursor) == CXCursor_InclusionDirective)
    {
        cparser_append_inclusion_from_cursor(cursor, static_cast<zval *>(client_data));
    }
    return CXChildVisit_Recurse;
}

template <>
void cparser_object_free<cparser_native_translation_unit>(zend_object *obj)
{
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(obj);
    if (intern->native.tu)
    {
        clang_disposeTranslationUnit(intern->native.tu);
        intern->native.tu = nullptr;
    }
    if (intern->native.index)
    {
        clang_disposeIndex(intern->native.index);
        intern->native.index = nullptr;
    }
    if (!Z_ISUNDEF(intern->native.includes_cache))
    {
        zval_ptr_dtor(&intern->native.includes_cache);
        ZVAL_UNDEF(&intern->native.includes_cache);
    }
    intern->native.includes_cache_ready = 0;
    intern->native.tu = nullptr;
    intern->native.index = nullptr;
    zend_object_std_dtor(obj);
}

template <>
void cparser_object_free<cparser_native_inclusion>(zend_object *obj)
{
    cparser_inclusion *intern = php_cparser_fetch<cparser_native_inclusion>(obj);
    if (intern->native.source_file)
    {
        zend_string_release(intern->native.source_file);
        intern->native.source_file = nullptr;
    }
    if (intern->native.included_file)
    {
        zend_string_release(intern->native.included_file);
        intern->native.included_file = nullptr;
    }
    if (intern->native.spelling)
    {
        zend_string_release(intern->native.spelling);
        intern->native.spelling = nullptr;
    }
    zend_object_std_dtor(obj);
}

#define RETURN_CURSOR_IT_WITH_FILTER(kind)                                                                           \
    do                                                                                                               \
    {                                                                                                                \
        cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));                 \
        object_init_ex(return_value, cparser_cursoriterator_ce);                                                     \
        auto *it_intern = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value));                          \
        it_intern->native.clearOwner();                                                                               \
        it_intern->native = NativeCXCursorIterator(clang_getTranslationUnitCursor(intern->native.tu), (int)kind, true); \
        ZVAL_COPY(&it_intern->native.owner, getThis());                                                               \
        return;                                                                                                      \
    } while (0)

ZEND_METHOD(CParser_TranslationUnit, fromFile)
{
    char *filename;
    size_t filename_len;
    zval *args = NULL;
    zend_long flags = 0;

    ZEND_PARSE_PARAMETERS_START(1, 3)
    Z_PARAM_PATH(filename, filename_len)
    Z_PARAM_OPTIONAL
    Z_PARAM_ARRAY(args)
    Z_PARAM_LONG(flags)
    ZEND_PARSE_PARAMETERS_END();

    // Extract args into C array of const char*
    std::vector<const char *> cargs;
    if (args)
    {
        HashTable *arr_hash = Z_ARRVAL_P(args);
        zval *val;
        ZEND_HASH_FOREACH_VAL(arr_hash, val)
        {
            if (Z_TYPE_P(val) == IS_STRING)
            {
                cargs.push_back(Z_STRVAL_P(val));
            }
        }
        ZEND_HASH_FOREACH_END();
    }

    CXIndex idx = clang_createIndex(0, 0);
    unsigned int parse_flags = (unsigned int)flags;
    parse_flags |= CXTranslationUnit_DetailedPreprocessingRecord;

    CXTranslationUnit tu = clang_parseTranslationUnit(
        idx,
        filename,
        cargs.empty() ? nullptr : cargs.data(),
        (int)cargs.size(),
        nullptr, 0,
        parse_flags);

    if (!tu)
    {
        if (idx)
        {
            clang_disposeIndex(idx);
        }
        zend_throw_exception(zend_ce_exception, "Unable to parse translation unit", 0);
        return;
    }

    // Allocate PHP object
    object_init_ex(return_value, cparser_translationunit_ce);
    auto *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(return_value));
    intern->native.index = idx;
    intern->native.tu = tu;
    ZVAL_UNDEF(&intern->native.includes_cache);
    intern->native.includes_cache_ready = 0;
}

ZEND_METHOD(CParser_TranslationUnit, cursors)
{
    zend_long kind = 0;
    ZEND_PARSE_PARAMETERS_START(0, 1)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(kind)
    ZEND_PARSE_PARAMETERS_END();

    RETURN_CURSOR_IT_WITH_FILTER(kind);
}

ZEND_METHOD(CParser_TranslationUnit, classes)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));
    object_init_ex(return_value, cparser_cursoriterator_ce);
    auto *it_intern = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value));
    it_intern->native.clearOwner();
    it_intern->native = NativeCXCursorIterator(
        clang_getTranslationUnitCursor(intern->native.tu),
        (int)CXCursor_ClassDecl,
        false,
        false,
        (int)CXCursor_StructDecl);
    ZVAL_COPY(&it_intern->native.owner, getThis());
}

ZEND_METHOD(CParser_TranslationUnit, enums)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));
    object_init_ex(return_value, cparser_cursoriterator_ce);
    auto *it_intern = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value));
    it_intern->native.clearOwner();
    it_intern->native = NativeCXCursorIterator(
        clang_getTranslationUnitCursor(intern->native.tu),
        (int)CXCursor_EnumDecl,
        false,
        false);
    ZVAL_COPY(&it_intern->native.owner, getThis());
}

ZEND_METHOD(CParser_TranslationUnit, diagnostics)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));

    object_init_ex(return_value, cparser_diagnosticiterator_ce);
    auto *it = php_cparser_fetch<NativeDiagnosticIterator>(Z_OBJ_P(return_value));
    it->native.tu = intern->native.tu;
    it->native.count = intern->native.tu ? clang_getNumDiagnostics(intern->native.tu) : 0;
    it->native.index = 0;
    ZVAL_COPY(&it->native.owner, getThis());
}

ZEND_METHOD(CParser_TranslationUnit, aliases)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));
    object_init_ex(return_value, cparser_cursoriterator_ce);
    auto *it_intern = php_cparser_fetch<NativeCXCursorIterator>(Z_OBJ_P(return_value));
    it_intern->native.clearOwner();
    it_intern->native = NativeCXCursorIterator(
        clang_getTranslationUnitCursor(intern->native.tu),
        (int)CXCursor_TypeAliasDecl,
        true,
        true,
        (int)CXCursor_TypedefDecl);
    ZVAL_COPY(&it_intern->native.owner, getThis());
}

ZEND_METHOD(CParser_TranslationUnit, includes)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_tu *intern = php_cparser_fetch<cparser_native_translation_unit>(Z_OBJ_P(getThis()));

    if (intern->native.includes_cache_ready && !Z_ISUNDEF(intern->native.includes_cache))
    {
        RETURN_ZVAL(&intern->native.includes_cache, 1, 0);
    }

    array_init(&intern->native.includes_cache);
    if (intern->native.tu)
    {
        CXCursor root = clang_getTranslationUnitCursor(intern->native.tu);
        clang_visitChildren(root, cparser_collect_inclusion_directives, &intern->native.includes_cache);
    }
    intern->native.includes_cache_ready = 1;

    RETURN_ZVAL(&intern->native.includes_cache, 1, 0);
}

ZEND_METHOD(CParser_Inclusion, getSourceFile)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_inclusion *intern = php_cparser_fetch<cparser_native_inclusion>(Z_OBJ_P(getThis()));
    if (!intern->native.source_file)
    {
        RETURN_NULL();
    }
    RETURN_STR_COPY(intern->native.source_file);
}

ZEND_METHOD(CParser_Inclusion, getIncludedFile)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_inclusion *intern = php_cparser_fetch<cparser_native_inclusion>(Z_OBJ_P(getThis()));
    if (!intern->native.included_file)
    {
        RETURN_NULL();
    }
    RETURN_STR_COPY(intern->native.included_file);
}

ZEND_METHOD(CParser_Inclusion, getSpelling)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_inclusion *intern = php_cparser_fetch<cparser_native_inclusion>(Z_OBJ_P(getThis()));
    if (!intern->native.spelling)
    {
        RETURN_EMPTY_STRING();
    }
    RETURN_STR_COPY(intern->native.spelling);
}

ZEND_METHOD(CParser_Inclusion, isAngled)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_inclusion *intern = php_cparser_fetch<cparser_native_inclusion>(Z_OBJ_P(getThis()));
    RETURN_BOOL(intern->native.angled);
}

ZEND_METHOD(CParser_Inclusion, getLine)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_inclusion *intern = php_cparser_fetch<cparser_native_inclusion>(Z_OBJ_P(getThis()));
    RETURN_LONG(intern->native.line);
}

ZEND_METHOD(CParser_Inclusion, getColumn)
{
    ZEND_PARSE_PARAMETERS_NONE();
    cparser_inclusion *intern = php_cparser_fetch<cparser_native_inclusion>(Z_OBJ_P(getThis()));
    RETURN_LONG(intern->native.column);
}
