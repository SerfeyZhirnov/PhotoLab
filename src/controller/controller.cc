#include "controller.h"

Controller::Controller() : m_model(new Model) {
  m_association["Grayscale"] = [&]() { m_model->Grayscale(); };
  m_association["Negative"] = [&]() { m_model->Negative(); };
  m_association["Toning"] = [&]() {
    emit need_color();
    m_model->Toning();
  };
  m_association["Emboss"] = [&]() { m_model->Emboss(); };
  m_association["Sharpen"] = [&]() { m_model->Sharpen(); };
  m_association["Box blur"] = [&]() { m_model->BoxBlur(); };
  m_association["Gaussian blur"] = [&]() { m_model->GaussianBlur(); };
  m_association["Laplacian"] = [&]() { m_model->LaplacianFilter(); };
  m_association["Sobel left"] = [&]() { m_model->SobelLeft(); };
  m_association["Sobel right"] = [&]() { m_model->SobelRight(); };
  m_association["Sobel full"] = [&]() { m_model->SobelFull(); };
  m_association["Custom filter"] = [&]() {
    emit need_kernel();
    m_model->Custom();
  };
  m_association["Brightness"] = [&]() { m_model->Brightness(); };
  m_association["Contrast"] = [&]() { m_model->Contrast(); };
}

Controller::~Controller() { delete m_model; }

bool Controller::SetImage(const QString &filename) {
  if (filename.isEmpty()) {
    return false;
  }

  QImage image;
  bool loaded = image.load(filename);
  if (!loaded) {
    return false;
  }

  m_model->SetImage(image);
  return true;
}

void Controller::SetColor(const QColor &color) {
  if (color.isValid()) {
    m_model->SetColor(color);
  } else {
    m_model->SetColor(Qt::white);
  }
}

bool Controller::ApplyFilter(const QString &name) {
  bool status = m_model->GetOriginal().isNull();
  if (status) {
    return false;
  }

  if (!m_association[name]) {
    return false;
  }

  m_association[name]();

  return true;
}
