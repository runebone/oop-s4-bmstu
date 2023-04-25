#ifndef __RB_TREE_IMPL__
#define __RB_TREE_IMPL__

#include "../rb_tree.h"
#include "rb_tree_errors.h"

// #include "concepts.h"

template<typename T>
// requires ValidNodeData<T>
using Node = typename RedBlackTree<T>::Node;

template<typename T>
// requires ValidNodeData<T>
using NodePtr = typename RedBlackTree<T>::NodePtr;

template<typename T>
// requires ValidNodeData<T>
using WeakNodePtr = typename RedBlackTree<T>::WeakNodePtr;

template<typename T>
// requires ValidNodeData<T>
using Iterator = typename RedBlackTree<T>::Iterator;

template<typename T>
// requires ValidNodeData<T>
using difference_type = typename RedBlackTree<T>::Iterator::difference_type; // XXX

template<typename T>
// requires ValidNodeData<T>
NodePtr<T> next(NodePtr<T> node)
{
    if (node == nullptr)
    {
        time_t timer = time(nullptr);
        throw NullNodeError(__FILE__, "Node<T>", __func__, __LINE__, ctime(&timer));
    }

    NodePtr<T> next_ptr = node->right;

    if (next_ptr == nullptr)
    {
        if (node->parent == nullptr)
        {
            time_t timer = time(nullptr);
            throw OrphanNodeError(__FILE__, "Node<T>", __func__, __LINE__, ctime(&timer));
        }

        NodePtr<T> parent = node->parent;

        if (next_ptr == parent->left)
        {
            next_ptr = parent;
        }
        else if (next_ptr == parent->right)
        {
            next_ptr = parent->parent;
        }
    }
    else
    {
        while (next_ptr->left != nullptr)
        {
            next_ptr = next_ptr->left;
        }
    }

    return next_ptr;
}

template<typename T>
// requires ValidNodeData<T>
NodePtr<T> prev(NodePtr<T> node)
{
    if (node == nullptr)
    {
        time_t timer = time(nullptr);
        throw NullNodeError(__FILE__, "Node<T>", __func__, __LINE__, ctime(&timer));
    }

    NodePtr<T> prev_ptr = node->left;

    if (prev_ptr == nullptr)
    {
        if (node->parent == nullptr)
        {
            time_t timer = time(nullptr);
            throw OrphanNodeError(__FILE__, "Node<T>", __func__, __LINE__, ctime(&timer));
        }

        NodePtr<T> parent = node->parent;

        if (prev_ptr == parent->left)
        {
            prev_ptr = parent->parent;
        }
        else if (prev_ptr == parent->right)
        {
            prev_ptr = parent;
        }
    }
    else
    {
        while (prev_ptr->right != nullptr)
        {
            prev_ptr = prev_ptr->right;
        }
    }

    return prev_ptr;
}

#pragma region Iterator
template<typename T>
// requires ValidNodeData<T>
RedBlackTree<T>::Iterator::Iterator(const Iterator& other)
    : m_current(other.m_current), m_index(other.m_index)
{
}

template<typename T>
// requires ValidNodeData<T>
RedBlackTree<T>::Iterator::Iterator(Iterator&& other) noexcept
    : m_current(other.m_current), m_index(other.m_index)
{
}

template<typename T>
// requires ValidNodeData<T>
const T& RedBlackTree<T>::Iterator::operator*() const
{
    return m_current.lock()->data;
}

template<typename T>
// requires ValidNodeData<T>
const T* RedBlackTree<T>::Iterator::operator->() const
{
    return &(m_current.lock()->data);
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T>& RedBlackTree<T>::Iterator::operator++()
{
    try
    {
        m_current = next(m_current);
    }
    catch (const NullNodeError& e)
    {
        time_t timer = time(nullptr);
        throw NullNodeIError(__FILE__, "RedBlackTree<T>::Iterator", __func__, __LINE__, ctime(&timer));
    }

    ++m_index;

    return *this;
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T> RedBlackTree<T>::Iterator::operator++(int)
{
    Iterator tmp(*this);
    ++(*this);

    return tmp;
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T>& RedBlackTree<T>::Iterator::operator--()
{
    try
    {
        m_current = prev(m_current);
    }
    catch (const NullNodeError& e)
    {
        time_t timer = time(nullptr);
        throw NullNodeIError(__FILE__, "RedBlackTree<T>::Iterator", __func__, __LINE__, ctime(&timer));
    }

    --m_index;

    return *this;
}


template<typename T>
// requires ValidNodeData<T>
Iterator<T> RedBlackTree<T>::Iterator::operator--(int)
{
    Iterator tmp(*this);
    --(*this);

    return tmp;
}

template<typename T>
// requires ValidNodeData<T>
bool RedBlackTree<T>::Iterator::operator!=(const Iterator& other) const
{
    return m_index != other.m_index;
}

template<typename T>
// requires ValidNodeData<T>
bool RedBlackTree<T>::Iterator::operator==(const Iterator& other) const
{
    return m_index == other.m_index;
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T> RedBlackTree<T>::Iterator::operator+(const difference_type& n) const
{
    Iterator it(*this);

    for (auto count = n; count > 0; ++it, --count);

    return it;
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T> RedBlackTree<T>::Iterator::operator-(const difference_type& n) const
{
    Iterator it(*this);

    for (auto count = n; count > 0; --it, --count);

    return it;
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T>& RedBlackTree<T>::Iterator::operator+=(const difference_type& n)
{
    for (auto count = n; count > 0; ++(*this), --count);

    return *this;
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T>& RedBlackTree<T>::Iterator::operator-=(const difference_type& n)
{
    for (auto count = n; count > 0; --(*this), --count);

    return *this;
}

template<typename T>
// requires ValidNodeData<T>
const T& RedBlackTree<T>::Iterator::operator[](const difference_type& n) const
{
    Iterator it(*this);

    it = it - it.m_index + n;

    return *it; // XXX
}

template<typename T>
// requires ValidNodeData<T>
RedBlackTree<T>::Iterator::operator bool() const
{
    return !(m_current.expired());
}
#pragma endregion // Iterator

#pragma region RedBlackTree
template<typename T>
// requires ValidNodeData<T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree& other)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
RedBlackTree<T>::RedBlackTree(RedBlackTree&& other) noexcept
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
RedBlackTree<T>::RedBlackTree(std::initializer_list<T> l)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
bool RedBlackTree<T>::insert(const T& value)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
bool RedBlackTree<T>::remove(const T& key)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
bool RedBlackTree<T>::clear()
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
bool RedBlackTree<T>::is_empty() const
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
bool RedBlackTree<T>::contains(const T& value) const
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
void RedBlackTree<T>::dbg_print() const
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
size_t RedBlackTree<T>::size() const
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T> RedBlackTree<T>::begin() const
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
Iterator<T> RedBlackTree<T>::end() const
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree<T>& other)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
RedBlackTree<T>& RedBlackTree<T>::operator=(RedBlackTree<T>&& other)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
bool RedBlackTree<T>::operator==(const RedBlackTree& other) const
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
void rotate_right(NodePtr<T> node)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
void rotate_left(NodePtr<T> node)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
void fixup_insert(NodePtr<T> node)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
void fixup_remove(NodePtr<T> node)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
bool is_red(NodePtr<T> node)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
void change_color(NodePtr<T> node)
{
    // TODO
}

template<typename T>
// requires ValidNodeData<T>
NodePtr<T> RedBlackTree<T>::search(const T& key) const
{
    // TODO
}
#pragma endregion // RedBlackTree

#endif // __RB_TREE_IMPL__
