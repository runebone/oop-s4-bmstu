#ifndef __FACADE_H__
#define __FACADE_H__

#include <memory>

#include "draw_manager.h"
#include "load_manager.h"
#include "scene_manager.h"
#include "transform_manager.h"

class ControllerFacade
{
public:
    ControllerFacade();
    /* void execute(BaseCommand &command); */

private:
    std::shared_ptr<DrawManager> m_drawManager;
    std::shared_ptr<LoadManager> m_loadManager;
    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<TransformManager> m_transformManager;
};

#endif // __FACADE_H__
