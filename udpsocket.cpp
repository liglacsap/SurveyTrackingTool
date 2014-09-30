#include "udpsocket.h"

UDPSocket::UDPSocket(QObject *parent) :
    QObject(parent)
{
    bool res2 = ping("192.168.3.10");
    qDebug() << res2;

    socket = new QUdpSocket(this);
    sendSocket = new QUdpSocket(this);
    sendSocket->connectToHost("192.168.3.10", 1);

    socket->bind((quint16)454545, QUdpSocket::ShareAddress);



    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
}

void UDPSocket::write(string message){
    sendSocket->write(message.c_str());
}


void UDPSocket::read(){
    while(socket->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
    }
}
