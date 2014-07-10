#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <QVector3D>


extern QVector3D handPalmPosition;
extern std::vector<QVector3D> handFingers;
extern double fingerRadius;
extern unsigned int markerCount;

struct Ball{
    QVector3D position;
    double radius;
    QColor color;
};

#endif // GLOBALS_H
