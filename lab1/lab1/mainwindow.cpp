#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_load_triggered()
{
}

void MainWindow::on_action_save_triggered()
{
}

void MainWindow::on_action_about_triggered()
{
}

void MainWindow::on_translate_button_clicked()
{
}

void MainWindow::on_scale_button_clicked()
{
}

void MainWindow::on_rotate_button_clicked()
{
}
