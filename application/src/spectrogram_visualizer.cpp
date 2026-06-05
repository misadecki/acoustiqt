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
  if (std::isnan(magnitude) || magnitude <= 0.0) magnitude =
    AudioConfig::EPSILON;

  double db = 20 * std::log10(magnitude);
  double min_db = -AudioConfig::NOISE_THRESHOLD;
  double max_db = 0.0;

  double normalized = (db - min_db) / (max_db - min_db);
  normalized = qBound(0.0, normalized, 1.0);
  int r, g, b;

  if (normalized < 0.5) {
    double t = normalized * 2.0; 
    r = static_cast<int>(t * 150);
    g = 0;
    b = static_cast<int>(t * 200);
  } else {
    double t = (normalized - 0.5) * 2.0;
    r = static_cast<int>(150 + t * (255 - 150));
    g = static_cast<int>(t * 255);
    b = static_cast<int>(200 - t * 200); 
  }

  return qRgb(r, g, b);
}

void SpectrogramVisualizer::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  if (!this->isVisible() || this->width() <= 0 || this->height() <= 0) return;

  QPainter painter(this);
  if (!painter.isActive()) return;

  QMarginsF margins(60.0, 15.0, 20.0, 50.0);
  painter.fillRect(rect(), Qt::black);
  
  double w = this->width();
  double h = this->height();
  double draw_w = w - margins.left() - margins.right();
  double draw_h = h - margins.top() - margins.bottom();

  if (draw_w <= 0 || draw_h <= 0) return;

  if (!image.isNull()) {
    QRectF targetRect(margins.left(), margins.top(), draw_w, draw_h);
    painter.drawImage(targetRect, image);
  }

  AxisPainter::drawXAxis(painter, w, h, margins, "Time [s]", -10.0, 0.0, 5);
  AxisPainter::drawYAxis(painter, h, margins, "Frequency [Hz]", 0.0,
                         AudioConfig::SAMPLE_RATE / 2.0, 4);
}
