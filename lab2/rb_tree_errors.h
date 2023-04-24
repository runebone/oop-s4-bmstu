#ifndef __RB_TREE_ERRORS_H__
#define __RB_TREE_ERRORS_H__

#include "base_error.h"

#pragma region ContainerErrors
class ContainerError : public BaseError
{
public:
    explicit ContainerError(
        const std::string &filename,
        const std::string &classname,
        const std::string &funcname,
        const int line,
        const char *time,
        const std::string &info)
        : BaseError("ContainerError", filename, classname, funcname, line, time, info) {}
};

// example error; will be removed
class SomeError : public ContainerError
{
public:
    explicit SomeError(
        const std::string &filename,
        const std::string &classname,
        const std::string &funcname,
        const int line,
        const char *time,
        const std::string &info = "Description"
        )
        : ContainerError(filename, classname, funcname, line, time, info) {}
};
#pragma endregion

#pragma region IteratorErrors
class IteratorError : public BaseError
{
public:
    explicit IteratorError(
        const std::string &filename,
        const std::string &classname,
        const std::string &funcname,
        const int line,
        const char *time,
        const std::string &info)
        : BaseError("IteratorError", filename, classname, funcname, line, time, info) {}
};
#pragma endregion

#endif // __RB_TREE_ERRORS_H__
