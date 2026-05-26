#ifndef AXIS_PAINTER_HH
#define AXIS_PAINTER_HH

#include <QPainter>
#include <QString>
#include <QMarginsF>

class AxisPainter {
public:

  static void drawXAxis(QPainter &painter, double width, double height,
                      const QMarginsF &margins, const QString &title, double min, double
                      max, int numTicks);
  static void drawYAxis(QPainter &painter, double height,
                        const QMarginsF &margins, const QString &title, double min, double
                        max, int numTicks);
};

#endif // AXIS_PAINTER_HH
