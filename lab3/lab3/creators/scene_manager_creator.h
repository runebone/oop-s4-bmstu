#ifndef __SCENE_MANAGER_CREATOR_H__
#define __SCENE_MANAGER_CREATOR_H__

#include <memory>

#include "base_manager_creator.h"
#include "base_manager.h"
#include "scene_manager.h"

class SceneManagerCreator : public BaseManagerCreator<SceneManager>
{
public:
    std::shared_ptr<manager_type> get()
    {
        if (m_instance == nullptr)
            m_instance = BaseManagerCreator::create();

        return m_instance;
    }

private:
    std::shared_ptr<manager_type> m_instance = nullptr;
};

#endif // __SCENE_MANAGER_CREATOR_H__
