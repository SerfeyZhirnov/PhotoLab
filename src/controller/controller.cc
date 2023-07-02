#include "controller.h"
#include "./ui_mainwindow.h"

Controller::Controller(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Controller)
{
    ui->setupUi(this);
}

Controller::~Controller()
{
    delete ui;
}

