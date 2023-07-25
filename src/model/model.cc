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

void Model::Toning() {
  for (int x = 0; x < m_filtered.width(); ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      QColor pixel = m_original.pixel(x, y);

      int red = (pixel.red() * m_custom_color.red()) / 255;
      int green = (pixel.green() * m_custom_color.green()) / 255;
      int blue = (pixel.blue() * m_custom_color.blue()) / 255;
      QRgb changed = qRgb(red, green, blue);

      m_filtered.setPixel(x, y, changed);
    }
  }
}

void Model::CalculateColors(const QVector<QVector<double>> &kernel, int &x,
                            int &y, int &red, int &green, int &blue) {
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
      red += static_cast<int>(pixel.red() * kernel_val);
      green += static_cast<int>(pixel.green() * kernel_val);
      blue += static_cast<int>(pixel.blue() * kernel_val);
    }
  }
}

void Model::Convolution(const QVector<QVector<double>> &kernel) {
  for (int x = 0; x < m_filtered.width(); ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      int red = 0, green = 0, blue = 0;

      CalculateColors(kernel, x, y, red, green, blue);

      red = qBound(0, red, 255);
      green = qBound(0, green, 255);
      blue = qBound(0, blue, 255);
      QRgb changed = qRgb(red, green, blue);

      m_filtered.setPixel(x, y, changed);
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

void Model::SobelLeft() {
  const QVector<QVector<double>> kernel{{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
  Convolution(kernel);
}

void Model::SobelRight() {
  const QVector<QVector<double>> kernel{{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
  Convolution(kernel);
}

void Model::Overlap(const QImage &image) {
  for (int x = 0; x < m_filtered.width(); ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      QRgb first = m_filtered.pixel(x, y);
      QRgb second = image.pixel(x, y);
      QRgb changed = first + second;

      m_filtered.setPixel(x, y, changed);
    }
  }
}

void Model::SobelFull() {
  SobelLeft();
  const QImage buf = m_filtered;
  SobelRight();
  Overlap(buf);
}

void Model::Custom() { Convolution(m_custom_kernel); }

void Model::Brightness() {
  for (int x = 0; x < m_original.width(); ++x) {
    for (int y = 0; y < m_original.height(); ++y) {
      QColor pixel = m_original.pixel(x, y);

      int red = qBound(0, pixel.red() + m_brightness, 255);
      int green = qBound(0, pixel.green() + m_brightness, 255);
      int blue = qBound(0, pixel.blue() + m_brightness, 255);

      QRgb changed = qRgb(red, green, blue);
      m_filtered.setPixel(x, y, changed);
    }
  }
}

constexpr int Model::ApplyContrast(int color) const {
  double result = static_cast<double>(color) / 255.0;
  result -= 0.5;
  result *= m_contrast;
  result += 0.5;
  result *= 255.0;
  return qBound(0, static_cast<int>(result), 255);
}

void Model::Contrast() {
  for (int x = 0; x < m_original.width(); ++x) {
    for (int y = 0; y < m_original.height(); ++y) {
      QColor pixel = m_original.pixel(x, y);

      int red = ApplyContrast(pixel.red());
      int green = ApplyContrast(pixel.green());
      int blue = ApplyContrast(pixel.blue());

      QRgb changed = qRgb(red, green, blue);
      m_filtered.setPixel(x, y, changed);
    }
  }
}

void Model::HSL() {
  for (int x = 0; x < m_original.width(); ++x) {
    for (int y = 0; y < m_original.height(); ++y) {
      QColor color = m_original.pixelColor(x, y).toHsl();

      int hue = qBound(0, m_custom_color.hue() + color.hue(), 360);
      int saturation =
          qBound(0, m_custom_color.saturation() + color.saturation(), 255);
      int lightness =
          qBound(0, m_custom_color.lightness() + color.lightness(), 255);

      color.setHsl(hue, saturation, lightness);
      m_filtered.setPixelColor(x, y, color);
    }
  }
}

void Model::HSV() {
  for (int x = 0; x < m_original.width(); ++x) {
    for (int y = 0; y < m_original.height(); ++y) {
      QColor color = m_original.pixelColor(x, y).toHsv();

      int hue = qBound(0, m_custom_color.hue() + color.hue(), 360);
      int saturation =
          qBound(0, m_custom_color.saturation() + color.saturation(), 255);
      int lightness = qBound(0, m_custom_color.value() + color.value(), 255);

      color.setHsv(hue, saturation, lightness);
      m_filtered.setPixelColor(x, y, color);
    }
  }
}
