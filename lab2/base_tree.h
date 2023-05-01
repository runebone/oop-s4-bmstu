#ifndef __BASE_TREE_H__
#define __BASE_TREE_H__

#include <cstddef> // For size_t

/* #include "concepts.h" */

/* template<ValidNodeData T> */
class BaseTree
{
public:
    /* virtual bool insert(const T& value) = 0; */
    /* virtual bool remove(const T& key) = 0; */
    /* virtual bool search(const T& key) const = 0; */

    virtual size_t size() const = 0;

    virtual ~BaseTree() {};
};

#endif // __BASE_TREE_H__
