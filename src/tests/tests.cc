// toning 255 0 255
// custom matrix -1 -1 -1 -1 8 -1 -1 -1 -1
// brightness -75
#include <gtest/gtest.h>

#include "model.h"

bool AssertImages(const QImage &img1, const QImage &img2) {
  for (int x = 0; x < img1.width(); ++x) {
    for (int y = 0; y < img1.height(); ++y) {
        auto p1 = img1.pixel(x, y);
        auto p2 = img2.pixel(x,y); 
        if (p1 != p2) {
          return false;
        }
    }
  }

  return true;
}

TEST(PhotoLab, Grayscale) {
  QImage image("./datasets/sample.bmp");

  Model model;
  model.SetImage(image);
  model.Grayscale();
  auto result = model.GetFiltered();

  QImage needed("./datasets/grayscale.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, Negative) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.Negative();
  auto result = model.GetFiltered();

  QImage needed("./datasets/negative.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}