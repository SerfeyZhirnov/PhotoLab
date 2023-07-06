#include "controller.h"

bool Controller::SetImage(const QString &filename) {
  if (filename.isEmpty()) {
    return false;
  }

  QImage image;
  bool loaded = image.load(filename);
  if (!loaded) {
    return false;
  }

  m_model.SetImage(image);
  return true;
}
