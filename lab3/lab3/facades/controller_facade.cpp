#include "facade.h"

#include "lord_of_the_creators.h"

#include "draw_manager_creator.h"
#include "load_manager_creator.h"
#include "scene_manager_creator.h"
#include "transform_manager_creator.h"
#include "camera_manager_creator.h"

inline Facade::Facade()
{
    auto lord = LordOfTheCreators();

    /* m_drawManager = lord.create<DrawManagerCreator>(); */
    /* m_loadManager = lord.create<LoadManagerCreator>(); */
    /* m_sceneManager = lord.create<SceneManagerCreator>(); */
    /* m_transformManager = lord.create<TransformManagerCreator>(); */
    /* m_cameraManager = lord.create<CameraManagerCreator>(); */
}
