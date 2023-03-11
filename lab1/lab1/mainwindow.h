#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
};
#endif // MAINWINDOW_H
