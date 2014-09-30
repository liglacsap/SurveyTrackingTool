
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QTranslator>
#include <QVector3D>
#include <QObject>

#include "globals.h"

#define RIGHT_HAND_STRING "RightHand"
#define LEFT_HAND_STRING "LeftHand"

CapturedHand capturedHand;
CapturedTakeHandData capturedTakeHandData;
CapturedHandDataListener* listener;
struct NamedRigidBody{
    string name;
    int id;
};

MainWindow* w;

QList<NamedRigidBody> namedRigidBodies = QList<NamedRigidBody>();

QString getNameOfRigidBody(int id){
    QListIterator<NamedRigidBody> i(namedRigidBodies);


    while (i.hasNext()){
        NamedRigidBody a = i.next();
        if(a.id == id)
                return  QString::fromStdString(a.name);
    }

    return "undefined";
}

QVector3D thumb;



#ifdef _WIN32
    NatNetClient* client;
#endif



 /**
 * @brief Returns true if the x coordinate from QVector3D v1 is greater than the x coordinate of QVector3D v2
 * Neaded for sorting all fingers (each finger is a QVector3D) of the left hand.
 * @param v1 Vector of type QVector3D
 * @param v2 Vector of type QVector3D
 * @return bool
 */
bool vectorXGreaterThan(const QVector3D &v1, const QVector3D &v2){

    return v1.x() > v2.x();
}

/**
 * @brief Returns true if the x coordinate from QVector3D v1 is smaller than the x coordinate of QVector3D v2.
 * Neaded for sorting all fingers (each finger is a QVector3D) of the right hand.
 * @param v1 Vector of type QVector3D
 * @param v2 Vector of type QVector3D
 * @return bool
*/
bool vectorXLessThan(const QVector3D &v1, const QVector3D &v2){
    return v1.x() < v2.x();
}

/**
 * @brief Returns true if the x coordinate from QVector3D v1 is smaller than the x coordinate of QVector3D v2.
 * Neaded for sorting all fingers (each finger is a QVector3D).
 * @param v1 Vector of type QVector3D
 * @param v2 Vector of type QVector3D
 * @return bool
 */
bool distanceToThumbLessThan(const QVector3D &v1, const QVector3D &v2){
    return thumb.distanceToPoint(v1) < thumb.distanceToPoint(v2);
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


/**
 * @brief Calculates the radius of a circle based on three points of type QVector3D
 * @param p1
 * @param p2
 * @param p3
 * @return the radius of the circle as double
 */
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
 * @brief Calculates the hand palm position based on the three markers of the hand rigid body
 * @param data The Data provided by the OptiTrack and received in DataHandler
 * @return The calculated hand center position as QVector3D
 */
QVector3D calculateHandPalmPosition(sFrameOfMocapData* data){
    QVector3D result = QVector3D(0, 0, 0);


    // Loop through all rigid bodies. A rigid body needs to be defined in the OptiTrack Software Motive
    for (int i = 0; i < data->nRigidBodies; i++){
        if(getNameOfRigidBody(data->RigidBodies[i].ID) == RIGHT_HAND_STRING ||
           getNameOfRigidBody(data->RigidBodies[i].ID) == LEFT_HAND_STRING){

            QList<QVector3D> marker;
            for (int j = 0; j < data->RigidBodies[i].nMarkers; j++){

                float x = data->RigidBodies[i].Markers[j][0];
                float y = data->RigidBodies[i].Markers[j][1];
                float z = data->RigidBodies[i].Markers[j][2];

                marker.push_back(QVector3D(x, y, z));
            }

            // calculate the hand palm position in cm.
            if (marker.size() > 2)
                result = calculateCircleCenter(marker[0], marker[1], marker[2]) * 100;
        }

    }

    return result;
}

/**
 * @brief Return all marker not belonging to any rigid body as a list
 * @param data The Data provided by the OptiTrack and received in DataHandler
 * @param handPalmPosition the center of hand palm. See calculateHandPalmPosition for mor information
 * @return a QList of type QVector3D
 */
QList<QVector3D> getAllMarker(sFrameOfMocapData* data, QVector3D handPalmPosition){
    QList<QVector3D> result;

    for (int i = 0; i < data->nOtherMarkers; i++){
        double x = data->OtherMarkers[i][0]*100;
        double y = data->OtherMarkers[i][1]*100;
        double z = data->OtherMarkers[i][2]*100;

        // Filter all points and only return the points which are not related to any rigid body
        bool belongsToRigidBody = false;



        for(int j=0; j<data->nRigidBodies; j++){
            if(getNameOfRigidBody(data->RigidBodies[j].ID) == "RightHand" ||
               getNameOfRigidBody(data->RigidBodies[j].ID) == "LeftHand"){
                for (int m = 0; m < data->RigidBodies[j].nMarkers; m++){
                    double _x = data->RigidBodies[j].Markers[m][0] * 100;
                    double _y = data->RigidBodies[j].Markers[m][1] * 100;
                    double _z = data->RigidBodies[j].Markers[m][2] * 100;

                    QVector3D rigid(_x, _y, _z);
                    QVector3D point(x, y, z);


                    // filter all marker belonging to a rigid body.
                    if (rigid.distanceToPoint(point) < 0.1){
                        belongsToRigidBody = true;
                        break;
                    }
                }
            }
        }


        if (!belongsToRigidBody){
            QVector3D vec = QVector3D(x, y, z);
            double distance = vec.distanceToPoint(handPalmPosition);

            /* only add points that are in a specific range. Needed to filter
             * false tracked points */
            if(distance > 1.0 && distance < 20.0){
                result.push_back(QVector3D(x, y, z));
            }
        }
    }

    return result;
}

// The Optitrack system is only available on Windows. In order to use this software with linux the values must be send via
// network connection.
#ifdef _WIN32



int calculateIntensity(){
    float ps = capturedHand.fingerRadius / w->getCurrentTake().size * 10.0f;


    int f1 =  exp(120.0f*(1-ps));

    int v = f1;
    v = (v > 127) ? 127 : v;

   if(v==INT_MIN)
    v = 127;

    return v;
}

/**
 * @brief DataHandler of the OptiTrack. Retrieves all data and calculates the hand palm position and all hand specific stuff
 * @param data The Data provided by the OptiTrack
 */
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData)
{
    NatNetClient* pClient = (NatNetClient*)pUserData;

    // disable all debug information provided by the OptiTrack system
    pClient->SetVerbosityLevel(0);

    // Return if hand palm rigid body was not detected
    if (data->nRigidBodies == 0) return;


    capturedHand.palmPosition = calculateHandPalmPosition(data);


    // handFingers is a vector to save all captured finger positions within a take
    capturedHand.fingers.clear();

    QList<QVector3D> marker = getAllMarker(data, capturedHand.palmPosition);

    QListIterator<QVector3D> i(marker);
    while (i.hasNext())
         capturedHand.fingers.push_back(i.next());


    if(capturedHand.fingers.size() >= 3){
        for(int i=0; i<data->nRigidBodies; i++){
            if(getNameOfRigidBody(data->RigidBodies[i].ID)==RIGHT_HAND_STRING){
                capturedHand.type = RIGHT_HAND_STRING;

                qSort(capturedHand.fingers.begin(), capturedHand.fingers.end(), vectorXLessThan);
                break;
            }else if(getNameOfRigidBody(data->RigidBodies[i].ID)==LEFT_HAND_STRING){
                capturedHand.type = LEFT_HAND_STRING;

                qSort(capturedHand.fingers.begin(), capturedHand.fingers.end(), vectorXGreaterThan);
                break;
            }
        }




        thumb = capturedHand.fingers[0];

        qSort(capturedHand.fingers.begin(), capturedHand.fingers.end(), distanceToThumbLessThan);

        capturedHand.fingerRadius = calculateCircleRadius(capturedHand.fingers[0], capturedHand.fingers[1], capturedHand.fingers[2]); // 5.5 is the radius of a marker
        capturedHand.fingerRadius = (capturedHand.fingerRadius > 200) ? 0 : capturedHand.fingerRadius;
        capturedHand.fingerRadius = (capturedHand.fingerRadius < 1.25) ? 1.25 : capturedHand.fingerRadius;


        capturedTakeHandData.hands.push_back(capturedHand);


        capturedHand.emsIntensity = 666; //calculateIntensity();

    }


}

/**
 * @brief Creates the client for the OptiTrack system.
 * @param iConnectionType
 * @return
 */
int createClient()
{
    // release previous server
    if (client){
        client->Uninitialize();
        delete client;
    }

    // create NatNet client
    client = new NatNetClient(ConnectionType_Multicast);


    // Set callback handlers
    client->SetDataCallback(DataHandler, client);	// this function will receive data from the server

    int retCode = client->Initialize("127.0.0.1", "127.0.0.1");
    if (retCode != ErrorCode_OK){
        return ErrorCode_Internal;
    }else {
        sServerDescription ServerDescription;
        memset(&ServerDescription, 0, sizeof(ServerDescription));
        client->GetServerDescription(&ServerDescription);
        if (!ServerDescription.HostPresent)
        {
            printf("Unable to connect to server. Host not present. Exiting.");
            return 1;
        }
    }


    sDataDescriptions* pDataDefs = NULL;
    client->GetDataDescriptions(&pDataDefs);

    for (int i=0;i<pDataDefs->nDataDescriptions;i++)
    {
      if (pDataDefs->arrDataDescriptions[i].type != Descriptor_RigidBody)
        continue;

        sRigidBodyDescription *pRB = pDataDefs->arrDataDescriptions[i].Data.RigidBodyDescription;

        NamedRigidBody namedRigidBody;
        namedRigidBody.id = pRB->ID;
        namedRigidBody.name = pRB->szName;

        namedRigidBodies.push_back(namedRigidBody);
    }
    return ErrorCode_OK;
}
#endif

/**
 * @brief Loads all takes saved in the file ../balls.csv.
 */
QList<Take> loadTakesFromFile(QString filename){
    QVector< QVector<QString> > values = CSVFileHandler::loadFile("../balls.csv");
    QList<Take> takes;
    for(int row=0; row<values.length(); row++){
        Take take;
        take.size = values.at(row).at(0).toDouble();
        take.hardness = values.at(row).at(1).toDouble();
        take.name = values.at(row).at(2);
        takes.push_back(take);
    }

    return takes;
}



int main(int argc, char *argv[])
{
    time_t t;

     time(&t);
     srand((unsigned int)t);


    QApplication a(argc, argv);

    QList<Take> takes = loadTakesFromFile("../balls.csv");


    w = new MainWindow();
    w->setCapturedHand(&capturedHand);
    w->setCapturedTakeHandData(&capturedTakeHandData);
    w->setTakes(&takes);
    w->show();

#ifdef _WIN32
    int result = createClient();
    if (result != ErrorCode_OK){
        QMessageBox msg;
        msg.setText("The connection to the OptiTrack tracking system couldn't be established \n");

        QString detailed;

        switch(result){
        case 1:
            detailed = "An internal error occured. Make sure, that no other application uses the OptiTrack system right now.";
            break;
        case 2:
            detailed = "An external error occured. Please verify the connections to all cameras and hubs of the OptiTrack system.";
            break;
        case 3:
            detailed = "A network error occured. Check all options in Motive:Tracker and make sure that the pc is connected to the right network and uses a available ip adress";
            break;
        }

        msg.setDetailedText(detailed);

        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Save);
        msg.setIcon(QMessageBox::Critical);
        int res = msg.exec();

        // if the user cancels
        if(res==QMessageBox::Cancel) return -1;
    }
#endif

    return a.exec();
}
