#ifndef PHOTOLAB_MODEL_MODEL_H_
#define PHOTOLAB_MODEL_MODEL_H_

#include <QDebug>
#include <QImage>

class Model {
 public:
  void SetImage(const QImage &image) { m_original = m_filtered = image; }
  const QImage &GetOriginal() const { return m_original; }
  const QImage &GetFiltered() const { return m_filtered; }
  void Grayscale();
  void Negative();
  void Toning() { qDebug() << "Toning"; }

 private:
  QImage m_original;
  QImage m_filtered;
};

#endif  // PHOTOLAB_MODEL_MODEL_H_
