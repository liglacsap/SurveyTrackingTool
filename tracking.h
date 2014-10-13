#ifndef TRACKING_H
#define TRACKING_H

#ifdef _WIN32
    #include <NatNetTypes.h>
    #include <NatNetClient.h>
#endif

#include <stdio.h>
#include <string.h>

#include <QVector3D>
#include <QDebug>

#include "math.h"

#define RIGHT_HAND_STRING "RightHand"
#define LEFT_HAND_STRING "LeftHand"

#define FEEDBACK_EMS "EMS"
#define FEEDBACK_REAL "REAL"

using namespace std;

struct CapturedHand{
    QVector3D palmPosition;
    QList<QVector3D> fingers;
    int emsIntensity;

    double fingerRadius;
    QString type;
};

struct CapturedCondition{
    CapturedHand hand;

    int feedback;
    int condition;
    int run;
    int user;
    int userSelectedBall = -1;
};


struct NamedRigidBody{
    char* name;
    int id;
};

class Tracking : public QObject
{
    Q_OBJECT

    // Singleton Pattern Implementation
private:
    NatNetClient* client;

    CapturedHand capturedHand;

    QList<NamedRigidBody> namedRigidBodies;

    QVector3D thumb;


    int calculateIntensity();
private:
    QString getNameOfRigidBody(int id);

    QList<QVector3D> getAllMarker(sFrameOfMocapData* data, QVector3D handPalmPosition);

signals:
    void handCaptured(CapturedHand);

public:
    Tracking(){};

    static Tracking& getInstance()
    {
        static Tracking instance;

        return instance;
    }

    QVector3D calculateHandPalmPosition(sFrameOfMocapData* data);
    static void DataHandlerStatic(sFrameOfMocapData* data, void* pUserData);
    void DataHandler(sFrameOfMocapData* data, void* pUserData);

    /**
     * @brief Creates the client for the OptiTrack system.
     * @param iConnectionType
     * @return
     */
    int createClient();
};

#endif // TRACKING_H
