#ifndef __RB_TREE_IMPL__
#define __RB_TREE_IMPL__

#include "../rb_tree.h"
#include "rb_tree_errors.h"

template<typename T>
using Node = typename RedBlackTree<T>::Node;

template<typename T>
using NodePtr = typename RedBlackTree<T>::NodePtr;

template<typename T>
using WeakNodePtr = typename RedBlackTree<T>::WeakNodePtr;

template<typename T>
using Iterator = typename RedBlackTree<T>::Iterator;

template<typename T>
using difference_type = typename RedBlackTree<T>::Iterator::difference_type; // XXX

template<typename T>
NodePtr<T> next(NodePtr<T> node)
{
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
NodePtr<T> prev(NodePtr<T> node)
{
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
/* explicit Iterator(const Iterator& other); */
template<typename T>
RedBlackTree<T>::Iterator::Iterator(const Iterator& other)
    : m_current(other.m_current), m_index(other.m_index)
{
}

/* Iterator(Iterator&& other) noexcept; */
template<typename T>
RedBlackTree<T>::Iterator::Iterator(Iterator&& other) noexcept
    : m_current(other.m_current), m_index(other.m_index)
{
}

/* const T&  operator*() const; */
template<typename T>
const T& RedBlackTree<T>::Iterator::operator*() const
{
    return m_current.lock()->data;
}

/* const T*  operator->() const; */
template<typename T>
const T* RedBlackTree<T>::Iterator::operator->() const
{
    return &(m_current.lock()->data);
}

/* Iterator& operator++(); */
template<typename T>
Iterator<T>& RedBlackTree<T>::Iterator::operator++()
{
    m_current = next(m_current);
    ++m_index;

    return *this;
}

/* Iterator  operator++(int); */
template<typename T>
Iterator<T> RedBlackTree<T>::Iterator::operator++(int)
{
    Iterator tmp(*this);
    ++(*this);

    return tmp;
}

/* Iterator& operator--(); */
template<typename T>
Iterator<T>& RedBlackTree<T>::Iterator::operator--()
{
    m_current = prev(m_current);
    --m_index;

    return *this;
}


/* Iterator  operator--(int); */
template<typename T>
Iterator<T> RedBlackTree<T>::Iterator::operator--(int)
{
    Iterator tmp(*this);
    --(*this);

    return tmp;
}

/* bool      operator!=(const Iterator& other) const; */
template<typename T>
bool RedBlackTree<T>::Iterator::operator!=(const Iterator& other) const
{
    return m_index != other.m_index;
}

/* bool      operator==(const Iterator& other) const; */
template<typename T>
bool RedBlackTree<T>::Iterator::operator==(const Iterator& other) const
{
    return m_index == other.m_index;
}

/* Iterator  operator+(difference_type n) const; */
template<typename T>
Iterator<T> RedBlackTree<T>::Iterator::operator+(const difference_type& n) const
{
    Iterator it(*this);

    for (auto count = n; count > 0; ++it, --count);

    return it;
}

/* Iterator  operator-(difference_type n) const; */
template<typename T>
Iterator<T> RedBlackTree<T>::Iterator::operator-(const difference_type& n) const
{
    Iterator it(*this);

    for (auto count = n; count > 0; --it, --count);

    return it;
}

/* Iterator& operator+=(difference_type n); */
template<typename T>
Iterator<T>& RedBlackTree<T>::Iterator::operator+=(const difference_type& n)
{
    for (auto count = n; count > 0; ++(*this), --count);

    return *this;
}

/* Iterator& operator-=(difference_type n); */
template<typename T>
Iterator<T>& RedBlackTree<T>::Iterator::operator-=(const difference_type& n)
{
    for (auto count = n; count > 0; --(*this), --count);

    return *this;
}

/* const T&  operator[](difference_type n) const; */
template<typename T>
const T& RedBlackTree<T>::Iterator::operator[](const difference_type& n) const
{
    Iterator it(*this);

    it = it - it.m_index + n;

    return *it; // XXX
}

/* explicit  operator bool() const; */
template<typename T>
RedBlackTree<T>::Iterator::operator bool() const
{
    return !(m_current.expired());
}
#pragma endregion // Iterator

#pragma region RedBlackTree
/* RedBlackTree() = default; */
/* explicit RedBlackTree(const RedBlackTree& other); */
template<typename T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree& other)
{
    // TODO
}

/* RedBlackTree(RedBlackTree&& other) noexcept; */
template<typename T>
RedBlackTree<T>::RedBlackTree(RedBlackTree&& other) noexcept
{
    // TODO
}

/* RedBlackTree(std::initializer_list<T> l); */
template<typename T>
RedBlackTree<T>::RedBlackTree(std::initializer_list<T> l)
{
    // TODO
}

/* virtual ~RedBlackTree(); */

/* bool insert(const T& value); */
template<typename T>
bool RedBlackTree<T>::insert(const T& value)
{
    // TODO
}

/* bool remove(const T& key); */
template<typename T>
bool RedBlackTree<T>::remove(const T& key)
{
    // TODO
}

/* bool clear(); */
template<typename T>
bool RedBlackTree<T>::clear()
{
    // TODO
}

/* bool is_empty() const; */
template<typename T>
bool RedBlackTree<T>::is_empty() const
{
    // TODO
}

/* bool contains(const T& value) const; */
template<typename T>
bool RedBlackTree<T>::contains(const T& value) const
{
    // TODO
}

/* void dbg_print() const; */
template<typename T>
void RedBlackTree<T>::dbg_print() const
{
    // TODO
}

/* size_t size() const; */
template<typename T>
size_t RedBlackTree<T>::size() const
{
    // TODO
}

/* Iterator begin() const; */
template<typename T>
Iterator<T> RedBlackTree<T>::begin() const
{
    // TODO
}

/* Iterator end() const; */
template<typename T>
Iterator<T> RedBlackTree<T>::end() const
{
    // TODO
}

/* RedBlackTree& operator=(const RedBlackTree& other); */
template<typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree<T>& other)
{
    // TODO
}

/* RedBlackTree& operator=(RedBlackTree&& other); */
template<typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(RedBlackTree<T>&& other)
{
    // TODO
}

/* bool operator==(const RedBlackTree& other) const; */
template<typename T>
bool RedBlackTree<T>::operator==(const RedBlackTree& other) const
{
    // TODO
}

/* protected: */
/* void rotate_right(NodePtr node); */
template<typename T>
void rotate_right(NodePtr<T> node)
{
    // TODO
}

/* void rotate_left(NodePtr node); */
template<typename T>
void rotate_left(NodePtr<T> node)
{
    // TODO
}

/* void fixup_insert(NodePtr node); */
template<typename T>
void fixup_insert(NodePtr<T> node)
{
    // TODO
}

/* void fixup_remove(NodePtr node); */
template<typename T>
void fixup_remove(NodePtr<T> node)
{
    // TODO
}

/* bool is_red(NodePtr node) const; */
template<typename T>
bool is_red(NodePtr<T> node)
{
    // TODO
}

/* void change_color(NodePtr node); */
template<typename T>
void change_color(NodePtr<T> node)
{
    // TODO
}

/* NodePtr search(const T& key) const; */
template<typename T>
NodePtr<T> RedBlackTree<T>::search(const T& key) const
{
    // TODO
}

/* bool validate() const; */
template<typename T>
bool validate()
{
    // TODO
}
#pragma endregion // RedBlackTree

#endif // __RB_TREE_IMPL__
