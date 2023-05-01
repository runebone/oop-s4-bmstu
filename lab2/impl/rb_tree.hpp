#ifndef __RB_TREE_IMPL__
#define __RB_TREE_IMPL__

/* #include "../rb_tree.h" */
/* #include "../rb_tree_errors.h" */

/* #include "../concepts.h" */

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

template<ValidNodeData T>
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
template<ValidNodeData T>
RedBlackTree<T>::Iterator::Iterator(const Iterator& other)
    : m_current(other.m_current), m_index(other.m_index)
{
}

template<ValidNodeData T>
RedBlackTree<T>::Iterator::Iterator(Iterator&& other) noexcept
    : m_current(other.m_current), m_index(other.m_index)
{
}

template<ValidNodeData T>
const T& RedBlackTree<T>::Iterator::operator*() const
{
    return m_current.lock()->data;
}

template<ValidNodeData T>
const T* RedBlackTree<T>::Iterator::operator->() const
{
    return &(m_current.lock()->data);
}

template<ValidNodeData T>
typename RedBlackTree<T>::Iterator& RedBlackTree<T>::Iterator::operator++()
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
    return m_index != other.m_index;
}

template<ValidNodeData T>
bool RedBlackTree<T>::Iterator::operator==(const Iterator& other) const
{
    return m_index == other.m_index;
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
    Iterator it(*this);

    it = it - it.m_index + n;

    return *it;
}

template<ValidNodeData T>
RedBlackTree<T>::Iterator::difference_type RedBlackTree<T>::Iterator::operator-(const Iterator& other) const
{
    return std::distance(this, other);
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
    // TODO
}

template<ValidNodeData T>
RedBlackTree<T>::RedBlackTree(RedBlackTree&& other) noexcept
{
    // TODO
}

template<ValidNodeData T>
RedBlackTree<T>::RedBlackTree(std::initializer_list<T> l)
{
    // TODO
}

template<ValidNodeData T>
template<typename U>
requires IsConvertible<U, T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree<U>& other)
{
    // TODO
}

template<ValidNodeData T>
template<IsContainer C>
requires IsConvertible<typename C::value_type, T>
RedBlackTree<T>::RedBlackTree(const C& container)
{
    // TODO
}

template<ValidNodeData T>
bool RedBlackTree<T>::insert(const T& value)
{
    // TODO
}

template<ValidNodeData T>
bool RedBlackTree<T>::remove(const T& key)
{
    // TODO
}

template<ValidNodeData T>
bool RedBlackTree<T>::search(const T& key) const
{
    // TODO
}

template<ValidNodeData T>
bool RedBlackTree<T>::clear()
{
    // TODO
}

template<ValidNodeData T>
bool RedBlackTree<T>::is_empty() const
{
    // TODO
}

template<ValidNodeData T>
size_t RedBlackTree<T>::size() const
{
    // TODO
}

template<ValidNodeData T>
void RedBlackTree<T>::dbg_print() const
{
    // TODO
}

template<ValidNodeData T>
RedBlackTree<T>::Iterator RedBlackTree<T>::begin() const
{
    // TODO
}

template<ValidNodeData T>
RedBlackTree<T>::Iterator RedBlackTree<T>::end() const
{
    // TODO
}

template<ValidNodeData T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree<T>& other)
{
    // TODO
}

template<ValidNodeData T>
RedBlackTree<T>& RedBlackTree<T>::operator=(RedBlackTree<T>&& other)
{
    // TODO
}

template<ValidNodeData T>
bool RedBlackTree<T>::operator==(const RedBlackTree& other) const
{
    // TODO
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
    if (y->parent == nullptr)
    {
        m_root = x;
    }
    else if (y == y->parent->right)
    {
        y->parent->right = y;
    }
    else
    {
        y->parent->left = y;
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
    if (x->parent == nullptr)
    {
        m_root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left = x;            // Размещение x в качестве левого
                            // дочернего узла y
    x->parent = y;
}

template<ValidNodeData T>
void RedBlackTree<T>::insert_fixup(NodePtr node)
{
    // TODO
}

template<ValidNodeData T>
void RedBlackTree<T>::remove_fixup(NodePtr node)
{
    // TODO
}
#pragma endregion // RedBlackTree

#endif // __RB_TREE_IMPL__
