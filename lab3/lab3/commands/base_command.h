#ifndef __BASE_COMMAND_H__
#define __BASE_COMMAND_H__

class BaseCommand
{
public:
    BaseCommand() = default;
    virtual ~BaseCommand() = default;
    virtual void exec() = 0;
};

#endif // __BASE_COMMAND_H__
