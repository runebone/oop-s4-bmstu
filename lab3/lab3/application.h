#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include <string>
#include <memory>

#include "config.h"

#include "guis/gui_bridge.h"
#include "guis/qt6_impl.h"
// #include "guis/imgui_opengl3_glfw_impl.h"

class LabApplication
{
public:
    LabApplication() : LabApplication(YAML, "./config.yml") {}

    LabApplication(ConfigType type, std::string configPath)
    {
        m_configPath = configPath;

        BaseConfigReaderCreator *creator;

        // TODO: ConfigSolver
        if (type == YAML) {
            creator = new YAMLConfigReaderCreator();
        }
        // TODO: errors

        m_configReader = creator->createConfigReader();

        Config config;
        m_configReader->readConfig(config, m_configPath);

        m_guiFramework = std::make_unique<GUIBridge>();

        if (config.gui == QT6) {
            m_guiFramework->set_gui(std::make_unique<Qt6GUI>());
        }
        // else if (config.gui == IMGUI_OPENGL3_GLFW) {
            // m_guiFramework->set_gui(std::make_unique<ImguiOpengl3GLFWGUI>());
        // }
    }

    int run()
    {
        return m_guiFramework->run();
    }

private:
    std::string m_configPath;
    std::unique_ptr<BaseConfigReader> m_configReader;

    std::unique_ptr<GUIBridge> m_guiFramework;
};

#endif // __APPLICATION_H__
