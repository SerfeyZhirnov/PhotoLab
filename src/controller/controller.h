#ifndef PHOTOLAB_CONTROLLER_CONTROLLER_H_
#define PHOTOLAB_CONTROLLER_CONTROLLER_H_

#include <QImage>
#include <QMap>
#include <QObject>
#include <QString>
#include <functional>

#include "model.h"

class Controller : public QObject {
  Q_OBJECT
 public:
  Controller();
  ~Controller();
  bool SetImage(const QString &);
  void SetColor(const QColor &);
  void SetKernel(const QVector<QVector<double>> &kernel) {
    m_model->SetKernel(kernel);
  }
  void SetBrightness(const int &value) { m_model->SetBrightness(value); }
  void SetContrast(const int &value) { m_model->SetContrast(value); }
  const QImage &GetOriginal() { return m_model->GetOriginal(); }
  const QImage &GetFiltered() { return m_model->GetFiltered(); }
  bool ApplyFilter(const QString &);

  enum class Color { Simple, Custom };

 private:
  Model *m_model;
  QMap<QString, std::function<void()>> m_association;
 signals:
  void need_color(Controller::Color);
  void need_kernel();
};

#endif  // PHOTOLAB_CONTROLLER_CONTROLLER_H_
