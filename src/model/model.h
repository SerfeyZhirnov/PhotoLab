#ifndef PHOTOLAB_MODEL_MODEL_H_
#define PHOTOLAB_MODEL_MODEL_H_

#include <QDebug>
#include <QImage>
#include <QVector>

class Model {
 public:
  Model() = default;
  ~Model() = default;
  void SetImage(const QImage &image) { m_original = m_filtered = image; }
  void SetColor(const QColor &color) { m_custom_color = color; }
  void SetKernel(const QVector<QVector<double>> &kernel) {
    m_custom_kernel = kernel;
  }
  void SetBrightness(const int &value) { m_brightness = value; }
  void SetContrast(const int &value) {
    m_contrast = qPow((100.0 + value) / 100.0, 2.0);
  }
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
  void Custom();
  void Brightness();
  void Contrast();
  void HSL();
  void HSV();

 private:
  QImage m_original;
  QImage m_filtered;
  QColor m_custom_color;
  QVector<QVector<double>> m_custom_kernel;
  int m_brightness;
  double m_contrast;

  inline void CalculateColors(const QVector<QVector<double>> &, int &, int &,
                              int &, int &, int &);
  inline void Convolution(const QVector<QVector<double>> &);
  inline void Overlap(const QImage &);
  constexpr inline int ApplyContrast(int) const;
};

#endif  // PHOTOLAB_MODEL_MODEL_H_
