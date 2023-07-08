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

bool Controller::ApplyFilter(const QString &name) {
  bool status = m_model.GetOriginal().isNull();
  if (status) {
    return false;
  }

  if (!association[name]) {
    return false;
  }

  association[name]();

  return true;
}
