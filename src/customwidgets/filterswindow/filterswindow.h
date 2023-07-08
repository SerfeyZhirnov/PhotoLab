#ifndef FILTERSWINDOW_H
#define FILTERSWINDOW_H

#include <QAbstractButton>
#include <QWidget>

namespace Ui {
class FiltersWindow;
}

class FiltersWindow : public QWidget {
  Q_OBJECT

 public:
  explicit FiltersWindow(QWidget *parent = nullptr);
  ~FiltersWindow();

 private slots:
  void on_btn_group_buttonClicked(QAbstractButton *);

 signals:
  void filter_chosen(QString);

 private:
  Ui::FiltersWindow *ui;
};

#endif  // FILTERSWINDOW_H
