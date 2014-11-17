#include "../ems/udpsocket.h"

UDPSocket::UDPSocket(QObject *parent) :
    QObject(parent)
{
    socket = new QUdpSocket(this);
    sendSocket = new QUdpSocket(this);
    sendSocket->connectToHost("192.168.3.8", 2000);

    socket->bind((quint16)454545, QUdpSocket::ShareAddress);

    connect(socket, SIGNAL(readyRead()), this, SLOT(read()));
}

void UDPSocket::write(string message){
    qDebug() << QString::fromStdString(message);
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
