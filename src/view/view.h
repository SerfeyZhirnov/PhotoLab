#ifndef PHOTOLAB_VIEW_VIEW_H_
#define PHOTOLAB_VIEW_VIEW_H_

#include <QAbstractButton>
#include <QAbstractScrollArea>
#include <QColorDialog>
#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QStandardItemModel>
#include <QString>
#include <QTimer>

#include "controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class View : public QMainWindow {
  Q_OBJECT
 public:
  View(QWidget *parent = nullptr);
  ~View();

 protected:
  void resizeEvent(QResizeEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;

 private:
  Ui::MainWindow *m_ui;
  Controller *m_controller;

  const int m_display_time = 10000;
  enum class Image { Original, Filtered };

  void UpdateStatusBarMessage(QString);
  void SetVisibleMatrixMenu(bool);

 private slots:
  void on_act_open_triggered();
  void on_act_save_triggered();
  void on_btnGroupSent(QString);
  void on_imageUpdate(View::Image);
  void on_colorNeed();
  void on_btn_group_buttonClicked(QAbstractButton *);
  void on_bt_custom_clicked();
  void on_sb_matrix_size_valueChanged(int);
  void on_kernelNeed();
  void on_sb_brightness_valueChanged(int);
  void on_sb_contrast_valueChanged(int);

 signals:
  void update_image(View::Image);
  void filter_chosen(QString);
};
#endif  // PHOTOLAB_VIEW_VIEW_H_
