#ifndef PHOTOLAB_MODEL_MODEL_H_
#define PHOTOLAB_MODEL_MODEL_H_

#include <QDebug>
#include <QImage>
#include <QVector>

class Model {
 public:
  void SetImage(const QImage &image) { m_original = m_filtered = image; }
  const QImage &GetOriginal() const { return m_original; }
  const QImage &GetFiltered() const { return m_filtered; }
  void Grayscale();
  void Negative();
  void Toning(QColor &);
  void Emboss();
  void Sharpen();

 private:
  QImage m_original;
  QImage m_filtered;

  constexpr inline void Convolution(const QVector<QVector<double>> &);
  constexpr inline void CalculateColors(const QVector<QVector<double>> &, int &,
                                        int &, int &, int &, int &);
};

#endif  // PHOTOLAB_MODEL_MODEL_H_
