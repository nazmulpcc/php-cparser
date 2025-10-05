extern "C"
{
#include "php.h"
#include "ext/standard/info.h"
}

#include <stack>
#include <vector>

#include "php_cparser.h"

ZEND_METHOD(CParser_ClassIterator, __construct)
{
    zval *source;
    zend_long filter_kind = -1;
    ZEND_PARSE_PARAMETERS_START(1, 2)
    Z_PARAM_OBJECT(source)
    Z_PARAM_OPTIONAL
    Z_PARAM_LONG(filter_kind)
    ZEND_PARSE_PARAMETERS_END();

    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));

    /* Store a persistent zval reference to source so it won't be GC'd */
    ZVAL_COPY(&it->source_obj, source);

    /* Initialize traversal state */
    it->done = false;
    it->filter_kind = filter_kind;
    it->stack = std::stack<CXCursor>(); // ensure empty

    /* Determine source type and push root cursor */
    if (instanceof_function(Z_OBJCE_P(source), cparser_translationunit_ce))
    {
        cparser_tu *tu_intern = php_cparser_fetch<CXTranslationUnit>(Z_OBJ_P(source));
        if (!tu_intern || !tu_intern->native)
        {
            it->done = true;
            RETURN_NULL();
        }
        CXCursor root = clang_getTranslationUnitCursor(tu_intern->native);
        it->current = root;
        it->stack.push(root);
    }
    else if (instanceof_function(Z_OBJCE_P(source), cparser_cursor_ce))
    {
        auto *cursor_intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(source));
        CXCursor root = cursor_intern->native;
        it->current = root;
        it->stack.push(root);
    }
    else
    {
        it->done = true;
        RETURN_NULL();
    }
}

ZEND_METHOD(CParser_ClassIterator, current)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));

    if (it->done || it->stack.empty())
    {
        RETURN_NULL();
    }

    /* Return a Cursor PHP object wrapping it->current */
    zval obj;
    object_init_ex(&obj, cparser_cursor_ce);

    /* set native cursor on wrapper */
    auto *cursor_intern = php_cparser_fetch<CXCursor>(Z_OBJ(obj));
    cursor_intern->native = it->current;

    RETURN_ZVAL(&obj, 1, 1);
}

ZEND_METHOD(CParser_ClassIterator, key)
{
    ZEND_PARSE_PARAMETERS_NONE();
    /* There is no meaningful numeric key in lazy DFS; return 0 for compatibility */
    RETURN_LONG(0);
}

ZEND_METHOD(CParser_ClassIterator, next)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));

    if (it->done)
    {
        RETURN_NULL();
    }

    // Advance until we find a node that matches filter_kind (or until stack is empty)
    while (!it->stack.empty())
    {
        // Pop top
        CXCursor node = it->stack.top();
        it->stack.pop();

        // Push children onto stack (push in reverse order if you want pre-order left-to-right)
        struct PushCtx
        {
            std::stack<CXCursor> *stack;
        };
        PushCtx ctx{&it->stack};

        clang_visitChildren(node, [](CXCursor c, CXCursor /*parent*/, CXClientData client_data)
                            {
                PushCtx *p = reinterpret_cast<PushCtx*>(client_data);
                // push child onto stack so it will be visited later
                p->stack->push(c);
                return CXChildVisit_Continue; }, &ctx);

        // If filter not set or node matches filter, set current and return
        if (it->filter_kind < 1 || clang_getCursorKind(node) == it->filter_kind)
        {
            it->current = node;
            RETURN_NULL();
        }

        // else continue loop to pop next node
    }

    // stack exhausted
    it->done = true;
    RETURN_NULL();
}

ZEND_METHOD(CParser_ClassIterator, rewind)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));

    // Clear existing stack
    it->stack = std::stack<CXCursor>();

    // Re-seed from source root
    if (Z_ISUNDEF(it->source_obj) || Z_TYPE(it->source_obj) != IS_OBJECT)
    {
        it->done = true;
        RETURN_NULL();
    }

    zval *source = &it->source_obj;

    if (instanceof_function(Z_OBJCE_P(source), cparser_translationunit_ce))
    {
        cparser_tu *tu_intern = php_cparser_fetch<CXTranslationUnit>(Z_OBJ_P(source));
        if (!tu_intern || !tu_intern->native)
        {
            it->done = true;
            RETURN_NULL();
        }
        CXCursor root = clang_getTranslationUnitCursor(tu_intern->native);
        it->stack.push(root);
        it->current = root;
        it->done = false;
    }
    else if (instanceof_function(Z_OBJCE_P(source), cparser_cursor_ce))
    {
        auto *cursor_intern = php_cparser_fetch<CXCursor>(Z_OBJ_P(source));
        CXCursor root = cursor_intern->native;
        it->stack.push(root);
        it->current = root;
        it->done = false;
    }
    else
    {
        it->done = true;
        RETURN_NULL();
    }

    RETURN_NULL();
}

ZEND_METHOD(CParser_ClassIterator, valid)
{
    ZEND_PARSE_PARAMETERS_NONE();
    ast_cursor_iterator *it = Z_AST_IT_P(Z_OBJ_P(getThis()));
    RETURN_BOOL(!it->done && !it->stack.empty());
}