#ifndef PHOTOLAB_CONTROLLER_CONTROLLER_H_
#define PHOTOLAB_CONTROLLER_CONTROLLER_H_

#include <QImage>
#include <QString>

#include "model.h"

class Controller {
 public:
  bool SetOriginal(const QString &filename) {
    if (filename.isEmpty()) {
      return false;
    }

    QImage image;
    bool loaded = image.load(filename);
    if (!loaded) {
      return false;
    }

    m_model.SetOriginal(image);
    return true;
  }

 private:
  Model m_model;
};

#endif  // PHOTOLAB_CONTROLLER_CONTROLLER_H_
