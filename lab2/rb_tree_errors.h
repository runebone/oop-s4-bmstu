#ifndef __RB_TREE_ERRORS_H__
#define __RB_TREE_ERRORS_H__

#include "base_error.h"

#pragma region ContainerErrors
class ContainerError : public BaseError
{
public:
    ContainerError(
        const char *filename,
        const char *classname,
        const char *funcname,
        const int line,
        const char *time,
        const char *info)
        : BaseError("ContainerError", filename, classname, funcname, line, time, info) {}
};

class NullNodeError : public ContainerError
{
public:
    NullNodeError(
        const char *filename,
        const char *classname,
        const char *funcname,
        const int line,
        const char *time,
        const char *info = "Null pointer passed to function."
        )
        : ContainerError(filename, classname, funcname, line, time, info) {}
};

class OrphanNodeError : public ContainerError
{
public:
    OrphanNodeError(
        const char *filename,
        const char *classname,
        const char *funcname,
        const int line,
        const char *time,
        const char *info = "Node without parent detected."
        )
        : ContainerError(filename, classname, funcname, line, time, info) {}
};
#pragma endregion

#pragma region IteratorErrors
class IteratorError : public BaseError
{
public:
    IteratorError(
        const char *filename,
        const char *classname,
        const char *funcname,
        const int line,
        const char *time,
        const char *info)
        : BaseError("IteratorError", filename, classname, funcname, line, time, info) {}
};

class NullNodeIError : public IteratorError
{
public:
    NullNodeIError(
        const char *filename,
        const char *classname,
        const char *funcname,
        const int line,
        const char *time,
        const char *info = "Null node pointer detected."
        )
        : IteratorError(filename, classname, funcname, line, time, info) {}
};
#pragma endregion

#endif // __RB_TREE_ERRORS_H__
