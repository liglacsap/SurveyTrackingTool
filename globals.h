#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <array>
#include <QVector3D>
#include <QProcess>


extern QVector3D handPalmPosition;
extern std::vector<QVector3D> handFingers;
extern double fingerRadius;
extern unsigned int markerCount;

extern std::vector<QVector3D> palmPositions;
extern std::vector<std::vector<QVector3D> > handFingersVector;






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
