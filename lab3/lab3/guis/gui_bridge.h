#ifndef __GUI_BRIDGE_H__
#define __GUI_BRIDGE_H__

#include <memory>

#include "gui_impl.h"

class GUIBridge
{
public:
    void set_gui(std::unique_ptr<GUI> new_gui) { gui = std::move(new_gui); }

    int run() { return gui->run(); }

private:
    std::unique_ptr<GUI> gui;
};

#endif // __GUI_BRIDGE_H__
