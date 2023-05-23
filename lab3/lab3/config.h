#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <memory>

enum ConfigType
{
    TXT,
    YAML,
};

struct Config
{
    std::string gui;
    std::string window;
};

class BaseConfigReader
{
public:
    virtual bool readConfig(Config &config, std::string &configPath) = 0;
};

class YAMLConfigReader : public BaseConfigReader
{
public:
    bool readConfig(Config &config, std::string &configPath)
    {
        // XXX
        config.gui = "qt6";
        config.window = "glfw";
        return true;
    }
};

class BaseConfigReaderCreator
{
public:
    virtual std::unique_ptr<BaseConfigReader> createConfigReader() = 0;
};

class YAMLConfigReaderCreator : public BaseConfigReaderCreator
{
public:
    std::unique_ptr<BaseConfigReader> createConfigReader()
    {
        std::unique_ptr<BaseConfigReader> configReader(new YAMLConfigReader());
        return configReader;
    }
};

#endif // __CONFIG_H__
