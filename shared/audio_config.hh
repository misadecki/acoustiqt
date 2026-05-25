/**
 * @file audio_config.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Definicje stałych konfiguracyjnych dla modułu przetwarzania dźwięku.
 * 
 * Plik zawiera parametry dotyczące próbkowania, normalizacji sygnału
 * oraz ustawienia wizualizacji widma.
 * @version 0.5
 * @date 2026-05-12
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef AUDIO_CONFIG_HH__
#define AUDIO_CONFIG_HH__

#include <cstdint>
#include <limits>
#include "protocol.hh"

/**
 * @namespace AudioConfig
 * @brief Przestrzeń nazw grupująca stałe parametry systemu audio.
 */
namespace AudioConfig {
  /**
   * @brief Częstotliwość próbkowania sygnału audio wyrażona w [Hz].
   */
  static constexpr uint16_t SAMPLE_RATE = 16000;

  /**
   * @brief Liczba próbek w jednym oknie analizy (paczce danych).
   */
  static constexpr uint16_t N = Protocol::SAMPLES_PER_PACKET;

  /**
   * @brief Maksymalna wartość dla typu int32_t używana do normalizacji próbek.
   */
  static constexpr double MAX_INT32_T = std::numeric_limits<int32_t>::max();

  /**
   * @brief Maksymalna wartość dla typu uint8_t, używana przy mapowaniu kolorów lub wysokości.
   */
  static constexpr double MAX_UINT8_T = std::numeric_limits<uint8_t>::max();

  /** 
   * @brief Wysokość wyświetlanego widma (liczba prążków).
   * Wynika z symetrii FFT (N/2).
   */
  static constexpr uint8_t HEIGHT = Protocol::SAMPLES_PER_PACKET / 2;

  /**
    * @brief Liczba historycznych ramek przechowywanych dla spektrogramu.
    */
  static constexpr uint16_t HISTORY_LEN = 200; 
  /**
    * @brief Mała wartość zapobiegająca dzieleniu przez zero w obliczeniach logarytmicznych.
    */
  static constexpr double EPSILON = 1e-6;

  /** 
   * @brief Próg odcięcia szumu wyrażony w decybelach [dB].
  */ 
  static constexpr double NOISE_THRESHOLD = 100.0;
};

#endif 
