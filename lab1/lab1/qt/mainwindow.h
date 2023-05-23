#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "requests.h"

#define SCENE_W 800
#define SCENE_H 600

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_load_triggered();
    void on_action_save_triggered();
    void on_action_about_triggered();

    void on_translate_button_clicked();
    void on_scale_button_clicked();
    void on_rotate_button_clicked();

private:
    Ui::MainWindow *ui;

    void mw_setup_canvas(canvas_t &canvas);
    void mw_setup_change(change_t &change, action_t action);
};

#endif // MAINWINDOW_H
