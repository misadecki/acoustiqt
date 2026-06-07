/**
 * @file base_visualizer.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Plik zawierający definicję formatów osi oraz klasę bazową dla widżetów wizualizujących dane.
 * @version 1.0
 * @date 2026-06-07
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */


#ifndef BASE_VISUALIZER_HH
#define BASE_VISUALIZER_HH

#include <QWidget>
#include "theme_palette.hh"

/**
 * @brief Klasa bazowa dla wszystkich widżetów wizualizujących dane.
 * 
 * Dostarcza mechanizmy do zarządzania kolorystyką widżetu w zależności od wybranego motywu (motyw jasny/ciemny itp.).
 * Dziedziczy po QWidget.
 */
class BaseVisualizer : public QWidget {
  Q_OBJECT
protected:
  VisualizerColors colors;  /**< Struktura przechowująca aktualną paletę kolorów wizualizatora. */
public:
  /**
   * @brief Konstruktor klasy BaseVisualizer.
   * @param[in] parent -- wskaźnik na widżet nadrzędny (domyślnie nullptr).
   */
  explicit BaseVisualizer(QWidget *parent = nullptr) : QWidget(parent) {}
  /**
   * @brief Aktualizuje paletę kolorów wizualizatora na podstawie nazwy motywu.
   * @param[in] themeName -- nazwa motywu do zastosowania (np. "dark", "light").
   */
  void applyThemeColors(const QString &themeName) {
    colors = ThemePalette::getColors(themeName);
    update();
  }
};

#endif // BASE_VISUALIZER_HH
