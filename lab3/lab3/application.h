#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <string>
#include <memory>

#include "config.h"

class LabApplication
{
public:
    LabApplication() : LabApplication(YAML, "./config.yml") {}

    LabApplication(ConfigType type, std::string configPath)
    {
        m_configPath = configPath;

        BaseConfigReaderCreator *creator;

        if (type == YAML) {
            creator = new YAMLConfigReaderCreator();
        }
        // TODO: errors

        m_configReader = creator->createConfigReader();

        Config config;
        m_configReader->readConfig(config, m_configPath);

        // TODO pass config on
    }

private:
    std::string m_configPath;
    std::unique_ptr<BaseConfigReader> m_configReader;
};

#endif // __APPLICATION_H__
