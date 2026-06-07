#include "spectrogram_scale_widget.hh"
#include "audio_config.hh"

void SpectrogramScaleWidget::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  int w = this->width();
  int h = this->height();
  int margin = 12;
  int y_top = margin;
  int y_bot = h - margin;
  int draw_h = y_bot - y_top;

  if (w <= 0 || h <= 0) return;

  double db_min = AudioConfig::NOISE_THRESHOLD;
  double db_max = 0.0;
  int numTicks = 6;
  int barWidth = qBound(10, static_cast<int>(0.4 * w), 40);
  QLinearGradient gradient(0, y_bot, 0, y_top);

  gradient.setColorAt(1.0, colors.grad1);
  gradient.setColorAt(0.5, colors.grad2);
  gradient.setColorAt(0.0, colors.grad3);

  painter.fillRect(0, y_top, barWidth, draw_h, gradient);
  painter.setPen(colors.axis);
  painter.setFont(QFont("Sans Serif", 8));

  for (int i = 0; i <= numTicks; ++i) {
    double frac = static_cast<double>(i) / numTicks;
    int y = y_bot - static_cast<int>(frac * draw_h);
    double db_val = db_min + (frac * (db_max - db_min));

    if (y == y_top) y += 1;
    if (y == y_bot) y -= 1;

    painter.drawLine(barWidth, y, barWidth + 5, y);
    painter.drawText(barWidth + 10, y + 4, QString("%1 [dBFS]")
                     .arg(static_cast<int>(db_val)));
  }
}
