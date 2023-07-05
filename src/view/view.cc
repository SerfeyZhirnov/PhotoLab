#include "view.h"

#include "ui_mainwindow.h"

View::View(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);
}

View::~View() { delete m_ui; }

void View::on_act_open_triggered() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Choose file", QDir::currentPath(), "BMP (*.bmp)");

  bool status = m_controller.SetOriginal(filename);
  if (status) {
    m_ui->statusbar->showMessage("Image uploaded successfully");
  } else {
    m_ui->statusbar->showMessage("Image uploaded unsuccessfully");
  }
  QTimer::singleShot(m_display_time, m_ui->statusbar,
                     [this]() { m_ui->statusbar->clearMessage(); });
}