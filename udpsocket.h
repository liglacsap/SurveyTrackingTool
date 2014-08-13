#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include <QObject>
#include <QUdpSocket>
#include <QtNetwork>

#include <string>
#include <ctime>

#include "globals.h"

using namespace std;

class UDPSocket : public QObject
{
    Q_OBJECT
public:
    explicit UDPSocket(QObject *parent = 0);

    void write(string message);

signals:

public slots:
    void read();

private:
    QUdpSocket* socket;
    QUdpSocket* sendSocket;
};

#endif // UDPSOCKET_H
