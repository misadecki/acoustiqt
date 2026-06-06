#ifndef INFLUX_CLIENT_HH
#define INFLUX_CLIENT_HH

#include <QObject>
#include <QNetworkAccessManager>
#include "fft_processor.hh"

class InfluxClient : public QObject {
  Q_OBJECT
  QNetworkAccessManager *manager;
  QString url, token;
public:
  explicit InfluxClient(QObject *parent = nullptr);
  void sendStats(const AudioStats &stats);
};

#endif // INFLUX_CLIENT_HH
