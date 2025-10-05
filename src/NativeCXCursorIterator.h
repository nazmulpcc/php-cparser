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
    bool include_root;

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
        if (filter_kind == 0)
        {
            return true;
        }
        return clang_getCursorKind(cursor) == (CXCursorKind)filter_kind;
    }

public:
    NativeCXCursorIterator(CXCursor root, int filter_kind = 0, bool include_root = false)
        : root(root), filter_kind(filter_kind), done(false), include_root(include_root)
    {
        rewind();
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

            // Always push children first (inclusive traversal)
            pushChildrenSafe(candidate);

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
        // Immediately advance to the first valid matching cursor
        if (!advanceToNextValidCursor())
        {
            done = true;
        }
    }
};

#endif
#endif // NATIVE_CX_CURSOR_ITERATOR_H