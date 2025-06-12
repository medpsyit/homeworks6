#include "tcpclient.h"

QDataStream &operator >>(QDataStream &out, ServiceHeader &data){

    out >> data.id;
    out >> data.idData;
    out >> data.status;
    out >> data.len;
    return out;
};
QDataStream &operator <<(QDataStream &in, ServiceHeader &data){

    in << data.id;
    in << data.idData;
    in << data.status;
    in << data.len;

    return in;
};

QDataStream &operator >>(QDataStream &out, StatServer &stat){

    out >> stat.incBytes;
    out >> stat.sendBytes;
    out >> stat.revPck;
    out >> stat.sendPck;
    out >> stat.workTime;
    out >> stat.clients;

    return out;
};
QDataStream &operator <<(QDataStream &in, StatServer &stat){

    in << stat.incBytes;
    in << stat.sendBytes;
    in << stat.revPck;
    in << stat.sendPck;
    in << stat.workTime;
    in << stat.clients;

    return in;
};

TCPclient::TCPclient(QObject *parent) : QObject(parent)
{   
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead, this, &TCPclient::ReadyRead);

    connect(socket, &QTcpSocket::connected, this, [&](){
        emit sig_connectStatus(STATUS_SUCCESS);
    });
    connect(socket, &QTcpSocket::errorOccurred, this, [&](){
        emit sig_connectStatus(ERR_CONNECT_TO_HOST);
    });

    connect(socket, &QTcpSocket::disconnected, this, &TCPclient::sig_Disconnected);
}

void TCPclient::SendRequest(ServiceHeader head)
{

    QByteArray data;
    QDataStream outStream(&data, QIODevice::WriteOnly);
    outStream << head;

    socket->write(data);

}

void TCPclient::SendData(ServiceHeader head, QString str)
{
    QByteArray data;
    QDataStream outStream(&data, QIODevice::WriteOnly);
    outStream << head;
    outStream << str;

    socket->write(data);

}

void TCPclient::ConnectToHost(QHostAddress host, uint16_t port)
{
    socket->connectToHost(host, port);
}

void TCPclient::DisconnectFromHost()
{
    socket->disconnectFromHost();
}

void TCPclient::ReadyRead()
{

    QDataStream incStream(socket);

    if(incStream.status() != QDataStream::Ok){
        QMessageBox msg;
        msg.setIcon(QMessageBox::Warning);
        msg.setText("Ошибка открытия входящего потока для чтения данных!");
        msg.exec();
    }

    while(incStream.atEnd() == false){
        if(servHeader.idData == 0){
            if(socket->bytesAvailable() < sizeof(ServiceHeader)){
                return;
            }
            else {
                incStream >> servHeader;
                if(servHeader.id != ID){
                    uint16_t hdr = 0;
                    while(incStream.atEnd()){
                        incStream >> hdr;
                        if(hdr == ID){
                            incStream >> servHeader.idData;
                            incStream >> servHeader.status;
                            incStream >> servHeader.len;
                            break;
                        }
                    }
                }
            }
        }
        if(socket->bytesAvailable() < servHeader.len){
            return;
        }
        else {
            ProcessingData(servHeader, incStream);
            servHeader.idData = 0;
            servHeader.status = 0;
            servHeader.len = 0;
        }
    }
}

void TCPclient::ProcessingData(ServiceHeader header, QDataStream &stream)
{

    switch (header.idData){

        case GET_TIME: {

            QDateTime time;
            stream >> time;
            emit sig_sendTime(time);
            break;

        }
        case GET_SIZE: {

            uint32_t size;
            stream >> size;
            emit sig_sendFreeSize(size);
            break;

        }
        case GET_STAT: {

            StatServer stat;
            stream >> stat;
            emit sig_sendStat(stat);
            break;
        }
        case SET_DATA: {

            if (header.status == ERR_NO_FREE_SPACE) {
                emit sig_Error(ERR_NO_FREE_SPACE);
            } else {
                QString str;
                stream >> str;
                emit sig_SendReplyForSetData(str);
            }
            break;

        }
        case CLEAR_DATA: {

            emit sig_Success(CLEAR_DATA);
            break;
        }
        default:
            return;
        }

}
