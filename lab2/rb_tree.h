#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include <memory>
#include <iterator>
#include <compare>

#include "base_tree.h"
#include "concepts.h"

template<ValidNodeData T>
/* template<typename T> */
class RedBlackTree : public BaseTree
{
private:
#pragma region Node
    struct Node
    {
        T data;

        std::weak_ptr<Node> parent;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node() = default;
        explicit Node(T value) : data(value) {}

        bool is_red() { return m_is_red; }
        void change_color() { m_is_red = is_red() ? false : true; }

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

        explicit Iterator(WeakNodePtr node) : m_current(node) {}
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

        // TODO
        /* std::strong_ordering operator<=>(const Iterator& other) const; */

        explicit  operator bool() const;

    private:
        WeakNodePtr m_current;
        size_t m_index;
    };
#pragma endregion

public:
    using value_type = typename Iterator::value_type;
    using size_type = size_t;
    using iterator = Iterator;
    using const_iterator = const Iterator;

    RedBlackTree() = default;
    explicit RedBlackTree(const RedBlackTree& other);
    RedBlackTree(RedBlackTree&& other) noexcept;
    RedBlackTree(std::initializer_list<T> l);
    // TODO: через деревья другого типа конструктор сделать

    virtual ~RedBlackTree();

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

private:
    NodePtr m_root;
};

/* #include "impl/rb_tree.hpp" */

#endif // __RB_TREE_H__
