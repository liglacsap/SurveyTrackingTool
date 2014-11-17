#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QVector3D>
#include <QList>
#include <QString>


struct CapturedHand{
    QVector3D palmPosition;
    QList<QVector3D> fingers;

    double fingerRadius;
    QString type;
};

struct CapturedCondition{
    CapturedHand hand;

    QString feedback;
    int intensity;
    int condition;
    int run;
    int user;

    double ballSize;
    int ballIndex;
    qint64 time;
    int userSelectedBall = -1;
};


struct NamedRigidBody{
    char* name;
    int id;
};


struct Condition{
    double size;
};



struct IndexedFingerForThumbOrdering{
    QVector3D position;
    QVector3D thumb;
};


#endif // STRUCTURES_H
