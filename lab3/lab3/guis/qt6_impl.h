#ifndef __QT6_GUI_IMPL_H__
#define __QT6_GUI_IMPL_H__

#include "gui_impl.h"

#include <QtWidgets/QApplication>

#include "qt6_ui_impl/mainwindow.h"

class Qt6GUI : public GUI
{
public:
    int run()
    {
        int argc = 1;
        char *argv = "lab3";

        QApplication app(argc, &argv);

        MainWindow window;

        window.show();

        return app.exec();
    }
};

#endif // __QT6_GUI_IMPL_H__
