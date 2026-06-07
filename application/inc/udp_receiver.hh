/**
 * @file udp_receiver.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Implementacja komunikacji WiFi UDP po stronie aplikacji Qt.
 * @version 1.0
 * @date 2026-04-30
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */

#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QList>
#include <QTimer>

/**
 * @class UdpReceiver
 * @brief Przeprowadza odbiór datagramów po stronie aplikacji Qt.
 *
 * Klasa implementuje odbiór danych, rozpakowywanie ich, a po poprawnym procesie
 * wysyła sygnał o nowych danych do przetwarzania.
 */
class UdpReceiver : public QObject {
  QUdpSocket *udp_socket;     /**< Gniazdo UDP wykorzystywane do komunikacji sieciowej. */
  QTimer *watchdog;           /**< Wskaźnik na timer pełniący rolę strażnika, monitorujący ciągłość odbieranych pakietów. */
  bool isConnected = false;   /**< Flaga przechowująca obecny stan połączenia
  sieciowego (true = obecne, false = brak). */
  Q_OBJECT 
public:
  /**
   * @brief Konstruktor, który inicjalizuje gniazdo na porcie, wiąże je i przygotowuje pod odbiór danych.
   *
   * @param[in] port -- port, na którym odbywa się komunikacja
   * @param[in] parent -- wskaźnik na rodzica obiektu
   */
  UdpReceiver(uint16_t port, QObject * parent = nullptr);

  /**
   * @brief Zwraca bieżący status połączenia z zewnętrznym źródłem danych (np. ESP32).
   * @return bool -- true, jeśli połączenie jest aktywne i dane napływają na
   * bieżąco, w przeciwnym razie false.
   */
  bool isCurrentlyConnected() const {return isConnected; }
signals:
  /**
  * @brief Sygnał, który informuje, że odebrano dane.
  *
  * @param[out] samples -- referencja na listę próbek, które odebrano.
  */
  void audioDataReceived(const QList<int32_t>& samples);

  /**
   * @brief Sygnał emitowany, gdy aplikacja odbierze pakiet z danymi po wcześniejszym stanie rozłączenia.
   */
  void connectionRestored();

  /**
   * @brief Sygnał emitowany, gdy aplikacja przestanie otrzymywać pakiety i minie założony czas bezczynności.
   */
  void connectionLost();

private slots:
  /**
  * @brief Odbiera datagramy UDP i wysyła sygnał po ich poprawnym odebraniu.
  *
  * Po odebraniu datagramu są sprawdzane bajty startu oraz suma kontrolna CRC8.
  * Dla poprawnie odebranej ramki dane są kopiowane do listy i wysyłany jest
  * sygnał o odebraniu danych.
  */
  void readPendingDatagrams();

  /**
   * @brief Slot wywoływany w momencie, gdy czasomierz watchdoga dojdzie do zera.
   * 
   * Uruchomienie tego slotu oznacza brak nowych danych UDP w wymaganym oknie czasowym,
   * co skutkuje zmianą stanu `isConnected` i emisją sygnału @ref connectionLost().
   */
  void onWatchdogTimeout();
};

#endif
