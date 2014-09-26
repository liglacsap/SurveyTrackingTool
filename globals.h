#ifndef GLOBALS_H
#define GLOBALS_H

#include <vector>
#include <array>
#include <string>
#include <QVector3D>
#include <QProcess>
#include <QString>

using namespace std;

struct Take{
    float hardness;
    float size;
    QString name;
};



extern QVector3D handPalmPosition;
extern std::vector<QVector3D> handFingers;
extern double fingerRadius;
extern unsigned int markerCount;

extern std::vector<QVector3D> palmPositions;
extern std::vector<std::vector<QVector3D> > handFingersVector;

extern vector<Take> takes;



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


double inline calculateCircleRadius(QVector3D p1, QVector3D p2, QVector3D p3){
    QVector3D t = p2 - p1;
    QVector3D u = p3 - p1;
    QVector3D v = p3 - p2;

    // Formel von Wikipedia
    double radius = (t.length() * v.length() * u.length()) /
        (2 * (QVector3D::crossProduct(t, v)).length());

    return radius;
}

/**
 * @brief Calculates the center of a circle in a 3D Space. Needs three different points as parameter.
 * @param Point 1
 * @param Point 2
 * @param Point 3
 * @return The center of type QVector3D
 */
QVector3D inline calculateCircleCenter(QVector3D p1, QVector3D p2, QVector3D p3){
    QVector3D t = p2 - p1;
    QVector3D u = p3 - p1;
    QVector3D v = p3 - p2;

    // Helfer
    double nenner = 2.0 * (QVector3D::crossProduct((p1 - p2), (p2 - p3))).lengthSquared();


    double a = v.lengthSquared() * QVector3D::dotProduct((p1 - p2), (p1 - p3)) / nenner;
    double b = u.lengthSquared() * QVector3D::dotProduct((p2 - p1), (p2 - p3)) / nenner;
    double g = t.lengthSquared() * QVector3D::dotProduct((p3 - p1), (p3 - p2)) / nenner;

    return a * p1 + b * p2 + g * p3;
}



#endif // GLOBALS_H
