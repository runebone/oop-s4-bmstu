#pragma once

#include <iostream>
#include <string>

class Writer
{
public:
    Writer() = default;

    void write(std::string message) { std::cout << message << std::endl; };
};
