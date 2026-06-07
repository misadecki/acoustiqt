/**
 * @file spectrum_visualizer.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Implementacja głównej części aplikacji, gdzie rysowane są słupki widma
 * częstotliwościowego.
 * @version 1.0
 * @date 2026-05-03
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef SPECTRUM_VISUALIZER_H__
#define SPECTRUM_VISUALIZER_H__

#include <QObject>
#include <QList>
#include "base_visualizer.hh"
#include "axis_painter.hh"

/**
  * @class SpectrumVisualizer
  * @brief Klasa służy do wizualizacji widma częstotliwości z odebranych prążków.
  *
  * Klasa implementuje aktualizację widma na podstawie nowych próbek i
  * wyrysowanie graficznej reprezentacji tych danych w aplikacji.
  */
class SpectrumVisualizer : public BaseVisualizer {
  Q_OBJECT 
  AxisFormat current_format = AxisFormat::Auto;
  QList<double> spectrum_data;  /**< Lokalny bufor na ostatnio odebrane dane widma. */
  QString xAxisTitle;           /**< Opis osi X przechowujący wielkość
                                      naniesioną wraz z jednostką. */

  /**
   * @brief Ustawia tytuł osi X w zależności od wybranej jednostki z @ref
   * AxisFormat.
   */
  void setxAxisTitle();
public:

  /**
   * @brief Ustawia format wyświetlania jednostek dla osi częstotliwości.
   * @param[in] format -- wybrany format osi (np. Hz lub kHz) zdefiniowany w
   * typie wyliczeniowym @ref AxisFormat.
   */
  void setFrequencyFormat(AxisFormat format);

  /**
   * @brief Czyści widżet, gdy zerwano połączenie i dane nie są odbierane.
   */
  void clearData();

  /**
  * @brief Inicjalizuje wizualizator widma. 
  *
  * @param[in] parent -- wskaźnik na rodzica tego widżetu.
  */
  explicit SpectrumVisualizer(QWidget *parent = nullptr);
public slots:
  /**
  * @brief Aktualizuje listę zawierającą prążki do wyrysowania i wymusza
  * odświeżanie.
  *
  * @param[in] data -- referencja na listę z gotowymi prążkami do wizualizacji
  * widma.
  */
  void updateSpectrum(const QList<double> &data);
protected:
  /**
  * @brief Obsługuje cykl rysowania widżetu.
  *
  * @param[in] event -- wskaźnik na obiekt zdarzenia rysowania. 
  */
  void paintEvent(QPaintEvent *event) override;
};

#endif 
