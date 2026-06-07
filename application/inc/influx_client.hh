/**
 * @file influx_client.hh
 * @author Michał Sadecki (michal.sadecki@proton.me)
 * @brief Plik zawierający klasę klienta bazy danych InfluxDB do wysyłania
 * statystyk sygnału.
 * @version 1.0
 * @date 2026-06-07
 *
 * @copyright Copyright (c) 2026 Michał Sadecki
 */


#ifndef INFLUX_CLIENT_HH
#define INFLUX_CLIENT_HH

#include <QObject>
#include <QNetworkAccessManager>
#include "fft_processor.hh"

/**
 * @brief Klasa odpowiedzialna za komunikację sieciową z bazą danych InfluxDB.
 * 
 * Klasa korzysta z mechanizmów REST/HTTP do asynchronicznego przesyłania
 * statystyk sygnału audio na zdalny serwer.
 */
class InfluxClient : public QObject {
  Q_OBJECT
  QNetworkAccessManager *manager;  /**< Menedżer dostępu do sieci obsługujący żądania HTTP. */
  QString url;                     /**< Adres URL serwera InfluxDB. */
  QString token;                   /**< Token autoryzacyjny dostępu do bazy InfluxDB. */
public:
  /**
   * @brief Konstruktor klasy InfluxClient.
   * @param[in] parent -- wskaźnik na obiekt nadrzędny (domyślnie nullptr).
   */
  explicit InfluxClient(QObject *parent = nullptr);

  /**
   * @brief Wysyła paczkę statystyk audio do bazy danych.
   * @param[in] stats -- struktura @ref AudioStats zawierająca przetworzone statystyki do wysłania.
   */
  void sendStats(const AudioStats &stats);
};

#endif // INFLUX_CLIENT_HH
