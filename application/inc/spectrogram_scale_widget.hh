/**
 * @file spectrogram_scale_widget.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Wyrysowywanie skali dla spektrogramu w postaci słupka z kolorem i
 * naniesioną skalą w [dBFS].
 * @version 1.0
 * @date 2026-05-26
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef SPECTROGRAM_SCALE_WIDGET_HH
#define SPECTROGRAM_SCALE_WIDGET_HH

#include <QWidget>
#include <QPainter>

/**
 * @brief Widżet rysujący legendę (skalę) dla spektrogramu.
 * * @details Klasa odpowiedzialna za wyrenderowanie pionowego paska gradientu oraz 
 * etykiet tekstowych reprezentujących wartości w [dBFS]. Działa jako osobny, niezależny 
 * komponent interfejsu, który automatycznie dopasowuje się do wysokości głównego wykresu.
 */
class SpectrogramScaleWidget : public QWidget {
  Q_OBJECT 
protected:
  /**
   * @brief Główna funkcja rysująca interfejs skali.
   * * @details Wywoływana automatycznie przez system Qt przy każdej potrzebie 
   * odświeżenia widżetu (np. zmiana rozmiaru okna). Maluje tło, pasek z gradientem 
   * oraz nakłada na niego podziałkę wraz z odpowiednimi wartościami decybeli.
   * * @param[in] event -- wskaźnik na zdarzenie odrysowania (nieużywany bezpośrednio w kodzie).
   */
  void paintEvent(QPaintEvent* event);
public:
  /**
   * @brief Konstruktor klasy SpectrogramScaleWidget.
   * * @param[in] parent -- wskaźnik na widżet nadrzędny, domyślnie nullptr.
   */
  explicit SpectrogramScaleWidget(QWidget *parent = nullptr);
};

#endif // SPECTROGRAM_SCALE_WIDGET_HH
