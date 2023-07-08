#include "model.h"

void Model::Grayscale() {
  for (int x = 0; x < m_filtered.width(); ++x) {
    for (int y = 0; y < m_filtered.height(); ++y) {
      QRgb pixel = m_original.pixel(x, y);
      int red = qRed(pixel) * 299;
      int green = qGreen(pixel) * 587;
      int blue = qBlue(pixel) * 114;

      int result = static_cast<int>((red + green + blue) / 1000);
      m_filtered.setPixel(x, y, qRgb(result, result, result));
    }
  }
}
