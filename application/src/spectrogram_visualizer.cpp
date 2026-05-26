#include "spectrogram_visualizer.hh"
#include <QPainter>
#include <QRect>
#include <QtMinMax>
#include "audio_config.hh"
#include "axis_painter.hh"

SpectrogramVisualizer::SpectrogramVisualizer(QWidget *parent) : QWidget(parent) {}

void SpectrogramVisualizer::addFFTLine(const QList<double> &latest_fft) {
  if (latest_fft.isEmpty()) return;
  int bins = latest_fft.size();

  int curr_width;

  if (this->width() > 0)
    curr_width = this->width();
  else 
    curr_width = 800;

  if (image.isNull() || image.height() != bins) {
    image = QImage(curr_width, bins, QImage::Format_RGB32);
    image.fill(Qt::black);
  }

  uint16_t w = image.width();
  uint16_t h = image.height();
  QRect sourceRect(1, 0, w - 1, h);
  QImage copy = image.copy(sourceRect);

  QPainter painter(&image);
  painter.drawImage(0, 0, copy);
  painter.end();

  for (int y = 0; y < latest_fft.size(); ++y) {
    image.setPixel(w - 1, h - 1 - y,
                   magnitudeToColor(latest_fft[y]));
  }
  update();
}

QRgb SpectrogramVisualizer::magnitudeToColor(double magnitude) {
  double db = 20 * std::log10(magnitude + AudioConfig::EPSILON) + AudioConfig::NOISE_THRESHOLD;
  int val = qBound(0, static_cast<int32_t>((db / AudioConfig::NOISE_THRESHOLD) * AudioConfig::MAX_UINT8_T),
                   static_cast<int32_t>(AudioConfig::MAX_UINT8_T));
  return qRgb(val / 4, val, val / 2);
}

void SpectrogramVisualizer::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  painter.drawImage(rect(), image);
}
