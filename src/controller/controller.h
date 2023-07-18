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
  bool SetImage(const QString &);
  void SetColor(const QColor &);
  const QImage &GetOriginal() { return m_model.GetOriginal(); }
  const QImage &GetFiltered() { return m_model.GetFiltered(); }
  bool ApplyFilter(const QString &);

 private:
  Model m_model;
  QMap<QString, std::function<void()>> m_association;
 signals:
  void need_color();
};

#endif  // PHOTOLAB_CONTROLLER_CONTROLLER_H_
