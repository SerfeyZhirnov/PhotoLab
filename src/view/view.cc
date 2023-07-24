#include "view.h"

#include "ui_mainwindow.h"

View::View(QWidget *parent) : QMainWindow(parent), m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);
  SetVisibleMatrixMenu(false);
  connect(&m_controller, &Controller::need_color, this, &View::on_colorNeed);
  connect(&m_controller, &Controller::need_kernel, this, &View::on_kernelNeed);
  connect(this, &View::update_image, this, &View::on_imageUpdate);
  connect(this, &View::filter_chosen, this, &View::on_btnGroupSent);
  connect(this, &View::filter_chosen, this,
          [this]() { emit update_image(Image::Filtered); });
  connect(m_ui->hs_brightness, &QSlider::valueChanged, m_ui->sb_brightness,
          &QSpinBox::setValue);
  connect(m_ui->sb_brightness, &QSpinBox::valueChanged, m_ui->hs_brightness,
          &QSlider::setValue);
  connect(m_ui->hs_contrast, &QSlider::valueChanged, m_ui->sb_contrast,
          &QSpinBox::setValue);
  connect(m_ui->sb_contrast, &QSpinBox::valueChanged, m_ui->hs_contrast,
          &QSlider::setValue);
}

View::~View() { delete m_ui; }

void View::resizeEvent(QResizeEvent *event) {
  QMainWindow::resizeEvent(event);
  emit m_ui->sb_matrix_size->valueChanged(m_ui->sb_matrix_size->value());
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

void View::SetVisibleMatrixMenu(bool visible) {
  m_ui->sb_matrix_size->setVisible(visible);
  m_ui->tv_matrix->setVisible(visible);
  m_ui->bt_custom_apply->setVisible(visible);
  emit update_image(Image::Filtered);
}

void View::on_bt_custom_clicked() { SetVisibleMatrixMenu(true); }

void View::on_sb_matrix_size_valueChanged(int value) {
  QStandardItemModel *model = new QStandardItemModel(value, value, this);
  m_ui->tv_matrix->setModel(model);

  const QSize rect = m_ui->tv_matrix->viewport()->size();
  const int col_width = rect.width() / value;
  const int row_height = rect.height() / value;

  for (int row = 0; row < value; ++row) {
    for (int col = 0; col < value; ++col) {
      QStandardItem *item = new QStandardItem();
      item->setData(0.0, Qt::EditRole);
      item->setTextAlignment(Qt::AlignCenter);
      model->setItem(row, col, item);
      m_ui->tv_matrix->setColumnWidth(col, col_width);
    }
    m_ui->tv_matrix->setRowHeight(row, row_height);
  }
}

void View::on_kernelNeed() {
  SetVisibleMatrixMenu(false);

  QAbstractItemModel *model = m_ui->tv_matrix->model();
  const int rows = model->rowCount();
  const int cols = model->columnCount();

  QVector<QVector<double>> result(rows, QVector<double>(cols));
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      QModelIndex index = model->index(row, col);
      double value = model->data(index).toDouble();
      result[row][col] = value;
    }
  }

  m_controller.SetKernel(result);
}

void View::on_sb_brightness_valueChanged(int value) {
  m_controller.SetBrightness(value);
  emit filter_chosen(m_ui->sb_brightness->whatsThis());
}

void View::on_sb_contrast_valueChanged(int value) {
  m_controller.SetContrast(value);
  emit filter_chosen(m_ui->sb_contrast->whatsThis());
}
