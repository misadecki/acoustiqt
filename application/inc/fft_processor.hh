/**
 * @file fft_processor.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Implementacja zasadniczych obliczeń dla próbek odbieranego sygnału.
 * Wyznaczanie FFT i podstawowych parametrów.
 * @version 0.5
 * @date 2026-05-12
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef FFT_PROCESSOR_H__
#define FFT_PROCESSOR_H__

#include <QObject> 
#include <fftw3.h>
#include <QList>

/**
* @struct AudioStats
* @brief Zbiór statystyk opisujących charakterystykę czasową i częstotliwościową sygnału.
 * 
 * Struktura przechowuje wyniki analizy wykonanej przez FFTProcessor dla pojedynczej 
 * ramki danych audio.
*/
struct AudioStats {
  /** 
    * @brief Wartość szczytowa amplitudy (Peak).
    * Wyznaczana jako maksymalna wartość bezwzględna próbki w ramce.
    * Zakres: od 0.0 do @ref AudioConfig::max_int32_t.
    */
  double peak;

  /**
    * @brief Wartość skuteczna sygnału (RMS - Root Mean Square).
    * Określa średnią moc sygnału w analizowanym oknie czasowym.
    */
  double rms;

  /**
    * @brief Częstotliwość dominująca.
    * Częstotliwość prążka o najwyższej amplitudzie wyznaczona z widma FFT.
    * Wyrażona w hercach [Hz].
    */
  double dominant_freq;

  /**
    * @brief Współczynnik przejść przez zero (ZCR - Zero Crossing Rate).
    * Liczba zmian znaku sygnału w obrębie jednej ramki. 
    * Pozwala na wstępne odróżnienie szumu od sygnałów sinusoidalnych.
    */
  uint16_t zcr;

  double dbfs;
};

/**
 * @class FFTProcessor
 * @brief Wyznacza FFT oraz podstawowe parametry sygnału.
 *
 * Obliczane parametry to: RMS [dBFS], ZCR, dominująca częstotliwość [Hz], wartość
 * szczytowa [dBFS].
 */
class FFTProcessor : public QObject {
  Q_OBJECT 
  fftw_plan fft_plan;        /**< @brief Struktura biblioteki FFTW definiująca optymalny plan obliczeń. */
  double *fft_in;            /**< @brief Bufor wejściowy dla algorytmu FFT. */
  fftw_complex *fft_out;     /**< @brief Bufor wyjściowy dla algorytmu FFT. */
  AudioStats stats;          /**< @brief Struktura przechowująca aktualne statystyki sygnału. */
  double volume_gain = 1.0;  /**< @brief Mnożnik wzmocnienia głośności sygnału wejściowego. */

  /**
   * @brief Oblicza wartość średnią (składową stałą) z podanej próbki sygnału.
   * * @param[in] raw_samples -- lista surowych próbek audio.
   * @return double -- wartość średnia. Zwraca 0.0, jeśli lista jest pusta.
   */
  double calculateMean(const QList<int32_t> &raw_samples);

  /**
  * @brief Oblicza widmo aplitudowe dla wartości z FFT.
  * 
  * @return QList<double> -- lista z wartościami amplitud.
  */
  QList<double> calculateMag();

  /**
  * @brief Oblicza dominującą częstotliwość w sygnale [Hz].
  * @param[in] magnitudes -- referencja na listę z amplitudami.
  * @return double -- maksymalna częstotliwość sygnału
  */
  double calculateDominantFreq(const QList<double> &magnitudes);

  /**
  * @brief Oblicza liczbę przecięć zera dla zebranych próbek.
  * @param[in] raw_samples -- referencja na listę z surowymi próbkami.
  * @return uint16_t -- liczba przecięć zera.
  */
  uint16_t calculateZCR(const QList<int32_t> &raw_samples);

  /**
  * @brief Oblicza śrędnią kwadratową z sygnału w [dBFS].
  * @param[in] raw_samples -- referencja na listę z surowymi próbkami.
  * @return double -- obliczona średnia kwadratowa.
  */
  double calculateRMS(const QList<int32_t> &raw_samples);

  /**
  * @brief Oblicza wartość szczytową amplitudy sygnału w [dBFS].
  * @param[in] raw_samples -- referencja na listę z surowymi próbkami.
  * @return double -- obliczona wartość szczytowa.
  */
  double calculatePeak(const QList<int32_t> &raw_samples);
public:
  /**
  * @brief Konstruktor inicjalizujący potrzebne elementy.
  * @param[in] parent -- wskaźnik na rodzica obiektu.
  */
  explicit FFTProcessor(QObject *parent = nullptr);

  /**
   * @brief Oblicza poziom głośności sygnału w decybelach na podstawie wartości RMS.
   *
   * @param[in] raw_samples -- referencja na listę z surowymi próbkami sygnału.
   * @return double -- poziom głośności sygnału w [dBFS].
   */
  double getDecibels(const QList<int32_t> &raw_samples); 

  /**
  * @brief Destruktor domyślny klasy FFTProcessor.
  *
  * Dealokuje wszystkie dynamicznie zaalokowane elementy.
  */
  ~FFTProcessor();
public slots:

  /**
  * @brief Główny slot procesora, który wykonuje zasadnicze obliczenia na surowych próbkach.
  *
  * Oblicza FFT dla paczki z surowymi danymi, oblicza parametry sygnału i wysyła
  * sygnały o gotowości danych.
  * @param[in] raw_samples -- referencja na listę z surowymi danymi.
  */
  void handleRawAudio(const QList<int32_t> &raw_samples);

  /**
   * @brief Ustawia wzmocnienie sygnału na połowę wartości (wartość domyślna).
   *
   * @param[in] value -- wartość ustawiana na suwaku bądź spinboxie w aplikacji.
   */
  inline void setVolumeGain(uint8_t value) {this->volume_gain = value / 50.0;};
signals:
  /**
  * @brief Sygnał wysyłany, gdy widmo zostanie poprawnie obliczone.
  * @param[in] magnitudes -- referencja na listę z widmem amplitudowym.
  */
  void spectrumReady(const QList<double> &magnitudes);

  /**
  * @brief Sygnał wysyłany, gdy statystyki zostaną poprawnie obliczone.
  * @param[in] stats -- referencja na strukturę z obliczonymi statystykami.
  */
  void statsReady(const AudioStats &stats);
};

#endif 
