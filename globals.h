#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <array>
#include <string>
#include <QVector3D>
#include <QProcess>
#include <QString>

#include "capturedhanddata.h"


using namespace std;

struct Take{
    float hardness;
    float size;
    QString name;

    inline bool operator == (const Take &b) const{
        return (hardness == b.hardness) &&
                (size == b.size) &&
                (name == b.name);
    }
};


struct CapturedTakeHandData{
    QList<CapturedHand> hands;
};

extern CapturedHand capturedHand;


/**
 * @brief Checks wether an IP adress is pingable or note
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

    return !QProcess::execute(p);
}


#endif // GLOBALS_H
