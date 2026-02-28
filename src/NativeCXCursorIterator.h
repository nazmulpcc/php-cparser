#ifndef NATIVE_CX_CURSOR_ITERATOR_H
#define NATIVE_CX_CURSOR_ITERATOR_H

#ifdef __cplusplus
#include <clang-c/Index.h>
#include <stack>
#include <vector>

class NativeCXCursorIterator
{
private:
    std::stack<CXCursor> stack;
    CXCursor current;
    CXCursor root;
    bool done;
    int filter_kind;
    int alt_filter_kind;
    bool include_root;
    bool recursive;
    zend_long index;

    bool isValidCursor(const CXCursor &cursor) const
    {
        return !clang_Cursor_isNull(cursor) &&
               clang_getCursorKind(cursor) != CXCursor_InvalidFile &&
               clang_getCursorKind(cursor) != CXCursor_NoDeclFound;
    }

    void pushChildrenSafe(CXCursor parent)
    {
        clang_visitChildren(
            parent,
            [](CXCursor c, CXCursor /*parent*/, CXClientData client_data)
            {
                std::stack<CXCursor> *stack_ptr = static_cast<std::stack<CXCursor> *>(client_data);
                if (!clang_Cursor_isNull(c))
                {
                    CXCursorKind kind = clang_getCursorKind(c);
                    if (kind != CXCursor_InvalidFile &&
                        kind != CXCursor_NoDeclFound)
                    {
                        stack_ptr->push(c);
                    }
                }
                return CXChildVisit_Continue;
            },
            &stack);
    }

    bool cursorMatchesFilter(CXCursor cursor) const
    {
        if (filter_kind < 1)
        {
            return true;
        }
        CXCursorKind kind = clang_getCursorKind(cursor);
        if (kind == (CXCursorKind)filter_kind)
        {
            return true;
        }
        if (alt_filter_kind > 0 && kind == (CXCursorKind)alt_filter_kind)
        {
            return true;
        }
        return false;
    }

public:
    zval owner;

    NativeCXCursorIterator(CXCursor root, int filter_kind = 0, bool include_root = false, bool recursive = true, int alt_filter_kind = 0)
        : root(root), done(false), filter_kind(filter_kind), alt_filter_kind(alt_filter_kind), include_root(include_root), recursive(recursive), index(0)
    {
        ZVAL_UNDEF(&owner);
        rewind();
    }

    void clearOwner()
    {
        if (!Z_ISUNDEF(owner))
        {
            zval_ptr_dtor(&owner);
            ZVAL_UNDEF(&owner);
        }
    }

    bool advanceToNextValidCursor()
    {
        while (!stack.empty())
        {
            CXCursor candidate = stack.top();
            stack.pop();

            if (!isValidCursor(candidate))
            {
                continue;
            }

            if (recursive)
            {
                // Recursive traversal: descend into descendants before filtering result set.
                pushChildrenSafe(candidate);
            }

            if (cursorMatchesFilter(candidate))
            {
                current = candidate;
                return true;
            }
        }
        done = true;
        return false;
    }

    bool next()
    {
        if (done)
        {
            return false;
        }
        return advanceToNextValidCursor();
    }

    bool valid() const
    {
        return !done;
    }

    CXCursor currentCursor() const
    {
        return current;
    }

    zend_long currentIndex() const
    {
        return index;
    }

    void rewind()
    {
        while (!stack.empty())
        {
            stack.pop();
        }
        if (include_root)
        {
            if (isValidCursor(root))
            {
                stack.push(root);
            }
        }
        else
        {
            clang_visitChildren(root, [](CXCursor c, CXCursor parent, CXClientData client_data)
                                {
                                    std::stack<CXCursor> *stack_ptr = static_cast<std::stack<CXCursor> *>(client_data);
                                    if (!clang_Cursor_isNull(c))
                                    {
                                        stack_ptr->push(c);
                                    }
                                    return CXChildVisit_Continue; }, &stack);
        }
        done = false;
        index = 0;
        // Immediately advance to the first valid matching cursor
        if (!advanceToNextValidCursor())
        {
            done = true;
        }
    }

    bool nextWithIndex()
    {
        if (done)
        {
            return false;
        }
        ++index;
        return advanceToNextValidCursor();
    }
};

#endif
#endif // NATIVE_CX_CURSOR_ITERATOR_H
