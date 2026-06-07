/**
 * @file protocol.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Definicja protokołu komunikacyjnego przesyłu danych audio przez UDP.
 * 
 * Plik zawiera definicję ramki danych oraz stałe konfiguracyjne niezbędne
 * do poprawnej serializacji i deserializacji pakietów.
 * @version 0.5
 * @date 2026-05-08
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef PROTOCOL_HH
#define PROTOCOL_HH

#include <cstdint>
#include <stddef.h>

/**
 * @namespace Protocol
 * @brief Przestrzeń nazw zawierająca parametry techniczne protokołu.
 */
namespace Protocol {
  /**
   * @brief Numer portu UDP wykorzystywanego do transmisji danych.
   */
  inline static constexpr uint16_t PORT = 42069;

  /**
   * @brief Numer portu UDP wykorzystywy do rozgłoszenia.
   */
  inline static constexpr uint16_t DISCOVERY_PORT = 5555;

  /**
   * @brief Bajty startowe identyfikujące początek ramki: 0xDEAD.
   */
  inline static constexpr uint16_t START_BYTES = 0xDEAD;

  /**
   * @brief Liczba próbek audio typu int32_t przesyłanych w pojedynczej ramce.
   */
  inline static constexpr uint16_t SAMPLES_PER_PACKET = 256;

  /**
   * @brief Wielomian używany do obliczeń sumy kontrolnej CRC8.
   */
  inline static constexpr uint8_t POLYNOMIAL = 0x07;

  /**
   * @brief Wartość początkowa rejestru CRC.
   */
  inline static constexpr uint8_t INIT_VAL = 0x00;

  /**
    * @brief Oblicza 8-bitową sumę kontrolną (CRC8) dla podanego bufora.
    * 
    * @param[in] data -- wskaźnik na dane, dla których liczone jest CRC.
    * @param[in] len -- długość danych w bajtach.
    * @param[in] poly -- wielomian generujący.
    * @param[in] init_val -- wartość inicjalizująca.
    * @return uint8_t -- obliczona suma kontrolna.
    */
  uint8_t calculate_crc8(const unsigned char* data, uint16_t len,
                                    uint8_t poly, uint8_t init_val);
}

#pragma pack(push, 1)
/**
 * @struct AudioFrame
 * @brief Struktura ramki binarnej przesyłanej po sieci.
 * 
 * Struktura jest upakowana do jednego bajtu, aby zapewnić zgodność binarną
 * między różnymi architekturami systemowymi.
 */
struct AudioFrame {
  /**
   * @brief Znacznik początku ramki. Powinien wynosić @ref Protocol::START_BYTES.
   */
  uint16_t start;

  /**
   * @brief Numer sekwencyjny pakietu (ułatwia wykrywanie strat).
   */
  uint8_t count;

  /**
   * @brief Tablica próbek audio w formacie 32-bitowym.
   */
  int32_t samples[Protocol::SAMPLES_PER_PACKET];

  /**
    * @brief Suma kontrolna CRC8 obejmująca pola: start, count oraz samples.
    */
  uint8_t crc;
};
#pragma pack(pop)

#endif 
