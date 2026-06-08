#include "axis_painter.hh"

void AxisPainter::drawXAxis(QPainter &painter, double width, double height,
                      const QMarginsF &margins, const QString &title, double min, double
                      max, int numTicks, AxisFormat format, QColor axisColor) {
  painter.setPen(axisColor);
  painter.setFont(QFont("Sans Serif", 8));
  QFontMetrics fm = painter.fontMetrics();

  double draw_w = width - margins.left() - margins.right();
  double draw_h = height - margins.top() - margins.bottom();

  painter.drawLine(margins.left(), margins.top() + draw_h, margins.left() + draw_w, margins.top() + draw_h);

  for (int i = 0; i <= numTicks; ++i) {
    double frac = static_cast<double>(i) / numTicks;
    double x_pos = margins.left() + (frac * draw_w);
    double x = min + frac * (max - min);

    painter.drawLine(x_pos, margins.top() + draw_h, x_pos, margins.top() + draw_h + 5);

    QString label;
    if (format == AxisFormat::Time) {
      label = QString::number(x, 'f', 1);
    } else if (format == AxisFormat::kHz)
      label = QString::number(x / 1000.0, 'f', 1) + "k";
    else if (format == AxisFormat::Hz)
      label = QString::number(static_cast<int>(x));
    else {
      if (x >= 1000.0)
        label = QString::number(x / 1000.0, 'f', 1) + "k";
      else
        label = QString::number(static_cast<int>(x));
    }

    int text_w = fm.horizontalAdvance(label);

    painter.drawText(x_pos - (text_w / 2.0), margins.top() + draw_h + 20, label);
  }

  int title_w = fm.horizontalAdvance(title);
  painter.drawText(margins.left() + (draw_w / 2.0) - (title_w / 2.0), height - 5,
                   title);
}

void AxisPainter::drawYAxis(QPainter &painter, double height,
                        const QMarginsF &margins, const QString &title, double min, double
                        max, int numTicks, AxisFormat format, QColor axisColor) {
  painter.setPen(axisColor);
  painter.setFont(QFont("Sans Serif", 8));
  QFontMetrics fm = painter.fontMetrics();
  double draw_h = height - margins.top() - margins.bottom();

  painter.drawLine(margins.left(), margins.top(), margins.left(), margins.top()
                   + draw_h);

  for (int i = 0; i <= numTicks; ++i) {
    double frac = static_cast<double>(i) / numTicks;
    double y_pos = margins.top() + (frac * draw_h);
    double y = max - frac * (max - min);

    painter.drawLine(margins.left() - 5, y_pos, margins.left(), y_pos);

    QString label = QString::number(static_cast<int>(y));

    if (format == AxisFormat::kHz)
      label = QString::number(y / 1000.0, 'f', 1);
    else if (format == AxisFormat::Hz)
      label = QString::number(static_cast<int>(y));
    else {
      if (y >= 1000.0)
        label = QString::number(y / 1000.0, 'f', 1) + "k";
      else
        label = QString::number(static_cast<int>(y));
    }

    int text_w = fm.horizontalAdvance(label);

    painter.drawText(margins.left() - 10 - text_w, y_pos + 4, label);
  }

  int title_w = fm.horizontalAdvance(title);
  painter.save();
  painter.translate(15, margins.top() + (draw_h / 2.0));
  painter.rotate(-90);
  painter.drawText(-(title_w / 2.0), 0, title);
  painter.restore();
}
