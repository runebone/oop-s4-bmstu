#ifndef __BASE_MANAGER_CREATOR_H__
#define __BASE_MANAGER_CREATOR_H__

#include <memory>

#include "base_manager.h"

template <typename T>
concept ManagerType = requires {
    std::is_base_of<BaseManager, T>::value;
};

class BaseManagerCreator
{
public:
    virtual ~BaseManagerCreator() = 0;

    template <ManagerType T>
    std::shared_ptr<T> create()
    {
        return std::make_shared<T>(new T);
    }
};

#endif // __BASE_MANAGER_CREATOR_H__
