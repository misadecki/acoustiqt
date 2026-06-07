#include "spectrum_visualizer.hh"
#include <QPainter>
#include <QPainterPath>
#include "audio_config.hh"
#include "axis_painter.hh"

SpectrumVisualizer::SpectrumVisualizer(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_StyledBackground, true);
}

void SpectrumVisualizer::updateSpectrum(const QList<double> &data) {
  spectrum_data = data;
  update();
}

void SpectrumVisualizer::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  if (!this->isVisible() || this->width() <= 0 || this->height() <= 0) return;
  if (spectrum_data.isEmpty()) return;

  QPainter painter(this);
  QMarginsF margins(60.0, 15.0, 20.0, 50.0);

  if(!painter.isActive()) return;

  QPainterPath clipPath;
  clipPath.addRoundedRect(rect(), 5.0, 5.0);
  painter.setClipPath(clipPath);

  painter.fillRect(rect(), qRgb(200, 200, 200));

  double width = this->width();
  double height = this->height();
  int nbars = spectrum_data.size();

  double draw_width = width - margins.left() - margins.right();
  double draw_height = height - margins.top() - margins.bottom();

  if (draw_height <= 0 || draw_width <= 0) return;

  double bar_width = static_cast<double>(width) / nbars;
  const double max_db = 0.0;
  const double min_db = -AudioConfig::NOISE_THRESHOLD;

  for (int i = 0; i < nbars; ++i) {
    double magnitude = spectrum_data[i];
    if (std::isnan(magnitude) || magnitude < 0.0)
      magnitude = 0.0;

    double db = 20.0 * std::log10(magnitude + AudioConfig::EPSILON);

    if (std::isinf(db) || db < min_db)
      db = min_db;

    double normalized = (db - min_db) / (max_db - min_db);
    normalized = qBound(0.0, normalized, 1.0);

    double bar_height = normalized * draw_height;
    double x = margins.left() + (i * bar_width);
    double y = margins.top() + draw_height - bar_height;
    double draw_w = std::max(1.0, bar_width - 1.0);

    QLinearGradient gradient(x, y, x, y + bar_height);
    gradient.setColorAt(0.0, QColor(255, 255, 0));
    gradient.setColorAt(0.5, QColor(170, 85, 255));
    gradient.setColorAt(1.0, QColor(85, 0, 127));
    painter.fillRect(QRectF(x, y, draw_w, bar_height), gradient);
  }

  setxAxisTitle();
  AxisPainter::drawXAxis(painter, width, height, margins, xAxisTitle, 0.0,
                         AudioConfig::SAMPLE_RATE / 2.0, 5, current_format,
                         axisColor);
  AxisPainter::drawYAxis(painter, height, margins, tr("Amplitude spectrum [dBFS]"), 
                         -AudioConfig::NOISE_THRESHOLD, 0.0, 4, current_format,
                         axisColor);
}

void SpectrumVisualizer::setxAxisTitle() {
  switch (current_format) {
    case AxisFormat::Hz:
      xAxisTitle = tr("Frequency [Hz]");
      break;
    case AxisFormat::kHz:
      xAxisTitle = tr("Frequency [kHz]");
      break;
    case AxisFormat::Auto:
    default:
      xAxisTitle = tr("Frequency [Hz/kHz]");
      break;
  }
}

void SpectrumVisualizer::setFrequencyFormat(AxisFormat format) {
  current_format = format;
  update();
}

void SpectrumVisualizer::applyThemeColors(const QString &themeName) {
  if (themeName == "light") {
    bgColor = QColor("#EDEDED");
    axisColor = QColor("#321052");
  } else if (themeName == "dark") {
    bgColor = QColor("#11111B");
    axisColor = QColor("#FFFFFF");
  } else if (themeName == "intense") {
    bgColor = QColor("#831DA3");
    axisColor = QColor("#FFFFFF");
  }
  update();
}
