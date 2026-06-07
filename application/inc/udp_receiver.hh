/**
 * @file udp_receiver.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Implementacja komunikacji WiFi UDP po stronie aplikacji Qt.
 * @version 0.5
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
  QUdpSocket *udp_socket;  /**< @brief Gniazdo UDP wykorzystywane do komunikacji sieciowej. */
  QTimer *watchdog;
  bool isConnected = false;
  Q_OBJECT 
public:
  /**
   * @brief Konstruktor, który inicjalizuje gniazdo na porcie, wiąże je i przygotowuje pod odbiór danych.
   *
   * @param[in] port -- port, na którym odbywa się komunikacja
   * @param[in] parent -- wskaźnik na rodzica obiektu
   */
  UdpReceiver(uint16_t port, QObject * parent = nullptr);

  bool isCurrentlyConnected() const {return isConnected; }
signals:
  /**
  * @brief Sygnał, który informuje, że odebrano dane.
  *
  * @param[out] samples -- referencja na listę próbek, które odebrano.
  */
  void audioDataReceived(const QList<int32_t>& samples);

  void connectionRestored();
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
  void onWatchdogTimeout();
};

#endif
