#include "controller_facade.h"

#include "lord_of_the_creators.h"

#include "draw_manager_creator.h"
#include "load_manager_creator.h"
#include "scene_manager_creator.h"
#include "transform_manager_creator.h"

inline ControllerFacade::ControllerFacade()
{
    auto lord = LordOfTheCreators();

    m_drawManager = lord.create<DrawManagerCreator>();
    /* m_loadManager = lord.create<LoadManagerCreator>(); */
    /* m_sceneManager = lord.create<SceneManagerCreator>(); */
    /* m_transformManager = lord.create<TransformManagerCreator>(); */
}
