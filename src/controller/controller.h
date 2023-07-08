#ifndef PHOTOLAB_CONTROLLER_CONTROLLER_H_
#define PHOTOLAB_CONTROLLER_CONTROLLER_H_

#include <QImage>
#include <QMap>
#include <QString>
#include <functional>

#include "model.h"

class Controller {
 public:
  bool SetImage(const QString &filename);
  const QImage &GetOriginal() { return m_model.GetOriginal(); }
  const QImage &GetFiltered() { return m_model.GetFiltered(); }
  bool ApplyFilter(const QString &);

 private:
  Model m_model;
  const QMap<QString, std::function<void()>> association = {
      {"Grayscale", [this]() { m_model.Grayscale(); }},
      {"Negative", [this]() { m_model.Negative(); }},
      {"Toning", [this]() { m_model.Toning(); }}};
};

#endif  // PHOTOLAB_CONTROLLER_CONTROLLER_H_
