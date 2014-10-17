#ifndef TRACKING_H
#define TRACKING_H

#ifdef _WIN32
    #include <NatNetTypes.h>
    #include <NatNetClient.h>
#endif

#include <stdio.h>
#include <string.h>

#include <QVector3D>
#include <QProcess>
#include <QDebug>

#include "csvfilehandler.h"

#define RIGHT_HAND_STRING "RightHand"
#define LEFT_HAND_STRING "LeftHand"

#define FEEDBACK_EMS "EMS"
#define FEEDBACK_REAL "REAL"

using namespace std;

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



    /**
    * @brief Returns true if the x coordinate from QVector3D v1 is greater than the x coordinate of QVector3D v2
    * Neaded for sorting all fingers (each finger is a QVector3D) of the left hand.
    * @param v1 Vector of type QVector3D
    * @param v2 Vector of type QVector3D
    * @return bool
    */
   inline bool vectorXGreaterThan(const QVector3D &v1, const QVector3D &v2){

       return v1.x() > v2.x();
   }

   /**
    * @brief Returns true if the x coordinate from QVector3D v1 is smaller than the x coordinate of QVector3D v2.
    * Neaded for sorting all fingers (each finger is a QVector3D) of the right hand.
    * @param v1 Vector of type QVector3D
    * @param v2 Vector of type QVector3D
    * @return bool
   */
   inline bool vectorXLessThan(const QVector3D &v1, const QVector3D &v2){
       return v1.x() < v2.x();
   }


   /**
    * @brief Calculates the center of a circle in a 3D Space. Needs three different points as parameter.
    * @param Point 1
    * @param Point 2
    * @param Point 3
    * @return The center of type QVector3D
    */
   inline QVector3D calculateCircleCenter(QVector3D p1, QVector3D p2, QVector3D p3){
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



   /**
    * @brief Calculates the radius of a circle based on three points of type QVector3D
    * @param p1
    * @param p2
    * @param p3
    * @return the radius of the circle as double
    */
   inline double calculateCircleRadius(QVector3D p1, QVector3D p2, QVector3D p3){
       QVector3D t = p2 - p1;
       QVector3D u = p3 - p1;
       QVector3D v = p3 - p2;

       // Formel von Wikipedia
       double radius = (t.length() * v.length() * u.length()) /
           (2 * (QVector3D::crossProduct(t, v)).length());

       return 2*radius-1.0f;
   }

   /**
    * @brief Returns true if the x coordinate from QVector3D v1 is smaller than the x coordinate of QVector3D v2.
    * Neaded for sorting all fingers (each finger is a QVector3D).
    * @param v1 Vector of type QVector3D
    * @param v2 Vector of type QVector3D
    * @return bool
    */
    inline bool distanceLowerToThumb(IndexedFingerForThumbOrdering finger1, IndexedFingerForThumbOrdering finger2){
        return finger1.position.distanceToPoint(finger1.thumb) < finger2.position.distanceToPoint(finger2.thumb);
    }











class Tracking : public QObject
{
    Q_OBJECT

    // Singleton Pattern Implementation
private:
    NatNetClient* client;

    CapturedHand capturedHand;

    QList<NamedRigidBody> namedRigidBodies;

    QVector3D thumb;

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
