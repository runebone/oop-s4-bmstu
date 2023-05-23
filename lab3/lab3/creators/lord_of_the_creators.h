#ifndef __LORD_OF_THE_CREATORS_H__
#define __LORD_OF_THE_CREATORS_H__

#include "base_manager_creator.h"

template <typename T>
concept ManagerCreator = requires {
    std::is_base_of<BaseManagerCreator, T>::value;
    typename T::manager_type; // Has alias for manager type
    std::is_base_of<BaseManager, T>::value;
};

template <typename T>
concept GuiFrameworkCreator = requires {
    typename T::test;
};

class LordOfTheCreators
{
public:
    template <ManagerCreator T>
    decltype(auto) create();

    template <GuiFrameworkCreator T>
    decltype(auto) create();
};

template <ManagerCreator T>
decltype(auto) LordOfTheCreators::create()
{
    auto manager_creator = T();
    auto manager = manager_creator.create();

    return manager;
}

template <GuiFrameworkCreator T>
decltype(auto) LordOfTheCreators::create()
{
    auto _creator = T();
    auto _ = _creator.create();

    return _;
}

#endif // __LORD_OF_THE_CREATORS_H__
