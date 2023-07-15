#ifndef PHOTOLAB_VIEW_VIEW_H_
#define PHOTOLAB_VIEW_VIEW_H_

#include <QColorDialog>
#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QString>
#include <QTimer>

#include "controller.h"
#include "filterswindow.h"

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
  FiltersWindow *m_filters;
  Controller m_controller;

  const int m_display_time = 10000;
  enum class Image { Original, Filtered };

  void UpdateStatusBarMessage(QString);

 private slots:
  void on_act_open_triggered();
  void on_act_save_triggered();
  void on_act_default_triggered();
  void on_btnGroupSent(QString);
  void on_imageUpdate(View::Image);
  void on_colorNeed();

 signals:
  void update_image(View::Image);
};
#endif  // PHOTOLAB_VIEW_VIEW_H_
