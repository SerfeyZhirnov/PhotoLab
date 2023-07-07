#ifndef FILTERSWINDOW_H
#define FILTERSWINDOW_H

#include <QWidget>

namespace Ui {
class FiltersWindow;
}

class FiltersWindow : public QWidget {
  Q_OBJECT

 public:
  explicit FiltersWindow(QWidget *parent = nullptr);
  ~FiltersWindow();

 private:
  Ui::FiltersWindow *ui;
};

#endif  // FILTERSWINDOW_H
