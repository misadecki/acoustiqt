#include "spectrogram_visualizer.hh"
#include <QPainter>
#include <QRect>
#include <QtMinMax>
#include "audio_config.hh"
#include "axis_painter.hh"

void SpectrogramVisualizer::addFFTLine(const QList<double> &latest_fft) {
  if (latest_fft.isEmpty()) return;
  int bins = latest_fft.size();

  QMarginsF margins(60.0, 15.0, 20.0, 50.0);
  int desiredWidth = this->width() - margins.left() - margins.right();
  if (desiredWidth <= 0) desiredWidth = 800;

  if (image.isNull() || image.height() != bins || image.width() != desiredWidth) {
    QImage newImage(desiredWidth, bins, QImage::Format_RGB32);
    newImage.fill(colors.background);

    if (!image.isNull()) {
      QPainter p(&newImage);
      int dx = desiredWidth - image.width();
      p.drawImage(dx, 0, image);
    }
    image = newImage;
  }

  uint16_t w = image.width();
  uint16_t h = image.height();
  QRect sourceRect(1, 0, w - 1, h);
  QImage copy = image.copy(sourceRect);

  QPainter painter(&image);
  painter.drawImage(0, 0, copy);
  painter.end();

  for (int y = 0; y < latest_fft.size(); ++y) {
    double magnitude = latest_fft[y];

    if (y == 0 && latest_fft.size() > 1) {
      magnitude = latest_fft[1];
    }

    image.setPixel(w - 1, h - 1 - y, magnitudeToColor(magnitude));
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
    r = static_cast<int>(colors.grad3.red() + t * (colors.grad2.red() -
                         colors.grad3.red()));
    g = static_cast<int>(colors.grad3.green() + t * (colors.grad2.green() -
                         colors.grad3.green()));
    b = static_cast<int>(colors.grad3.blue() + t * (colors.grad2.blue() -
                         colors.grad3.blue()));
  } else {
    double t = (normalized - 0.5) * 2.0;
    r = static_cast<int>(colors.grad2.red() + t * (colors.grad1.red() -
                         colors.grad2.red()));
    g = static_cast<int>(colors.grad2.green() + t * (colors.grad1.green() -
                         colors.grad2.green()));
    b = static_cast<int>(colors.grad2.blue() + t * (colors.grad1.blue() -
                         colors.grad2.blue()));
  }

  return qRgb(qBound(0, r, 255), qBound(0, g, 255), qBound(0, b, 255));
}

void SpectrogramVisualizer::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  if (!this->isVisible() || this->width() <= 0 || this->height() <= 0) return;

  QPainter painter(this);
  if (!painter.isActive()) return;

  QMarginsF margins(60.0, 15.0, 20.0, 50.0);
  painter.fillRect(rect(), colors.background);
  
  double w = this->width();
  double h = this->height();
  double draw_w = w - margins.left() - margins.right();
  double draw_h = h - margins.top() - margins.bottom();

  if (draw_w <= 0 || draw_h <= 0) return;

  painter.setRenderHint(QPainter::SmoothPixmapTransform);

  if (!image.isNull()) {
    QRectF targetRect(margins.left(), margins.top(), draw_w, draw_h);
    painter.drawImage(targetRect, image);
  }

  double timePerFrame = static_cast<double>(Protocol::SAMPLES_PER_PACKET) /
    AudioConfig::SAMPLE_RATE;

  double totalTime = timePerFrame * image.width();

  AxisPainter::drawXAxis(painter, w, h, margins, tr("Time [s]"), -totalTime, 0.0, 5,
                         AxisFormat::Time, colors.axis);
  AxisPainter::drawYAxis(painter, h, margins, tr("Frequency [Hz]"), 0.0,
                         AudioConfig::SAMPLE_RATE / 2.0, 4, AxisFormat::Auto, colors.axis);
}

void SpectrogramVisualizer::clearData() {
  if (!image.isNull()) {
    image.fill(colors.background);
    update();
  }
}
