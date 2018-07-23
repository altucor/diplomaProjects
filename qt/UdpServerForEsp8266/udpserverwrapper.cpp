#include "udpserverwrapper.h"

UdpServerWrapper::UdpServerWrapper(QObject * parent)
    : QObject(parent)
{
    //
}

UdpServerWrapper::~UdpServerWrapper()
{
    m_socket->close();
}

bool UdpServerWrapper::init()
{
    m_socket = new QUdpSocket(this);

    if( !m_socket->bind(QHostAddress("192.168.0.2"), m_port) ){
        qDebug() << "Failed socket binding";
        return false;
    }

    bool connectionResult = connect(m_socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    if(connectionResult){
        qDebug() << "Successfully connected signal and slot";
    } else {
        qDebug() << "Error connecting signal and slot";
    }

    qDebug() << "IsOpen? : " + (m_socket->isOpen() ? QString("True") : QString("False"));
    qDebug() << "IsReadable? : " + (m_socket->isReadable() ? QString("True") : QString("False"));
    qDebug() << "IsWritable? : " + (m_socket->isWritable() ? QString("True") : QString("False"));

    qDebug() << m_socket->errorString();

    return true;
}

void UdpServerWrapper::readyRead()
{
    m_buffer.clear();
    while (m_socket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = m_socket->receiveDatagram();
        m_buffer += datagram.data();
    }

    emit processNewPacket(m_buffer);
}

QByteArray UdpServerWrapper::getData()
{
    QByteArray buff = m_buffer;
    m_buffer.clear();
    return buff;
}

void UdpServerWrapper::write(QByteArray data)
{
    m_socket->writeDatagram(data, QHostAddress::LocalHost, m_port);
}
