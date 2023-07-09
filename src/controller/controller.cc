#include "controller.h"

Controller::Controller() {
  m_association["Grayscale"] = [&]() { m_model.Grayscale(); };
  m_association["Negative"] = [&]() { m_model.Negative(); };
  m_association["Toning"] = [&]() {
    QColor color = QColorDialog::getColor();
    m_model.Toning(color);
  };
}

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

  if (!m_association[name]) {
    return false;
  }

  m_association[name]();

  return true;
}
