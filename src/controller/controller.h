#ifndef PHOTOLAB_CONTROLLER_CONTROLLER_H_
#define PHOTOLAB_CONTROLLER_CONTROLLER_H_

#include <QColorDialog>
#include <QImage>
#include <QMap>
#include <QString>
#include <functional>

#include "model.h"

class Controller {
 public:
  Controller();
  bool SetImage(const QString &filename);
  const QImage &GetOriginal() { return m_model.GetOriginal(); }
  const QImage &GetFiltered() { return m_model.GetFiltered(); }
  bool ApplyFilter(const QString &);

 private:
  Model m_model;
  QMap<QString, std::function<void()>> m_association;
};

#endif  // PHOTOLAB_CONTROLLER_CONTROLLER_H_
