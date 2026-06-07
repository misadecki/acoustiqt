#include "influx_client.hh"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QSettings>

InfluxClient::InfluxClient(QObject *parent) : QObject(parent) {
  manager = new QNetworkAccessManager(this);

  QSettings settings("acoustiqt", "acoustiqt");

  url = settings.value("InfluxDB/Url", "").toString();
  token = settings.value("InfluxDB/Token", "").toString();

  if (token.isEmpty() || url.isEmpty()) {
    qWarning() << "No InfluxDB data in config file. Please check"
                  "~/.config/acoustiqt/acoustiqt.conf";
  }
}

void InfluxClient::sendStats(const AudioStats &stats) {
  QNetworkRequest request((QUrl(url)));
  request.setRawHeader("Authorization", token.toUtf8());
  request.setHeader(QNetworkRequest::ContentTypeHeader, "text/plain; charset=utf-8");

  qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
  QString data_line = QString("audio,device=esp32 "
                              "rms=%1,peak=%2,zcr=%3,freq=%4,dbfs=%5 %6")
                          .arg(stats.rms, 0, 'f', 4)
                          .arg(stats.peak, 0, 'f', 4)
                          .arg(stats.zcr)
                          .arg(stats.dominant_freq, 0, 'f', 2)
                          .arg(stats.dbfs, 0, 'f', 2)
                          .arg(timestamp);
  QNetworkReply *reply = manager->post(request, data_line.toUtf8());
  connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);

  connect(reply, &QNetworkReply::errorOccurred, [reply]() {
          qDebug() << "InfluxDB error: " << reply->errorString();
  });
}
