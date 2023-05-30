#ifndef __BASE_MANAGER_CREATOR_H__
#define __BASE_MANAGER_CREATOR_H__

#include <memory>

#include "base_manager.h"

template <typename T>
concept ManagerType = requires {
    std::is_base_of<BaseManager, T>::value;
};

template <ManagerType T>
class BaseManagerCreator
{
public:
    using manager_type = T;

    virtual ~BaseManagerCreator() = default;

    virtual std::shared_ptr<manager_type> get() = 0;

protected:
    static std::shared_ptr<manager_type> create()
    {
        return std::make_shared<manager_type>(new manager_type);
    }
};

#endif // __BASE_MANAGER_CREATOR_H__
