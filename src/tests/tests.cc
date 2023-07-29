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

TEST(PhotoLab, Toning) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.SetColor(QColor(255, 0, 255));
  model.Toning();
  auto result = model.GetFiltered();

  QImage needed("./datasets/toning.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, Emboss) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.Emboss();
  auto result = model.GetFiltered();

  QImage needed("./datasets/emboss.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, Sharpen) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.Sharpen();
  auto result = model.GetFiltered();

  QImage needed("./datasets/sharpen.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, BoxBlur) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.BoxBlur();
  auto result = model.GetFiltered();

  QImage needed("./datasets/boxblur.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, GaussianBlur) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.GaussianBlur();
  auto result = model.GetFiltered();

  QImage needed("./datasets/gaussianblur.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, LaplacianFilter) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.LaplacianFilter();
  auto result = model.GetFiltered();

  QImage needed("./datasets/laplacianfilter.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, SobelLeft) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.SobelLeft();
  auto result = model.GetFiltered();

  QImage needed("./datasets/sobelleft.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, SobelRight) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.SobelRight();
  auto result = model.GetFiltered();

  QImage needed("./datasets/sobelright.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, SobelFull) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.SobelFull();
  auto result = model.GetFiltered();

  QImage needed("./datasets/sobelfull.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, Custom) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  const QVector<QVector<double>> kernel{{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
  model.SetKernel(kernel);
  model.Custom();
  auto result = model.GetFiltered();

  QImage needed("./datasets/customkernel.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, Brightness) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.SetBrightness(-75);
  model.Brightness();
  auto result = model.GetFiltered();

  QImage needed("./datasets/brightness.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, Contrast) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.SetContrast(60);
  model.Contrast();
  auto result = model.GetFiltered();

  QImage needed("./datasets/contrast.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, HSL) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.SetHSL(180, 15, 50);
  model.HSL();
  auto result = model.GetFiltered();

  QImage needed("./datasets/hsl.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}

TEST(PhotoLab, HSV) {
  QImage image("./datasets/sample.bmp");
  
  Model model;
  model.SetImage(image);
  model.SetHSV(90, 255, 0);
  model.HSV();
  auto result = model.GetFiltered();

  QImage needed("./datasets/hsv.bmp");
  ASSERT_TRUE(AssertImages(result, needed));
}
