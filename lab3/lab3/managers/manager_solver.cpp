#include "manager_solver.h"

#include "draw_manager_creator.h"
#include "load_manager_creator.h"
#include "scene_manager_creator.h"
#include "transform_manager_creator.h"
#include "camera_manager_creator.h"

std::shared_ptr<DrawManager> ManagerSolver::get_draw_manager()
{
    static auto manager = DrawManagerCreator().get();

    return manager;
}

std::shared_ptr<LoadManager> ManagerSolver::get_load_manager()
{
    static auto manager = LoadManagerCreator().get();

    return manager;
}

std::shared_ptr<SceneManager> ManagerSolver::get_scene_manager()
{
    static auto manager = SceneManagerCreator().get();

    return manager;
}

std::shared_ptr<TransformManager> ManagerSolver::get_transform_manager()
{
    static auto manager = TransformManagerCreator().get();

    return manager;
}

std::shared_ptr<CameraManager> ManagerSolver::get_camera_manager()
{
    static auto manager = CameraManagerCreator().get();

    return manager;
}

