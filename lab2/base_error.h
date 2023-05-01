#ifndef __BASE_ERROR_H__
#define __BASE_ERROR_H__

#include <exception>
#include <cstdio>

#define ERROR_MESSAGE_BUFFER_SIZE 512

class BaseError : public std::exception
{
public:
    BaseError(
        const char *errorname,
        const char *filename,
        const char *classname,
        const char *funcname,
        const int line,
        const char *time,
        const char *info)
    {
        sprintf(msg,
                "In file: %s\n"
                "inside: %s\n"
                "in function: %s\n"
                "at line: %d\n"
                "at: %s\n"
                "%s occured: %s\n",
                filename, classname, funcname, line, time, errorname, info);
    }

    virtual const char *what() const noexcept override
    {
        return msg;
    }

private:
    static const size_t size_buf = ERROR_MESSAGE_BUFFER_SIZE;
    char msg[size_buf]{};
};

#endif // __BASE_ERROR_H__
