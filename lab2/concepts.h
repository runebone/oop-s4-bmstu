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

template<typename From, typename To>
concept IsConvertible = std::is_convertible<From, To>();

#include <ranges>

template<typename T>
concept IsContainer = requires {
    std::ranges::sized_range<T>; // i.e. T has begin(), end() and size() methods
    T::value_type;
};

#endif // __CONCEPTS_H__
