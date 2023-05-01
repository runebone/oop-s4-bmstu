#ifndef __CONCEPTS_H__
#define __CONCEPTS_H__

#include <concepts>
#include <type_traits>

template<typename T>
concept Comparable = requires(T a, T b)
{
    { a == b } -> std::same_as<bool>;
    { a != b } -> std::same_as<bool>;
    { a < b } -> std::same_as<bool>;
};

template<typename T>
concept ValidNodeData = Comparable<T> && std::is_default_constructible<T>();

#endif // __CONCEPTS_H__
