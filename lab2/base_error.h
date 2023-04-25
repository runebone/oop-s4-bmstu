#ifndef __BASE_ERROR_H__
#define __BASE_ERROR_H__

#include <exception>
#include <string>

class BaseError : public std::exception
{
public:
    BaseError(
        const std::string &errorname,
        const std::string &filename,
        const std::string &classname,
        const std::string &funcname,
        const int line,
        const char *time,
        const std::string &info)
    {
        msg = "In file: " + filename + "\n"
              + "inside: " + classname + "\n"
              + "in function: " + funcname + "\n"
              + "at line: " + std::to_string(line) + "\n"
              + "at: " + time + "\n"
              + errorname + " occured: " + info;
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        return msg.c_str();
    }

private:
    std::string msg{};
};

#endif // __BASE_ERROR_H__
