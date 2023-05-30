#ifndef __CAMERA_COMMAND_H__
#define __CAMERA_COMMAND_H__

#include "base_command.h"

class CameraCommand : public BaseCommand {};

class CameraAddCommand : public CameraCommand
{
};

class CameraRemoveCommand : public CameraCommand
{
};

class CameraSetCommand : public CameraCommand
{
};

class CameraRotateCommand : public CameraCommand
{
};

class CameraScaleCommand : public CameraCommand
{
};

class CameraTranslateCommand : public CameraCommand
{
};

class CameraTransformCommand : public CameraCommand
{
};

#endif // __CAMERA_COMMAND_H__
