#ifndef SPECTROGRAM_SCALE_WIDGET_HH
#define SPECTROGRAM_SCALE_WIDGET_HH

#include <QWidget>
#include <QPainter>

class SpectrogramScaleWidget : public QWidget {
  Q_OBJECT 
protected:
  void paintEvent(QPaintEvent* event);
public:
  explicit SpectrogramScaleWidget(QWidget *parent = nullptr);
};

#endif // SPECTROGRAM_SCALE_WIDGET_HH
