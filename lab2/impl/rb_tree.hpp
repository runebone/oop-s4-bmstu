#ifndef __RB_TREE_IMPL__
#define __RB_TREE_IMPL__

#include "../rb_tree.h"

template<typename T>
using NodePtr = typename RedBlackTree<T>::NodePtr;

template<typename T>
using WeakNodePtr = typename RedBlackTree<T>::WeakNodePtr;

template<typename T>
using Iterator = typename RedBlackTree<T>::Iterator;

// XXX
template<typename T>
using difference_type = typename RedBlackTree<T>::Iterator::difference_type;

#pragma region Iterator
/* const T&  operator*() const; */
template<typename T>
const T& RedBlackTree<T>::Iterator::operator*() const
{
    // TODO
}

/* const T*  operator->() const; */
template<typename T>
const T* RedBlackTree<T>::Iterator::operator->() const
{
    // TODO
}

/* Iterator& operator++(); */
template<typename T>
Iterator<T>& RedBlackTree<T>::Iterator::operator++()
{
    // TODO
}

/* Iterator  operator++(int); */
template<typename T>
Iterator<T> RedBlackTree<T>::Iterator::operator++(int)
{
    // TODO
}

/* Iterator& operator--(); */
template<typename T>
Iterator<T>& RedBlackTree<T>::Iterator::operator--()
{
    // TODO
}


/* Iterator  operator--(int); */
template<typename T>
Iterator<T> RedBlackTree<T>::Iterator::operator--(int)
{
    // TODO
}

/* bool      operator!=(const Iterator& other) const; */
template<typename T>
bool RedBlackTree<T>::Iterator::operator!=(const Iterator& other) const
{
    // TODO
}

/* bool      operator==(const Iterator& other) const; */
template<typename T>
bool RedBlackTree<T>::Iterator::operator==(const Iterator& other) const
{
    // TODO
}

/* Iterator  operator+(difference_type n) const; */
template<typename T>
Iterator<T> RedBlackTree<T>::Iterator::operator+(difference_type n) const
{
    // XXX: can you define difference_type like that?
}

/* Iterator  operator-(difference_type n) const; */
template<typename T>
Iterator<T> RedBlackTree<T>::Iterator::operator-(difference_type n) const
{
    // TODO
}

/* Iterator& operator+=(difference_type n); */
template<typename T>
Iterator<T>& RedBlackTree<T>::Iterator::operator+=(difference_type n)
{
    // TODO
}

/* Iterator& operator-=(difference_type n); */
template<typename T>
Iterator<T>& RedBlackTree<T>::Iterator::operator-=(difference_type n)
{
    // TODO
}

/* const T&  operator[](difference_type n) const; */
template<typename T>
const T& RedBlackTree<T>::Iterator::operator[](difference_type n) const
{
    // TODO
}

/* explicit  operator bool() const; */
template<typename T>
RedBlackTree<T>::Iterator::operator bool() const
{
    // TODO
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

/* Iterator find(const T& value) const; */
template<typename T>
Iterator<T> RedBlackTree<T>::find(const T& value) const
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
void RedBlackTree<T>::rotate_right(NodePtr node)
{
    // TODO
}

/* void rotate_left(NodePtr node); */
template<typename T>
void RedBlackTree<T>::rotate_left(NodePtr node)
{
    // TODO
}

/* void fixup_insert(NodePtr node); */
template<typename T>
void RedBlackTree<T>::fixup_insert(NodePtr node)
{
    // TODO
}

/* void fixup_remove(NodePtr node); */
template<typename T>
void RedBlackTree<T>::fixup_remove(NodePtr node)
{
    // TODO
}

/* bool is_red(NodePtr node) const; */
template<typename T>
bool RedBlackTree<T>::is_red(NodePtr node) const
{
    // TODO
}

/* void change_color(NodePtr node); */
template<typename T>
void RedBlackTree<T>::change_color(NodePtr node)
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
bool RedBlackTree<T>::validate() const
{
    // TODO
}
#pragma endregion // RedBlackTree

#endif // __RB_TREE_IMPL__
