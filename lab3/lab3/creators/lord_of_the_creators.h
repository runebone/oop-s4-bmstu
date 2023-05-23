#ifndef __LORD_OF_THE_CREATORS_H__
#define __LORD_OF_THE_CREATORS_H__

#include "base_manager_creator.h"

template <typename T>
concept ManagerCreator = requires {
    std::is_base_of<BaseManagerCreator, T>::value;
    typename T::manager_type; // Has alias for manager type
    std::is_base_of<BaseManager, T>::value;
};

template <ManagerCreator T>
class LordOfTheCreators
{
public:
    decltype(auto) create();
};

template <ManagerCreator T>
decltype(auto) LordOfTheCreators<T>::create()
{
    auto manager_creator = T();
    auto manager = manager_creator.create();

    return manager;
}

#endif // __LORD_OF_THE_CREATORS_H__
