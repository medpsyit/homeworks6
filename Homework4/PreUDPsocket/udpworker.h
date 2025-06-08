#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket( void );
    void ReadDatagram( QNetworkDatagram datagram);
    void SendDatagram(QByteArray data );

    void ReadSentDatagram (QNetworkDatagram dg);
    bool time_on = false;
    bool text_on = false;
private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;

signals:
    void sig_sendTimeToGUI(QDateTime data);

    void sig_sendDataToGUI(qint64 bytes, QHostAddress sender, QString text);
};

#endif // UDPWORKER_H
