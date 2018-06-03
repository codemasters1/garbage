#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    canvas = new Canvas(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    canvas->resize(event->size());
}

void MainWindow::wheelEvent(QWheelEvent *event)
{

}
