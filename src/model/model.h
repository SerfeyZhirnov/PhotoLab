#ifndef PHOTOLAB_MODEL_MODEL_H_
#define PHOTOLAB_MODEL_MODEL_H_

#include <QDebug>
#include <QImage>
#include <QVector>

class Model {
 public:
  void SetImage(const QImage &image) { m_original = m_filtered = image; }
  void SetColor(const QColor &color) { m_custom_color = color; }
  const QImage &GetOriginal() const { return m_original; }
  const QImage &GetFiltered() const { return m_filtered; }
  void Grayscale();
  void Negative();
  void Toning();
  void Emboss();
  void Sharpen();
  void BoxBlur();
  void GaussianBlur();
  void LaplacianFilter();
  void SobelLeft();
  void SobelRight();
  void SobelFull();

 private:
  QImage m_original;
  QImage m_filtered;
  QColor m_custom_color;

  constexpr inline void Convolution(const QVector<QVector<double>> &);
  constexpr inline void CalculateColors(const QVector<QVector<double>> &, int &,
                                        int &, int &, int &, int &);
  constexpr inline void Overlap(const QImage &);
};

#endif  // PHOTOLAB_MODEL_MODEL_H_
