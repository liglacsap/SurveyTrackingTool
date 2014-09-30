#ifndef CAPTUREDHANDDATA_H
#define CAPTUREDHANDDATA_H

#include <QObject>
#include <QString>
#include <QVector3D>
#include <QVector>
#include <QTime>
#include <QDebug>


#include "csvfilehandler.h"

struct CapturedHand{
    QVector3D palmPosition;
    QList<QVector3D> fingers;
    int emsIntensity;

    double fingerRadius;
    QString type;

    int rigidBodyId;
};


class CapturedHandDataListener : public QObject
{
    Q_OBJECT
private:
    QList<CapturedHand> capturedData;
public:
    explicit CapturedHandDataListener(QObject *parent = 0);

    void clear();
    void save();
    void appendData(CapturedHand hand);
signals:
    void dataAdded(CapturedHand* hand);
    void valueChanged(int);
public slots:

};

#endif // CAPTUREDHANDDATA_H
