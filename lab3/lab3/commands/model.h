#ifndef __MODEL_COMMAND_H__
#define __MODEL_COMMAND_H__

#include "base_command.h"

class ModelCommand : public BaseCommand {};

class ModelLoadCommand : public ModelCommand
{
};

class ModelSaveCommand : public ModelCommand
{
};

class ModelAddCommand : public ModelCommand
{
};

class ModelRemoveCommand : public ModelCommand
{
};

class ModelRotateCommand : public ModelCommand
{
};

class ModelScaleCommand : public ModelCommand
{
};

class ModelTranslateCommand : public ModelCommand
{
};

class ModelTransformCommand : public ModelCommand
{
};

#endif // __MODEL_COMMAND_H__
