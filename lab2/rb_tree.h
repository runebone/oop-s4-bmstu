#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include <memory>
#include <iterator>
#include <compare>

#include "base_tree.h"
#include "concepts.h"

template<ValidNodeData T>
class RedBlackTree : public BaseTree
{
private:
#pragma region Node
    struct Node
    {
        T key;

        std::weak_ptr<Node> parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node() = default;
        explicit Node(T value) : key(value) {}

        bool is_red() { return m_is_red; }

        void make_red() { m_is_red = true; }
        void make_black() { m_is_red = false; }

        void kill_children()
        {
            if (left) { left->kill_children(); left.reset(); }
            if (right) { right->kill_children(); right.reset(); }
        }

    private:
        bool m_is_red = false;
    };

public:
    using NodePtr = std::shared_ptr<Node>;
    using WeakNodePtr = std::weak_ptr<Node>;
#pragma endregion

public:
#pragma region Iterator
    class Iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;

        explicit Iterator(NodePtr node) : m_current(node) {}
        explicit Iterator(const Iterator& other);
        Iterator(Iterator&& other) noexcept;

        const T&  operator*() const;
        const T*  operator->() const;
        Iterator& operator++();
        Iterator  operator++(int);
        Iterator& operator--();
        Iterator  operator--(int);
        bool      operator!=(const Iterator& other) const;
        bool      operator==(const Iterator& other) const;

        Iterator  operator+(const difference_type& n) const;
        Iterator  operator-(const difference_type& n) const;
        Iterator& operator+=(const difference_type& n);
        Iterator& operator-=(const difference_type& n);
        const T&  operator[](const difference_type& n) const;

        difference_type operator-(const Iterator& other) const;
        auto      operator<=>(const Iterator& other) const;

        explicit  operator bool() const;

    protected:
        NodePtr next(NodePtr node) const;
        NodePtr prev(NodePtr node) const;

    private:
        WeakNodePtr m_current;
        /* size_t m_index = 0; */
    };
#pragma endregion

public:
    using value_type = typename Iterator::value_type;
    using size_type = size_t;
    using iterator = Iterator;
    using const_iterator = const Iterator;

    RedBlackTree() = default;
    RedBlackTree(const T& data) { m_root = std::make_shared<Node>(data); }
    explicit RedBlackTree(const RedBlackTree& other);
    RedBlackTree(RedBlackTree&& other) noexcept;
    RedBlackTree(std::initializer_list<T> l);

    // Construct tree based on tree of another type
    template<typename U>
    requires IsConvertible<U, T>
    explicit RedBlackTree(const RedBlackTree<U>& other);

    // Construct tree based on other container of another type
    template<IsContainer C>
    requires IsConvertible<typename C::value_type, T>
    explicit RedBlackTree(const C& container);

    ~RedBlackTree() noexcept = default;

    bool insert(const T& value);
    bool remove(const T& key);
    bool search(const T& key) const;

    bool clear();
    bool is_empty() const;

    size_t size() const;

    void dbg_print() const;

    // TODO:
    /* [[nodiscard]] RedBlackTree get_union(const RedBlackTree &other) const; */
    /* [[nodiscard]] RedBlackTree get_difference(const RedBlackTree &other) const; */
    /* [[nodiscard]] RedBlackTree get_intersection(const RedBlackTree &other) const; */
    // balance

    Iterator begin() const;
    Iterator end() const;

    RedBlackTree& operator=(const RedBlackTree& other);
    RedBlackTree& operator=(RedBlackTree&& other);

    bool operator==(const RedBlackTree& other) const;

protected:
    void insert_fixup(NodePtr node);
    void remove_fixup(NodePtr node);

    void rotate_left(NodePtr node);
    void rotate_right(NodePtr node);

    NodePtr minimum(NodePtr root) const;

private:
    NodePtr m_root;
};

#include "rb_tree_errors.h"
#include "impl/rb_tree.hpp"

#endif // __RB_TREE_H__
