/**
 * @file theme_palette.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Plik zawierający definicje kolorów wizualizatorów oraz klasę narzędziową zarządzającą paletami dla różnych motywów.
 * @version 1.0
 * @date 2026-06-07
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef THEME_PALETTE_HH
#define THEME_PALETTE_HH

#include <QColor>

/**
 * @brief Struktura przechowująca paletę kolorów wykorzystywaną do rysowania wykresów.
 */
struct VisualizerColors {
  QColor background;    /**< Kolor tła wizualizatora. */
  QColor axis;          /**< Kolor osi. */
  QColor grad1;         /**< Pierwszy (najniższy) kolor gradientu widma lub spektrogramu. */
  QColor grad2;         /**< Drugi (środkowy) kolor gradientu. */
  QColor grad3;         /**< Trzeci (najwyższy/szczytowy) kolor gradientu. */
};

/**
 * @brief Klasa dostarczająca predefiniowane zestawy kolorów dla poszczególnych motywów aplikacji.
 */
class ThemePalette {
public:
  /**
   * @brief Zwraca zestaw kolorów @ref VisualizerColors dopasowany do podanej nazwy motywu.
   * @param[in] themeName -- azwa wybranego motywu (np. "light", "dark", "intense").
   * @return VisualizerColors -- struktura z zainicjowanymi barwami dla tła, osi i gradientów.
   */
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
      colors.grad1 = QColor("#F0E442");
      colors.grad2 = QColor("#0072B2");
      colors.grad3 = QColor("#000000");
      colors.background = QColor("#831DA3");
      colors.axis = QColor("#FFFFFF");
    }

    return colors;
  }
};

#endif // THEME_PALETTE_HH
