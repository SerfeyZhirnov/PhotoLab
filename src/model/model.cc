#include "model.h"

void Model::Grayscale() {
  for (int x = 0; x < m_filtered.width(); ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      QRgb pixel = m_original.pixel(x, y);

      int red = qRed(pixel) * 299;
      int green = qGreen(pixel) * 587;
      int blue = qBlue(pixel) * 114;
      int result = static_cast<int>((red + green + blue) / 1000);
      QRgb changed = qRgb(result, result, result);

      m_filtered.setPixel(x, y, changed);
    }
  }
}

void Model::Negative() {
  for (int x = 0; x < m_filtered.width(); ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      QRgb pixel = m_original.pixel(x, y);

      int red = 255 - qRed(pixel);
      int green = 255 - qGreen(pixel);
      int blue = 255 - qBlue(pixel);
      QRgb changed = qRgb(red, green, blue);

      m_filtered.setPixel(x, y, changed);
    }
  }
}

void Model::Toning(QColor &color) {
  if (!color.isValid()) {
    color = Qt::white;
  }

  for (int x = 0; x < m_filtered.width(); ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      QColor pixel = m_original.pixel(x, y);

      int red = (pixel.red() * color.red()) / 255;
      int green = (pixel.green() * color.green()) / 255;
      int blue = (pixel.blue() * color.blue()) / 255;
      QRgb changed = qRgb(red, green, blue);

      m_filtered.setPixel(x, y, changed);
    }
  }
}

constexpr void Model::Convolution(const QVector<QVector<double>> &kernel) {
  for (int x = 0; x < m_filtered.width(); ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      int R = 0, G = 0, B = 0;

      CalculateColors(kernel, x, y, R, G, B);

      R = qBound(0, R, 255);
      G = qBound(0, G, 255);
      B = qBound(0, B, 255);
      QRgb changed = qRgb(R, G, B);

      m_filtered.setPixel(x, y, changed);
    }
  }
}

constexpr void Model::CalculateColors(const QVector<QVector<double>> &kernel,
                                      int &x, int &y, int &R, int &G, int &B) {
  const int kernel_size = kernel.size();

  for (int i = 0; i < kernel_size; ++i) {
    for (int j = 0; j < kernel_size; ++j) {
      const int offset_x = i - (kernel_size / 2);
      const int offset_y = j - (kernel_size / 2);
      int pixel_pos_x = x + offset_x;
      int pixel_pos_y = y + offset_y;

      if ((pixel_pos_x < 0) || (pixel_pos_x >= m_filtered.width())) {
        pixel_pos_x = x - offset_x;
      }
      if ((pixel_pos_y < 0) || (pixel_pos_y >= m_filtered.height())) {
        pixel_pos_y = y - offset_y;
      }

      double kernel_val = kernel[i][j];
      QColor pixel = m_original.pixel(pixel_pos_x, pixel_pos_y);
      R += static_cast<int>(pixel.red() * kernel_val);
      G += static_cast<int>(pixel.green() * kernel_val);
      B += static_cast<int>(pixel.blue() * kernel_val);
    }
  }
}

void Model::Emboss() {
  const QVector<QVector<double>> kernel{{-2, -1, 0}, {-1, 1, 1}, {0, 1, 2}};
  Convolution(kernel);
}

void Model::Sharpen() {
  const QVector<QVector<double>> kernel{{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
  Convolution(kernel);
}

void Model::BoxBlur() {
  const double value = 1. / 9.;
  const QVector<QVector<double>> kernel{
      {value, value, value}, {value, value, value}, {value, value, value}};
  Convolution(kernel);
}

void Model::GaussianBlur() {
  const double value1 = 1. / 16.;
  const double value2 = 1. / 8.;
  const double value3 = 1. / 4.;
  const QVector<QVector<double>> kernel{{value1, value2, value1},
                                        {value2, value3, value2},
                                        {value1, value2, value1}};
  Convolution(kernel);
}

void Model::LaplacianFilter() {
  const QVector<QVector<double>> kernel{
      {-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
  Convolution(kernel);
}