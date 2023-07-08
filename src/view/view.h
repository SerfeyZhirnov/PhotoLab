#ifndef PHOTOLAB_VIEW_VIEW_H_
#define PHOTOLAB_VIEW_VIEW_H_

#include <QDir>
#include <QFileDialog>
#include <QMainWindow>
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

 private slots:
  void on_act_open_triggered();
  void on_act_default_triggered();
  void on_btnGroupSent(QString);

 private:
  Ui::MainWindow *m_ui;
  FiltersWindow *m_filters;
  Controller m_controller;

  const int m_display_time = 10000;

  void UpdateLabelImage();
  void UpdateStatusBarMessage(QString);
};
#endif  // PHOTOLAB_VIEW_VIEW_H_
