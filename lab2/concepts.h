#ifndef __CONCEPTS_H__
#define __CONCEPTS_H__

#include <concepts>
#include <type_traits>

template<typename T>
concept HasDefaultConstructor = std::is_default_constructible<T>;

#endif // __CONCEPTS_H__
