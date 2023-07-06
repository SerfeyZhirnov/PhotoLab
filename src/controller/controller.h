#ifndef PHOTOLAB_CONTROLLER_CONTROLLER_H_
#define PHOTOLAB_CONTROLLER_CONTROLLER_H_

#include <QImage>
#include <QString>

#include "model.h"

class Controller {
 public:
  bool SetImage(const QString &filename);
  const QImage &GetOriginal() { return m_model.GetOriginal(); }
  const QImage &GetFiltered() { return m_model.GetFiltered(); }

 private:
  Model m_model;
};

#endif  // PHOTOLAB_CONTROLLER_CONTROLLER_H_
