#ifndef __RB_TREE_IMPL__
#define __RB_TREE_IMPL__

#include "rb_tree.h"

template<ValidNodeData T>
using Node = typename RedBlackTree<T>::Node;

template<ValidNodeData T>
using NodePtr = typename RedBlackTree<T>::NodePtr;

template<ValidNodeData T>
using WeakNodePtr = typename RedBlackTree<T>::WeakNodePtr;

// Do I need them?
// {
template<ValidNodeData T>
using Iterator = typename RedBlackTree<T>::Iterator;

template<ValidNodeData T>
using difference_type = typename RedBlackTree<T>::Iterator::difference_type;
// }

template<ValidNodeData T>
typename RedBlackTree<T>::NodePtr RedBlackTree<T>::Iterator::next(NodePtr node) const
{
    if (node == nullptr)
    {
        time_t timer = time(nullptr);
        throw NullNodeError(__FILE__, "Node<T>", __func__, __LINE__, ctime(&timer));
    }

    NodePtr next_ptr = node->right;

    if (next_ptr == nullptr)
    {
        if (node->parent.lock() == nullptr)
        {
            /* time_t timer = time(nullptr); */
            /* throw ParentlessNodeError(__FILE__, "Node<T>", __func__, __LINE__, ctime(&timer)); */
            NodePtr empty_ptr = nullptr;
            return empty_ptr;
        }

        NodePtr current = node;
        NodePtr parent = node->parent.lock();

        while (parent != nullptr && current != parent->left)
        {
            current = parent;
            parent = current->parent.lock();
        }

        next_ptr = parent;
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

template<ValidNodeData T>
typename RedBlackTree<T>::NodePtr RedBlackTree<T>::Iterator::prev(NodePtr node) const
{
    if (node == nullptr)
    {
        time_t timer = time(nullptr);
        throw NullNodeError(__FILE__, "Node<T>", __func__, __LINE__, ctime(&timer));
    }

    NodePtr prev_ptr = node->left;

    if (prev_ptr == nullptr)
    {
        if (node->parent.lock() == nullptr)
        {
            /* time_t timer = time(nullptr); */
            /* throw ParentlessNodeError(__FILE__, "Node<T>", __func__, __LINE__, ctime(&timer)); */
            NodePtr empty_ptr = nullptr;
            return empty_ptr;
        }

        NodePtr current = node;
        NodePtr parent = node->parent.lock();

        while (parent != nullptr && current != parent->right)
        {
            current = parent;
            parent = current->parent.lock();
        }

        prev_ptr = parent;
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
template<ValidNodeData T>
RedBlackTree<T>::Iterator::Iterator(const Iterator& other)
    : m_current(other.m_current)
{
}

template<ValidNodeData T>
RedBlackTree<T>::Iterator::Iterator(Iterator&& other) noexcept
    : m_current(other.m_current)
{
}

template<ValidNodeData T>
const T& RedBlackTree<T>::Iterator::operator*() const
{
    return m_current.lock()->key;
}

template<ValidNodeData T>
const T* RedBlackTree<T>::Iterator::operator->() const
{
    return &(m_current.lock()->key);
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator& RedBlackTree<T>::Iterator::operator++()
{
    try
    {
        m_current = next(m_current.lock());
    }
    catch (const NullNodeError& e)
    {
        time_t timer = time(nullptr);
        throw NullNodeIError(__FILE__, "RedBlackTree<T>::Iterator", __func__, __LINE__, ctime(&timer));
    }

    return *this;
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator RedBlackTree<T>::Iterator::operator++(int)
{
    Iterator tmp(*this);
    ++(*this);

    return tmp;
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator& RedBlackTree<T>::Iterator::operator--()
{
    try
    {
        m_current = prev(m_current.lock());
    }
    catch (const NullNodeError& e)
    {
        time_t timer = time(nullptr);
        throw NullNodeIError(__FILE__, "RedBlackTree<T>::Iterator", __func__, __LINE__, ctime(&timer));
    }

    return *this;
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator RedBlackTree<T>::Iterator::operator--(int)
{
    Iterator tmp(*this);
    --(*this);

    return tmp;
}

template<ValidNodeData T>
bool RedBlackTree<T>::Iterator::operator!=(const Iterator& other) const
{
    return !(*this == other);
}

template<ValidNodeData T>
bool RedBlackTree<T>::Iterator::operator==(const Iterator& other) const
{
    return m_current.lock() == other.m_current.lock();
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator RedBlackTree<T>::Iterator::operator+(const difference_type& n) const
{
    Iterator it(*this);

    for (auto count = n; count > 0; ++it, --count);

    return it;
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator RedBlackTree<T>::Iterator::operator-(const difference_type& n) const
{
    Iterator it(*this);

    for (auto count = n; count > 0; --it, --count);

    return it;
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator& RedBlackTree<T>::Iterator::operator+=(const difference_type& n)
{
    for (auto count = n; count > 0; ++(*this), --count);

    return *this;
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator& RedBlackTree<T>::Iterator::operator-=(const difference_type& n)
{
    for (auto count = n; count > 0; --(*this), --count);

    return *this;
}

template<ValidNodeData T>
const T& RedBlackTree<T>::Iterator::operator[](const difference_type& n) const
{
    NodePtr current = m_current.lock();

    // XXX
    try
    {
        NodePtr prev_ptr = prev(current);

        while (prev_ptr != nullptr)
        {
            current = prev_ptr;
            prev_ptr = prev(current);
        }
    }
    catch (const BaseError &e)
    {
        throw e;
    }

    Iterator it(current);

    it += n;

    return *it;
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator::difference_type RedBlackTree<T>::Iterator::operator-(const Iterator& other) const
{
    return std::distance(this, &other);
}

template<ValidNodeData T>
auto RedBlackTree<T>::Iterator::operator<=>(const Iterator& other) const
{
    auto distance = other - *this;

    if (distance < 0)
        return std::strong_ordering::less;
    else if (distance > 0)
        return std::strong_ordering::greater;
    else
        return std::strong_ordering::equal;
}

template<ValidNodeData T>
RedBlackTree<T>::Iterator::operator bool() const
{
    return !(m_current.expired());
}
#pragma endregion // Iterator

#pragma region RedBlackTree
template<ValidNodeData T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree& other)
{
    for (auto elem : other)
        insert(elem);
}

template<ValidNodeData T>
RedBlackTree<T>::RedBlackTree(RedBlackTree&& other) noexcept
{
    for (auto elem : other)
        insert(elem);
}

template<ValidNodeData T>
RedBlackTree<T>::RedBlackTree(std::initializer_list<T> l)
{
    for (auto elem : l)
        insert(elem);
}

template<ValidNodeData T>
template<typename U>
requires IsConvertible<U, T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree<U>& other)
{
    for (auto elem : other)
        insert(elem);
}

template<ValidNodeData T>
template<IsContainer C>
requires IsConvertible<typename C::value_type, T>
RedBlackTree<T>::RedBlackTree(const C& container)
{
    for (auto it = container.begin(); it != container.end(); ++it)
        insert(*it);
}

template<ValidNodeData T>
template<typename Iter>
requires IsConvertible<typename Iter::value_type, T>
RedBlackTree<T>::RedBlackTree(Iter first, Iter last)
{
    for (; first != last; ++first)
        insert(*first);
}

template<ValidNodeData T>
bool RedBlackTree<T>::insert(const T& value)
{
    if (contains(value))
        return false;

    NodePtr x = m_root;
    NodePtr y = nullptr;
    NodePtr z = std::make_shared<Node>(value);

    while (x != nullptr)
    {
        y = x;

        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == nullptr)
        m_root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    z->left = nullptr;
    z->right = nullptr;
    
    z->make_red();

    /* insert_fixup(z); */

    return true;
}

template<ValidNodeData T>
bool RedBlackTree<T>::remove(const T& key)
{
    if (!contains(key))
        return false;

    NodePtr z = search(key);
    NodePtr y = z;
    NodePtr x = nullptr;

    bool y_was_red = y->is_red();

    if (z->left == nullptr)
    {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nullptr)
    {
        x = x->left;
        transplant(z, z->left);
    }
    else
    {
        y = minimum(z->right);
        y_was_red = y->is_red();
        x = y->right;
        if (y->parent.lock() == z)
        {
            x->parent = y;
        }
        else
        {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;

        if (z->is_red())
            y->make_red();
        else
            y->make_black();
    }

    if (!y_was_red)
    {
        /* remove_fixup(x); */
    }

    return true;
}

template<ValidNodeData T>
bool RedBlackTree<T>::contains(const T& key) const
{
    for (auto elem : *this)
    {
        if (elem == key)
            return true;
    }
    return false;
}

template<ValidNodeData T>
bool RedBlackTree<T>::balance()
{
    // TODO
}

template<ValidNodeData T>
bool RedBlackTree<T>::clear()
{
    if (m_root)
    {
        m_root->kill_children();
        m_root.reset();
    }

    return is_empty();
}

template<ValidNodeData T>
bool RedBlackTree<T>::is_empty() const
{
    return m_root == nullptr;
}

template<ValidNodeData T>
size_t RedBlackTree<T>::size() const
{
    size_t count = 0;

    for (auto it = this->begin(); it != this->end(); ++it, ++count);

    return count;
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator RedBlackTree<T>::begin() const
{
    return Iterator(minimum(m_root));
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator RedBlackTree<T>::end() const
{
    return Iterator(nullptr);
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator RedBlackTree<T>::find(const T& key) const
{
    if (!contains(key))
        return Iterator(nullptr);

    Iterator it = begin();

    for (; it != end(); ++it)
    {
        if (*it == key)
            return it;
    }
}

template<ValidNodeData T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree<T>& other)
{
    clear();

    for (auto elem : other)
        insert(elem);

    return *this;
}

template<ValidNodeData T>
RedBlackTree<T>& RedBlackTree<T>::operator=(RedBlackTree<T>&& other)
{
    clear();

    for (auto elem : other)
        insert(elem);

    return *this;
}

template<ValidNodeData T>
bool RedBlackTree<T>::operator==(const RedBlackTree& other) const
{
    if (size() != other.size())
        return false;

    auto this_it = begin();
    auto other_it = other.begin();

    for (; this_it != end(); ++this_it, ++other_it)
    {
        if (*this_it != *other_it)
            return false;
    }

    return true;
}

template<ValidNodeData T>
void RedBlackTree<T>::transplant(NodePtr u, NodePtr v)
{
    if (u->parent.lock() == nullptr)
        m_root = v;
    else if (u == u->parent.lock()->left)
        u->parent.lock()->left = v;
    else
        u->parent.lock()->right = v;

    v->parent = u->parent;
}

template<ValidNodeData T>
void RedBlackTree<T>::rotate_right(NodePtr node)
{
    NodePtr y = node;
    NodePtr x = y->left;

    if (x->right != nullptr)
    {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent.lock() == nullptr)
    {
        m_root = x;
    }
    else if (y == y->parent.lock()->right)
    {
        y->parent.lock()->right = y;
    }
    else
    {
        y->parent.lock()->left = y;
    }
    x->right = y;

    y->parent = x;
}

template<ValidNodeData T>
void RedBlackTree<T>::rotate_left(NodePtr node)
{
    NodePtr x = node;       // Установка y
    NodePtr y = x->right;   // Превращение левого поддерева y
                            // в правое поддерево x
    if (y->left != nullptr)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;  // Передача родителя x узлу y
    if (x->parent.lock() == nullptr)
    {
        m_root = y;
    }
    else if (x == x->parent.lock()->left)
    {
        x->parent.lock()->left = y;
    }
    else
    {
        x->parent.lock()->right = y;
    }
    y->left = x;            // Размещение x в качестве левого
                            // дочернего узла y
    x->parent = y;
}

template<ValidNodeData T>
void RedBlackTree<T>::insert_fixup(NodePtr node)
{
    NodePtr z = node;

    while (z->parent.lock()->is_red())
    {
        if (z->parent.lock() == z->parent.lock()->parent.lock()->left)
        {
            NodePtr y = z->parent.lock()->parent.lock()->right;

            if (y->is_red())
            {
                z->parent.lock()->make_black();
                y->make_black();
                z->parent.lock()->parent.lock()->make_red();
                z = z->parent.lock()->parent.lock();
            }
            else
            {
                if (z == z->parent.lock()->right)
                {
                    z = z->parent.lock();
                    rotate_left(z);
                }
                z->parent.lock()->make_black();
                z->parent.lock()->parent.lock()->make_red();
                rotate_right(z->parent.lock()->parent.lock());
            }
        }
        else
        {
            NodePtr y = z->parent.lock()->parent.lock()->left;

            if (y->is_red())
            {
                z->parent.lock()->make_black();
                y->make_black();
                z->parent.lock()->parent.lock()->make_red();
                z = z->parent.lock()->parent.lock();
            }
            else
            {
                if (z == z->parent.lock()->left)
                {
                    z = z->parent.lock();
                    rotate_right(z);
                }
                z->parent.lock()->make_black();
                z->parent.lock()->parent.lock()->make_red();
                rotate_left(z->parent.lock()->parent.lock());
            }
        }
    }

    m_root->make_black();
}

template<ValidNodeData T>
void RedBlackTree<T>::remove_fixup(NodePtr node)
{
    // TODO
}

template<ValidNodeData T>
typename RedBlackTree<T>::NodePtr RedBlackTree<T>::minimum(NodePtr root) const
{
    NodePtr current = root;

    while (current != nullptr && current->left != nullptr)
        current = current->left;

    return current;
}

template<ValidNodeData T>
typename RedBlackTree<T>::NodePtr RedBlackTree<T>::search(NodePtr node, const T& key)
{
	if (node == nullptr || node->key == key)
		return node;

	if (node->key > key)
		return search(node->left, key);

	return search(node->right, key);
}

template<ValidNodeData T>
typename RedBlackTree<T>::NodePtr RedBlackTree<T>::search(const T& key)
{
    return search(m_root, key);
}


template<ValidNodeData T>
RedBlackTree<T> RedBlackTree<T>::get_union(const RedBlackTree &other) const
{
    RedBlackTree result;

    for (auto elem : *this)
        result.insert(elem);

    for (auto elem : other)
        result.insert(elem);

    return result;
}

template<ValidNodeData T>
RedBlackTree<T> RedBlackTree<T>::get_intersection(const RedBlackTree &other) const
{
    RedBlackTree result;

    for (auto elem : *this)
        if (other.contains(elem))
            result.insert(elem);

    return result;
}

template<ValidNodeData T>
RedBlackTree<T> RedBlackTree<T>::get_difference(const RedBlackTree &other) const
{
    RedBlackTree result;

    for (auto elem : *this)
        if (!other.contains(elem))
            result.insert(elem);

    return result;
}

template<ValidNodeData T>
RedBlackTree<T> RedBlackTree<T>::get_sym_difference(const RedBlackTree &other) const
{
    RedBlackTree result;

    for (auto elem : *this)
        if (!other.contains(elem))
            result.insert(elem);

    for (auto elem : other)
        if (!this->contains(elem))
            result.insert(elem);

    return result;
}
#pragma endregion // RedBlackTree

#endif // __RB_TREE_IMPL__
