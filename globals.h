#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <array>
#include <string>
#include <QVector3D>
#include <QProcess>
#include <QString>
#include <QDebug>

using namespace std;

struct Condition{
    float hardness;
    float size;
    QString name;

    inline bool operator == (const Condition &b) const{
        return (hardness == b.hardness) &&
                (size == b.size) &&
                (name == b.name);
    }
};

struct CapturedHand{
    QVector3D palmPosition;
    QList<QVector3D> fingers;
    int emsIntensity;

    double fingerRadius;
    QString type;

    int condition;
    int run;
    int user;
    int userSelectedBall = -1;
};


struct CapturedConditionHandData{
    QList<CapturedHand> hands;
};

extern CapturedHand capturedHand;



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
