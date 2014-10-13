#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <array>
#include <string>
#include <QVector3D>
#include <QProcess>
#include <QString>
#include <QDebug>
#include "tracking.h"

using namespace std;

struct Condition{
    float size;
};




struct CapturedConditionHandData{
    QList<CapturedHand> hands;
};

//extern CapturedHand capturedHand;


inline QList<QList <int>> shuffled(int size, int trials){
    QList<QList <int>> result;

    QList<int> values;
    for(int i=0; i<size; i++){
        values.push_back(i);
    }

    for(int i=0; i<trials; i++){
        QList<int> shuffled = values;
        for(int j=0; j<size*size; j++){
            shuffled.swap(rand() % size, rand() % size);
        }

        result.push_back(shuffled);
    }



    return result;
}

inline QList<QList <int>> latinSquare(int size){
    QList<QList <int>> result;

    for(int i=0; i<size; i++){
        QList<int> row;
        for(int j=0; j<size; j++){

            row.append((i+j) % size);
        }

        result.append(row);
    }

    return result;
}

/**
 * @brief Checks wether an IP adress is pingable or not
 * @param The IPv4 or IPv6 adress as a QString
 * @return If the IP adress is reachable (true/false)
 */
inline bool ping(QString ip)
{
    QString param;

    QString p = "ping";
#if defined(WIN32)
    p.append(" -n 1 ");

#else
    p.append(" -c 1 ");
#endif

    p.append(ip);

    QProcess proc;
    proc.start(p);
    if(!proc.waitForStarted()) return false;
    QByteArray result = proc.readAll();


    proc.waitForFinished();
    result = proc.readAllStandardOutput();
    proc.close();

    int index1 = ((QString)result).indexOf("Destination host unreachable");
    int index2 = ((QString)result).indexOf("Received = 0");
    int index3 = ((QString)result).indexOf("Lost = 1 (100% loss)");

    return (index1 == -1 && index2 == -1 && index3 == -1);
}


#endif // GLOBALS_H
