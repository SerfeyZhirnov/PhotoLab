#include "view.h"

#include "ui_mainwindow.h"

View::View(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);
  connect(&m_controller, &Controller::need_color, this, &View::on_colorNeed);
  connect(this, &View::update_image, this, &View::on_imageUpdate);
  connect(this, &View::filter_chosen, this, &View::on_btnGroupSent);
  connect(this, &View::filter_chosen, this,
          [this]() { emit update_image(Image::Filtered); });
}

View::~View() { delete m_ui; }

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

void View::on_btn_group_buttonClicked(QAbstractButton *button) {
  emit filter_chosen(button->whatsThis());
}

void View::on_pushButton_clicked() {
  QStandardItemModel *model = new QStandardItemModel(3, 3, this);

  model->setHorizontalHeaderLabels(QStringList() << "1"
                                                 << "2"
                                                 << "3");

  m_ui->tableView->setModel(model);

  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      QStandardItem *item = new QStandardItem();
      item->setData(0.0, Qt::EditRole);
      model->setItem(row, col, item);
    }
  }
  m_ui->tableView->resizeColumnsToContents();
}
