#ifndef __MANAGER_SOLVER_H__
#define __MANAGER_SOLVER_H__

#include <memory>

#include "draw_manager.h"
#include "load_manager.h"
#include "scene_manager.h"
#include "transform_manager.h"
#include "camera_manager.h"

class ManagerSolver
{
public:
    ManagerSolver() = delete;
    ~ManagerSolver() = delete;
    ManagerSolver(ManagerSolver &other) = delete;

    static std::shared_ptr<DrawManager> get_draw_manager();
    static std::shared_ptr<LoadManager> get_load_manager();
    static std::shared_ptr<SceneManager> get_scene_manager();
    static std::shared_ptr<TransformManager> get_transform_manager();
    static std::shared_ptr<CameraManager> get_camera_manager();
};

#endif // __MANAGER_SOLVER_H__
