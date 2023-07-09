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

      int red = static_cast<int>((pixel.red() * color.red()) / 255);
      int green = static_cast<int>((pixel.green() * color.green()) / 255);
      int blue = static_cast<int>((pixel.blue() * color.blue()) / 255);
      QRgb changed = qRgb(red, green, blue);

      m_filtered.setPixel(x, y, changed);
    }
  }
}

void Model::Convolution(const QVector<QVector<int>> &kernel) {
  int kernel_width = kernel.size();
  int kernel_height = kernel[0].size();
  for (int x = 0; x < m_filtered.width() - 1; ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      int R = 0, G = 0, B = 0, kernel_sum = 0;
      for (int i = 0; i < kernel_width; ++i) {
        for (int j = 0; j < kernel_height; ++j) {
          int pixel_pos_x = x + (i - (kernel_width / 2));
          int pixel_pos_y = y + (j - (kernel_height / 2));
          if ((pixel_pos_x < 0) || (pixel_pos_x > m_filtered.width()) ||
              (pixel_pos_y < 0) || (pixel_pos_y > m_filtered.height())) {
            continue;
          }

          int kernel_val = kernel[i][j];
          QColor pixel = m_original.pixel(pixel_pos_x, pixel_pos_y);
          R += pixel.red() * kernel_val;
          G += pixel.green() * kernel_val;
          B += pixel.blue() * kernel_val;
          kernel_sum += kernel_val;
        }
      }

      if (kernel_sum <= 0) {
        kernel_sum = 1;
      }
      //      R /= kernel_sum;
      //      G /= kernel_sum;
      //      B /= kernel_sum;
      qBound(0, R, 255);
      qBound(0, G, 255);
      qBound(0, B, 255);
      QRgb changed = qRgb(R, G, B);

      m_filtered.setPixel(x, y, changed);
    }
  }
}

void Model::Emboss() {
  qDebug() << "ABOBA";
  const QVector<QVector<int>> kernel{{-2, -1, 0}, {-1, 1, 1}, {0, 1, 2}};
  Convolution(kernel);
}
