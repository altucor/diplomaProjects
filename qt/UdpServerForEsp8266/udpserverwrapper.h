#ifndef UDPSERVERWRAPPER_H
#define UDPSERVERWRAPPER_H

#include <QUdpSocket>
#include <QNetworkDatagram>

class UdpServerWrapper : public QObject
{
    Q_OBJECT

public:
    explicit UdpServerWrapper(QObject *parent = 0);
    ~UdpServerWrapper();
    bool init();
    QByteArray getData();
    void write(QByteArray data);

private:
    QUdpSocket *m_socket;
    QByteArray m_buffer;
    unsigned int m_port = 27020;

signals:
    void processNewPacket(QByteArray data);

public slots:
    void readyRead();
};

#endif // UDPSERVERWRAPPER_H
