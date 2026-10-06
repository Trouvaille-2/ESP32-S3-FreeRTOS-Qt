#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("直流伺服双环测控系统");
    resize(1200,750);
}

MainWindow::~MainWindow()
{
    delete ui;
}
