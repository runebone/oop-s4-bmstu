#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

static void qt_show_error(err_t error_code);
static void qt_show_about();
static void qt_show_saved(filename_t filename);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, SCENE_W, SCENE_H);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

MainWindow::~MainWindow()
{
    action_t action = QUIT;

    request_t request;
    request.action = action;

    err_t error_code = process_request(request);

    if (error_code != OK)
    {
        qt_show_error(error_code);
    }

    delete ui;
}


void MainWindow::on_action_load_triggered()
{
    action_t action = LOAD;

    filename_t filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/human/University/oop/lab1", "Text File (*.txt)")
            .toLocal8Bit()
            .data();

    canvas_t canvas;
    mw_setup_canvas(canvas);

    request_t request;
    request.action = action;
    request.filename = filename;
    request.canvas = canvas;

    err_t error_code = process_request(request);

    if (error_code != OK)
    {
        qt_show_error(error_code);
    }
    else
    {
        request.action = DRAW;
        error_code = process_request(request);

        if (error_code != OK)
        {
            qt_show_error(error_code);
        }
    }
}

void MainWindow::on_action_save_triggered()
{
    action_t action = SAVE;

    filename_t filename = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/human/University/oop/lab1", "Text File (*.txt)")
            .toLocal8Bit()
            .data();

    request_t request;
    request.action = action;
    request.filename = filename;

    err_t error_code = process_request(request);

    if (error_code != OK)
    {
        qt_show_error(error_code);
    }
    else
    {
        qt_show_saved(filename);
    }
}

void MainWindow::on_action_about_triggered()
{
    qt_show_about();
}

void MainWindow::on_translate_button_clicked()
{
    action_t action = TRANSLATE;

    canvas_t canvas;
    mw_setup_canvas(canvas);

    change_t change;
    mw_setup_change(change, action);

    request_t request;
    request.action = action;
    request.canvas = canvas;
    request.change = change;

    err_t error_code = process_request(request);

    if (error_code != OK)
    {
        qt_show_error(error_code);
    }
    else
    {
        request.action = DRAW;
        error_code = process_request(request);

        if (error_code != OK)
        {
            qt_show_error(error_code);
        }
    }
}

void MainWindow::on_scale_button_clicked()
{
    action_t action = SCALE;

    canvas_t canvas;
    mw_setup_canvas(canvas);

    change_t change;
    mw_setup_change(change, action);

    request_t request;
    request.action = action;
    request.canvas = canvas;
    request.change = change;

    err_t error_code = process_request(request);

    if (error_code != OK)
    {
        qt_show_error(error_code);
    }
    else
    {
        request.action = DRAW;
        error_code = process_request(request);

        if (error_code != OK)
        {
            qt_show_error(error_code);
        }
    }
}

void MainWindow::on_rotate_button_clicked()
{
    action_t action = ROTATE;

    canvas_t canvas;
    mw_setup_canvas(canvas);

    change_t change;
    mw_setup_change(change, action);

    request_t request;
    request.action = action;
    request.canvas = canvas;
    request.change = change;

    err_t error_code = process_request(request);

    if (error_code != OK)
    {
        qt_show_error(error_code);
    }
    else
    {
        request.action = DRAW;
        error_code = process_request(request);

        if (error_code != OK)
        {
            qt_show_error(error_code);
        }
    }
}

void MainWindow::mw_setup_canvas(canvas_t &canvas)
{
    canvas.scene = ui->graphicsView->scene();
}

void MainWindow::mw_setup_change(change_t &change, action_t action)
{
    switch (action)
    {
        case TRANSLATE:
            change.x = ui->dxDoubleSpinBox->value();
            change.y = ui->dyDoubleSpinBox->value();
            change.z = ui->dzDoubleSpinBox->value();
            break;
        case SCALE:
            change.x = ui->sxDoubleSpinBox->value();
            change.y = ui->syDoubleSpinBox->value();
            change.z = ui->szDoubleSpinBox->value();
            break;
        case ROTATE:
            change.x = ui->rxDoubleSpinBox->value();
            change.y = ui->ryDoubleSpinBox->value();
            change.z = ui->rzDoubleSpinBox->value();
            break;
        default:
            qt_show_error(ERR_INVALID_ACTION_IN_MW_SETUP_CHANGE);
            break;
    }
}

static void qt_show_error(err_t error_code)
{
    QString error_text;

    switch (error_code)
    {
        // TODO
        case ERR_INVALID_ACTION_IN_MW_SETUP_CHANGE:
            error_text = QString("Invalid action in MainWindow setup_change function.");
            break;
        case ERR_UNKNOWN:
            error_text = QString("Unknown error.");
            break;
        default:
            error_text = QString("Error description is not implemented in qt_show_error func (TODO).");
            break;
    }

    QMessageBox::critical(nullptr, "Error", error_text);
}

static void qt_show_about()
{
    QMessageBox::information(nullptr, "About", "Runov Konstantin IU7-44B");
}

static void qt_show_saved(filename_t filename)
{
    QString qstr_filename = QString::fromUtf8(filename);
    QString info = QString("Figure has been saved to: %1").arg(qstr_filename);
    QMessageBox::information(nullptr, "Saved", info);
}
