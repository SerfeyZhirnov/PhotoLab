#ifndef PHOTOLAB_VIEW_VIEW_H_
#define PHOTOLAB_VIEW_VIEW_H_

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();

private:
    Ui::MainWindow *ui;
};
#endif // PHOTOLAB_VIEW_VIEW_H_
