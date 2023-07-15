#include "view.h"

#include "ui_mainwindow.h"

View::View(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_filters(new FiltersWindow) {
  m_ui->setupUi(this);
  connect(&m_controller, &Controller::need_color, this, &View::on_colorNeed);
  connect(this, &View::update_image, this, &View::on_imageUpdate);
  connect(m_filters, &FiltersWindow::filter_chosen, this,
          &View::on_btnGroupSent);
  connect(m_filters, &FiltersWindow::filter_chosen, this,
          [this]() { emit update_image(Image::Filtered); });
}

View::~View() {
  delete m_ui;
  delete m_filters;
}

void View::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  emit update_image(Image::Filtered);
}

void View::mousePressEvent(QMouseEvent *event) {
  QMainWindow::mousePressEvent(event);
  if (event->button() == Qt::RightButton) {
    emit update_image(Image::Original);
  }
}

void View::mouseReleaseEvent(QMouseEvent *event) {
  QMainWindow::mouseReleaseEvent(event);
  if (event->button() == Qt::RightButton) {
    emit update_image(Image::Filtered);
  }
}

void View::UpdateStatusBarMessage(QString message) {
  m_ui->status_bar->showMessage(message);
  QTimer::singleShot(m_display_time, m_ui->status_bar,
                     [this]() { m_ui->status_bar->clearMessage(); });
}

void View::on_act_open_triggered() {
  const QString filename = QFileDialog::getOpenFileName(
      this, "Choose Image", QDir::homePath(), "BMP Files (*.bmp)");

  bool status = m_controller.SetImage(filename);
  if (status) {
    const QImage &image = m_controller.GetOriginal();
    m_ui->lb_image->setMinimumHeight(image.height());
    m_ui->lb_image->setMinimumWidth(image.width());
    emit update_image(Image::Filtered);
    UpdateStatusBarMessage("Image uploaded successfully!");
  } else {
    UpdateStatusBarMessage("Image uploaded unsuccessfully!");
  }
}

void View::on_act_save_triggered() {
  const QImage &image = m_controller.GetFiltered();
  if (image.isNull()) {
    UpdateStatusBarMessage("Image was not loaded!");
  } else {
    const QString filename = QFileDialog::getSaveFileName(
        this, "Save Image", QDir::homePath(), "BMP Files (*.bmp)");

    if (filename.isEmpty()) {
      UpdateStatusBarMessage("Saving cancelled by the user!");
    } else {
      bool saved = image.save(filename, "BMP");
      if (saved) {
        UpdateStatusBarMessage("Image saved successfully to BMP!");
      } else {
        UpdateStatusBarMessage("Failed to save image as BMP!");
      }
    }
  }
}

void View::on_act_default_triggered() { m_filters->show(); }

void View::on_btnGroupSent(QString title) {
  bool status = m_controller.ApplyFilter(title);

  if (status) {
    UpdateStatusBarMessage("Filter successfully applied!");
  } else {
    UpdateStatusBarMessage("Non-existent filter selected!");
  }
}

void View::on_imageUpdate(View::Image choose) {
  QImage image = m_controller.GetFiltered();
  if (choose == Image::Original) {
    image = m_controller.GetOriginal();
  }

  if (image.isNull()) {
    UpdateStatusBarMessage("Please, upload image!");
  } else {
    QSize updated = m_ui->lb_image->size();
    QImage scaled = image.scaled(updated, Qt::KeepAspectRatio);
    m_ui->lb_image->setPixmap(QPixmap::fromImage(scaled));
  }
}

void View::on_colorNeed() {
  QColor color = QColorDialog::getColor();

  m_controller.SetColor(color);
}
