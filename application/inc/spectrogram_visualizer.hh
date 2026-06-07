/**
 * @file spectrogram_visualizer.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Implementacja rysowania spektrogramu w drugim oknie aplikacji.
 * @version 0.2
 * @date 2026-05-06
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef SPECTROGRAM_VISUALIZER_H__
#define SPECTROGRAM_VISUALIZER_H__

#include <QImage>
#include "base_visualizer.hh"
#include <QList>
#include <QRgb>

/**
  * @class SpectrogramVisualizer
  * @brief Klasa służy do wizualizacji spektrogramu widma częstotliwości z
  * odebieranych prążków.
  *
  * Klasa implementuje odświeżanie widżetu ze spektrogramem. Ten jest
  * wyrysowywany poprzez przesuwanie obrazu o jeden piksel w lewo, a następnie
  * dodanie nowego paska z amplitudami widma. 
  */
class SpectrogramVisualizer : public BaseVisualizer {
  Q_OBJECT
  /**
  * @brief Konwertuje amplitudę na kolor RGB. 
  *
  * @param[in] magnitude -- wartość amplitudy [dB]
  */
  QRgb magnitudeToColor(double magnitude);
  QImage image;  /**< Obraz przechowujący bufor graficzny (Off-screen rendering). */
protected:
  /**
  * @brief Obsługuje cykl rysownaia widżetu spektrogramu.
  *
  * @param[in] event -- wskaźnik na obiekt zdarzenia rysowania. 
  */
  void paintEvent(QPaintEvent *event) override;
public slots:
  /**
  * @brief Dodaje linię spektrogramu z nowo zebranych próbek.
  *
  * @param[in] latest_fft -- referencja na listę z ostatnio odebranymi próbkami.
  */
  void addFFTLine(const QList<double> &lastest_fft);
public:
  /**
  * @brief Inicjalizuje wizualizator spektrogramu. 
  *
  * @param[in] parent -- wskaźnik na rodzica tego widżetu.
  */
  explicit SpectrogramVisualizer(QWidget *parent = nullptr) :
    BaseVisualizer(parent) {}

  void clearData();
};

#endif 
