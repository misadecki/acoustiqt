#include "udp_receiver.hh"
#include "protocol.hh"
#include <QNetworkDatagram>
#include <QDebug>

UdpReceiver::UdpReceiver(uint16_t port, QObject * parent) : QObject(parent) {
  udp_socket = new QUdpSocket(this);

  if (udp_socket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress |
                       QUdpSocket::ReuseAddressHint)) {
    qDebug() << "Listening UDP on port: " << port;
  } else {
    qCritical() << "UdpReceiver: error in binding port.";
  }

  connect(udp_socket, &QUdpSocket::readyRead, this,
          &UdpReceiver::readPendingDatagrams);

  watchdog = new QTimer(this);
  watchdog->setInterval(1500);
  connect(watchdog, &QTimer::timeout, this, &UdpReceiver::onWatchdogTimeout);
  watchdog->start();
}

void UdpReceiver::readPendingDatagrams() {
  while (udp_socket->hasPendingDatagrams()) {
    watchdog->start();

    if (!isConnected) {
      isConnected = true;
      emit connectionRestored();
    }

    QByteArray datagram;
    datagram.resize(static_cast<int>(udp_socket->pendingDatagramSize()));

    udp_socket->readDatagram(datagram.data(), datagram.size());

    if (datagram.size() != sizeof(AudioFrame)) {
      qWarning() << "Bad frame size. Received:" << datagram.size() <<
        "Expect:" << sizeof(AudioFrame);
      continue;
    }

    const AudioFrame* frame = reinterpret_cast<const
      AudioFrame*>(datagram.constData());

    if (frame->start != Protocol::START_BYTES) {
      qWarning() << "Bad start byte.";
      continue;
    }

    uint8_t expected_crc = Protocol::calculate_crc8(reinterpret_cast<const unsigned
                                          char*>(frame), sizeof(AudioFrame) - 1,
                                          Protocol::POLYNOMIAL, Protocol::INIT_VAL);

    if (expected_crc != frame->crc) {
      qWarning() << "Bad CRC. Received:" << Qt::hex << frame->crc << "Calculated:"
        << expected_crc;
      continue;
    }

    QList<int32_t> samples_list;
    samples_list.resize(Protocol::SAMPLES_PER_PACKET);
    std::copy(std::begin(frame->samples), std::end(frame->samples),
              samples_list.begin());
    // qDebug() << "Otrzymano dane: " << samples_list.size();
    emit audioDataReceived(samples_list);
  }
}

void UdpReceiver::onWatchdogTimeout() {
  if (isConnected) {
    isConnected = false;
    emit connectionLost();
  }
}
