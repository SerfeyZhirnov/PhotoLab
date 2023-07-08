#include "view.h"

#include "ui_mainwindow.h"

View::View(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_filters(new FiltersWindow) {
  m_ui->setupUi(this);
  connect(m_filters, &FiltersWindow::filter_chosen, this,
          &View::on_btnGroupSent);
}

View::~View() {
  delete m_ui;
  delete m_filters;
}

void View::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  UpdateLabelImage();
}

void View::UpdateLabelImage() {
  QSize updated = m_ui->lb_image->size();
  const QImage &image = m_controller.GetFiltered();
  if (image.isNull()) {
    UpdateStatusBarMessage("Please, upload image!");
  } else {
    QImage scaled = image.scaled(updated, Qt::KeepAspectRatio);
    m_ui->lb_image->setPixmap(QPixmap::fromImage(scaled));
  }
}

void View::UpdateStatusBarMessage(QString message) {
  m_ui->status_bar->showMessage(message);
  QTimer::singleShot(m_display_time, m_ui->status_bar,
                     [this]() { m_ui->status_bar->clearMessage(); });
}

void View::on_act_open_triggered() {
  QString filename = QFileDialog::getOpenFileName(
      this, "Choose file", QDir::currentPath(), "BMP (*.bmp)");

  bool status = m_controller.SetImage(filename);
  if (status) {
    const QImage &image = m_controller.GetOriginal();
    m_ui->lb_image->setMinimumHeight(image.height());
    m_ui->lb_image->setMinimumWidth(image.width());
    UpdateLabelImage();
    UpdateStatusBarMessage("Image uploaded successfully!");
  } else {
    UpdateStatusBarMessage("Image uploaded unsuccessfully!");
  }
}

void View::on_act_default_triggered() { m_filters->show(); }

void View::on_btnGroupSent(QString title) {
  bool status = m_controller.ApplyFilter(title);

  if (status) {
    UpdateStatusBarMessage("Filter successfully applied!");
  } else {
    UpdateStatusBarMessage("Image not loaded or non-existent filter selected!");
  }
}
