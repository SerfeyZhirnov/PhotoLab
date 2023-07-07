#include "filterswindow.h"

#include "ui_filterswindow.h"

FiltersWindow::FiltersWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::FiltersWindow) {
  ui->setupUi(this);
}

FiltersWindow::~FiltersWindow() { delete ui; }
