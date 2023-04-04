// Пример 09.01. Выбор шаблона (специализации) подстановкой параметров (инстанцирование).
#include <iostream>

using namespace std;

#define PRIM_5

#ifdef PRIM_1
template <typename T, bool>
struct PrintHelper
{
    static void print(const T& t) { cout << t << endl; }
};

template <typename T>
struct PrintHelper<T, true>
{
    static void print(const T& t) { cout << *t << endl; }
};

template <typename T>
void print(const T& t)
{
    /* https://en.cppreference.com/w/cpp/types/is_pointer */
    /* std::is_pointer */
    /* Checks whether T is a pointer to object or a pointer to function (but
     * not a pointer to member/member function) or a cv-qualified version
     * thereof. Provides the member constant value which is equal to true, if T
     * is an object/function pointer type. Otherwise, value is equal to false.
     * */ 
    /* inline constexpr bool is_pointer_v = is_pointer<T>::value; */
    PrintHelper<T, is_pointer_v<T>>::print(t);
}

#elif defined(PRIM_2)

/* https://en.cppreference.com/w/cpp/types/integral_constant */
/* true_type 	std::integral_constant<bool, true> */
/* false_type 	std::integral_constant<bool, false> */
/* std::integral_constant wraps a static constant of specified type. It is the
 * base class for the C++ type traits. */ 
template <typename T>
void printHelper(false_type, const T& t) { cout << t << endl; }

template <typename T>
void printHelper(true_type, const T& t) { cout << *t << endl; }

template <typename T>
void print(const T& t)
{
    /* https://en.cppreference.com/w/cpp/types/is_pointer */
    /* type 	std::integral_constant<bool, value> */
    printHelper(typename is_pointer<T>::type{}, t);
}

#elif defined(PRIM_3)

/* https://en.cppreference.com/w/cpp/language/constexpr */
/* The constexpr specifier declares that it is possible to evaluate the value
 * of the function or variable at compile time. */ 
template <typename T>
void print(const T& t)
{
    if constexpr (is_pointer_v<T>)
    {
        cout << *t << endl;
    }
    else
    {
        cout << t << endl;
    }
}

#elif defined(PRIM_4)
void print(auto& t)
{
    cout << t << endl;
}

void print(auto* t)
{
    cout << *t << endl;
}

#elif defined(PRIM_5)
void print(const auto& t)
{
    cout << t << endl;
}

/* https://en.cppreference.com/w/cpp/language/constraints */
/* A concept is a named set of requirements. The definition of a concept must
 * appear at namespace scope. */ 
template <typename T>
concept pointer = is_pointer_v<T>;

// t is a reference to any type, which satisfies the "pointer" concept
void print(const pointer auto& t)
{
    cout << *t << endl;
}

#endif

int main()
{
    double d = 1.5;

    print(d);
    print(&d);
}

/* Outputs: */
/* 1.5 */
/* 1.5 */
