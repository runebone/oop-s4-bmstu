#ifndef __DRAW_MANAGER_CREATOR_H__
#define __DRAW_MANAGER_CREATOR_H__

#include <memory>

#include "base_manager_creator.h"
#include "base_manager.h"
#include "draw_manager.h"

class DrawManagerCreator : public BaseManagerCreator
{
public:
    using manager_type = DrawManager;

    std::shared_ptr<manager_type> create()
    {
        return BaseManagerCreator::create<manager_type>();
    }
};

#endif // __DRAW_MANAGER_CREATOR_H__
