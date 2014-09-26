
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QTranslator>
#include <QVector3D>


#include "globals.h"

#define RIGHT_HAND_STRING "RightHand"
#define LEFT_HAND_STRING "LeftHand"

QVector3D handPalmPosition;
std::vector<QVector3D> handFingers;
double fingerRadius;
unsigned int markerCount;

struct NamedRigidBody{
    string name;
    int id;
};

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

vector<Take> takes;

QVector3D thumb;

vector<QVector3D> palmPositions;
std::vector<std::vector<QVector3D> > handFingersVector;

#ifdef _WIN32
    NatNetClient* client;
#endif




bool vectorXGreaterThan(const QVector3D &v1, const QVector3D &v2){
    return v1.x() > v2.x();
}
bool vectorXLessThan(const QVector3D &v1, const QVector3D &v2){
    return v1.x() < v2.x();
}

bool distanceToThumb(const QVector3D &v1, const QVector3D &v2){
    return thumb.distanceToPoint(v1) < thumb.distanceToPoint(v2);
}

// The Optitrack system is only available on Windows. In order to use this software with linux the values must be send via
// network connection.
#ifdef _WIN32
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData)
{
    NatNetClient* pClient = (NatNetClient*)pUserData;

    // disable all debug information provided by the OptiTrack system
    pClient->SetVerbosityLevel(0);


    for (int i = 0; i < data->nRigidBodies; i++){
        std::vector<QVector3D> marker;
        for (int j = 0; j < data->RigidBodies[i].nMarkers; j++){
            float x = data->RigidBodies[i].Markers[j][0];
            float y = data->RigidBodies[i].Markers[j][1];
            float z = data->RigidBodies[i].Markers[j][2];

            marker.push_back(QVector3D(x, y, z));
        }

        if (marker.size() > 2)
            handPalmPosition = calculateCircleCenter(marker[0], marker[1], marker[2]) * 100;

    }

    if(isnan(handPalmPosition.x()))
        handPalmPosition = QVector3D(0, 0, 0);


    palmPositions.push_back(handPalmPosition);

    handFingers.clear();

    markerCount =  data->nOtherMarkers;

    for (int i = 0; i < data->nOtherMarkers; i++){
        double x = data->OtherMarkers[i][0]*100;
        double y = data->OtherMarkers[i][1]*100;
        double z = data->OtherMarkers[i][2]*100;


        // Return if hand palm rigid body was not detected
        if (data->nRigidBodies == 0) return;

        // Filter all points and only return the points which are not related to any rigid body
        bool a = true;

        for (int j = 0; j < data->RigidBodies[0].nMarkers; j++){
            double _x = data->RigidBodies[0].Markers[j][0];
            double _y = data->RigidBodies[0].Markers[j][1];
            double _z = data->RigidBodies[0].Markers[j][2];

            double diffX = abs(x - _x);
            double diffY = abs(y - _y);
            double diffZ = abs(z - _z);

            if (diffX < 0.001 && diffY < 0.001 && diffZ < 0.001)
                a = false;
        }

        if (a){
            QVector3D vec = QVector3D(x, y, z);
            double distance = vec.distanceToPoint(handPalmPosition);

            if(distance > 10.0 && distance < 20.0){
                handFingers.push_back(QVector3D(x, y, z));
            }
        }
    }



    if(handFingers.size() == 5){
        if(getNameOfRigidBody(data->RigidBodies[0].ID)==RIGHT_HAND_STRING)
            qSort(handFingers.begin(), handFingers.end(), vectorXLessThan);
        else
            qSort(handFingers.begin(), handFingers.end(), vectorXGreaterThan);

        thumb = handFingers[0];

        qSort(handFingers.begin(), handFingers.end(), distanceToThumb);

        fingerRadius = calculateCircleRadius(handFingers[0], handFingers[1], handFingers[2]); // 5.5 is the radius of a marker
        fingerRadius = (fingerRadius > 200) ? 0 : fingerRadius;
        fingerRadius = (fingerRadius < 1.25) ? 1.25 : fingerRadius;

        handFingersVector.push_back(handFingers);
    }
}

/**
 * @brief Creates the client for the OptiTrack system.
 * @param iConnectionType
 * @return
 */
int createClient(int iConnectionType)
{
    // release previous server
    if (client){
        client->Uninitialize();
        delete client;
    }

    // create NatNet client
    client = new NatNetClient(iConnectionType);


    // Set callback handlers
    client->SetDataCallback(DataHandler, client);	// this function will receive data from the server

    int retCode = client->Initialize("127.0.0.1", "127.0.0.1");

    // to use a different port for commands and/or data:
    //int retCode = theClient->Initialize(szMyIPAddress, szServerIPAddress, MyServersCommandPort, MyServersDataPort);
    if (retCode != ErrorCode_OK){
        printf("Unable to connect to server.  Error code: %d. Exiting", retCode);
        return ErrorCode_Internal;
    }else {
        // print server info
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
        std::string rigidBodyName = pRB->szName;

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
void loadTakes(){
    QVector< QVector<QString> > values = CSVFileHandler::loadFile("../balls.csv");
    takes.clear();
    for(int row=0; row<values.length(); row++){
        Take take;
        take.size = values.at(row).at(0).toDouble();
        take.hardness = values.at(row).at(1).toDouble();
        take.name = values.at(row).at(2);
        takes.push_back(take);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef _WIN32
    int result = createClient(ConnectionType_Multicast);
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
        msg.exec();
    }
#endif
    /*
    if(!ping("192.168.3.7")){
        QMessageBox msg;
        msg.setText("The connection to the EMS system couldn't be established \n");
        msg.setIcon(QMessageBox::Critical);
        msg.exec();
    }
    */

    loadTakes();

    MainWindow w;
    w.show();

    return a.exec();
}
