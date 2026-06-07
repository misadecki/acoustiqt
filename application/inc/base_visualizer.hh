#ifndef BASE_VISUALIZER_HH
#define BASE_VISUALIZER_HH

#include <QWidget>
#include "theme_palette.hh"

class BaseVisualizer : public QWidget {
  Q_OBJECT
protected:
  VisualizerColors colors;
public:
  explicit BaseVisualizer(QWidget *parent = nullptr) : QWidget(parent) {}
  void applyThemeColors(const QString &themeName) {
    colors = ThemePalette::getColors(themeName);
    update();
  }
};

#endif // BASE_VISUALIZER_HH
