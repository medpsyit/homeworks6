#include "udpworker.h"

UDPworker::UDPworker(QObject *parent) : QObject(parent)
{

}

void UDPworker::InitSocket()
{

    serviceUdpSocket = new QUdpSocket(this);
    textUdpSocket = new QUdpSocket(this);

    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    textUdpSocket->bind(QHostAddress::LocalHost, TEXT_PORT);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(textUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readTextPendingDatagrams);

}

void UDPworker::ReadSentDatagram (QNetworkDatagram dg) {

    QByteArray buffer;

    qint64 sended_bytes;
    QHostAddress sender;
    QString text;

    buffer = dg.data();

    sended_bytes = buffer.size();

    sender = dg.senderAddress();

    QDataStream inStr(&buffer, QIODevice::ReadOnly);

    inStr >> text;

    emit sig_sendDataToGUI(sended_bytes, sender, text);
}

void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{

    QByteArray data;
    data = datagram.data();


    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendTimeToGUI(dateTime);
}

void UDPworker::SendDatagram(QByteArray data)
{
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::SendTextDatagram(QByteArray data) {
    textUdpSocket->writeDatagram(data, QHostAddress::LocalHost, TEXT_PORT);
}

void UDPworker::readPendingDatagrams( void )
{

    while(serviceUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
            ReadDatagram(datagram);
    }

}

void UDPworker::readTextPendingDatagrams( void )
{

    while(textUdpSocket->hasPendingDatagrams()){
            QNetworkDatagram datagram = textUdpSocket->receiveDatagram();
                ReadSentDatagram(datagram);
    }

}

