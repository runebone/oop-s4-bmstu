#ifndef __SCENE_COMMAND_H__
#define __SCENE_COMMAND_H__

#include "base_command.h"

class SceneCommand : public BaseCommand {};

class SceneDrawCommand : public SceneCommand
{
};

class SceneClearCommand : public SceneCommand
{
};

#endif // __SCENE_COMMAND_H__
