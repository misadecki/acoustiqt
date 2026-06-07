#ifndef THEME_PALETTE_HH
#define THEME_PALETTE_HH

#include <QColor>

struct VisualizerColors {
  QColor background;
  QColor axis;
  QColor grad1;
  QColor grad2;
  QColor grad3;
};

class ThemePalette {
public:
  static VisualizerColors getColors(const QString &themeName) {
    VisualizerColors colors;

    colors.grad1 = QColor(255, 255, 0);
    colors.grad2 = QColor(170, 85, 255);
    colors.grad3 = QColor(85, 0, 127);

    if (themeName == "light") {
      colors.background = QColor("#EDEDED");
      colors.axis = QColor("#321052");
    } else if (themeName == "dark") {
      colors.background = QColor("#11111B");
      colors.axis = QColor("#FFFFFF");
    } else if (themeName == "intense") {
      colors.background = QColor("#831DA3");
      colors.axis = QColor("#FFFFFF");
    }

    return colors;
  }
};

#endif // THEME_PALETTE_HH
