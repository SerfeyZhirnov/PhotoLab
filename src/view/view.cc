#include "view.h"
#include "./ui_mainwindow.h"

View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

View::~View()
{
    delete ui;
}

