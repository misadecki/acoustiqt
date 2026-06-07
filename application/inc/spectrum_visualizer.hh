/**
 * @file spectrum_visualizer.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Implementacja głównej części aplikacji, gdzie rysowane są słupki widma
 * częstotliwościowego.
 * @version 0.2
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
  QString xAxisTitle;

  void setxAxisTitle();
public:
  void setFrequencyFormat(AxisFormat format);

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
