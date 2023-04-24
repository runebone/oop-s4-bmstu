#ifndef __RB_TREE_IMPL__
#define __RB_TREE_IMPL__

#include "../rb_tree.h"

template<typename T>
using NodePtr = typename RedBlackTree<T>::NodePtr;

template<typename T>
using WeakNodePtr = typename RedBlackTree<T>::WeakNodePtr;

template<typename T>
using Iterator = typename RedBlackTree<T>::Iterator;

/* RedBlackTree() = default; */
/* explicit RedBlackTree(const RedBlackTree& other); */
template<typename T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree& other)
{
}

/* RedBlackTree(RedBlackTree&& other) noexcept; */
template<typename T>
RedBlackTree<T>::RedBlackTree(RedBlackTree&& other) noexcept
{
}

/* virtual ~RedBlackTree(); */

/* bool insert(const T& value); */
template<typename T>
bool RedBlackTree<T>::insert(const T& value)
{
}

/* bool remove(const T& key); */
template<typename T>
bool RedBlackTree<T>::remove(const T& key)
{
}

/* bool clear(); */
template<typename T>
bool RedBlackTree<T>::clear()
{
}

/* bool is_empty() const; */
template<typename T>
bool RedBlackTree<T>::is_empty() const
{
}

/* bool contains(const T& value) const; */
template<typename T>
bool RedBlackTree<T>::contains(const T& value) const
{
}

/* void dbg_print() const; */
template<typename T>
void RedBlackTree<T>::dbg_print() const
{
}

/* size_t size() const; */
template<typename T>
size_t RedBlackTree<T>::size() const
{
}

/* Iterator begin() const; */
template<typename T>
Iterator<T> RedBlackTree<T>::begin() const
{
}

/* Iterator end() const; */
template<typename T>
Iterator<T> RedBlackTree<T>::end() const
{
}

/* Iterator find(const T& value) const; */
template<typename T>
Iterator<T> RedBlackTree<T>::find(const T& value) const
{
}

/* RedBlackTree& operator=(const RedBlackTree& other); */
template<typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree<T>& other)
{
}

/* RedBlackTree& operator=(RedBlackTree&& other); */
template<typename T>
RedBlackTree<T>& RedBlackTree<T>::operator=(RedBlackTree<T>&& other)
{
}

/* bool operator==(const RedBlackTree& other) const; */
template<typename T>
bool RedBlackTree<T>::operator==(const RedBlackTree& other) const
{
}

/* protected: */
/* void rotate_right(NodePtr node); */
template<typename T>
void RedBlackTree<T>::rotate_right(NodePtr node)
{
}

/* void rotate_left(NodePtr node); */
template<typename T>
void RedBlackTree<T>::rotate_left(NodePtr node)
{
}

/* void fixup_insert(NodePtr node); */
template<typename T>
void RedBlackTree<T>::fixup_insert(NodePtr node)
{
}

/* void fixup_remove(NodePtr node); */
template<typename T>
void RedBlackTree<T>::fixup_remove(NodePtr node)
{
}

/* bool is_red(NodePtr node) const; */
template<typename T>
bool RedBlackTree<T>::is_red(NodePtr node) const
{
}

/* void change_color(NodePtr node); */
template<typename T>
void RedBlackTree<T>::change_color(NodePtr node)
{
}

/* NodePtr search(const T& key) const; */
template<typename T>
NodePtr<T> RedBlackTree<T>::search(const T& key) const
{
}

/* bool validate() const; */
template<typename T>
bool RedBlackTree<T>::validate() const
{
}

#endif // __RB_TREE_IMPL__
