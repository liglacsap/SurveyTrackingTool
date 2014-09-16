#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QTranslator>
#include <QVector3D>


#include "globals.h"

QVector3D handPalmPosition;
std::vector<QVector3D> handFingers;
double fingerRadius;
unsigned int markerCount;

vector<Take> takes;

vector<QVector3D> palmPositions;
std::vector<std::vector<QVector3D> > handFingersVector;

#ifdef _WIN32
    NatNetClient* client;
#endif

double calculateCircleRadius(QVector3D p1, QVector3D p2, QVector3D p3){
    QVector3D t = p2 - p1;
    QVector3D u = p3 - p1;
    QVector3D v = p3 - p2;

    //QVector3D w = QVector3D::crossProduct(t, u);

    // Formel von Wikipedia
    double radius = (t.length() * v.length() * u.length()) /
        (2 * (QVector3D::crossProduct(t, v)).length());

    return radius;
}
QVector3D calculateCircleCenter(QVector3D p1, QVector3D p2, QVector3D p3){
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

#ifdef _WIN32
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData)
{
    NatNetClient* pClient = (NatNetClient*)pUserData;
    pClient->SetVerbosityLevel(0);



    for (int i = 0; i < data->nRigidBodies; i++){
        //bool bTrackingValid = data->RigidBodies[i].params & 0x01;

        std::vector<QVector3D> marker;
        for (int j = 0; j < data->RigidBodies[i].nMarkers; j++){
            float x = data->RigidBodies[i].Markers[j][0];
            float y = data->RigidBodies[i].Markers[j][1];
            float z = data->RigidBodies[i].Markers[j][2];

            qDebug() << j<<"  x:" << x << " y:"<<y<<" z:"<<z;

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
       // if (data->nRigidBodies == 0) return;

        // Filter all points and only return the points which are not related to any rigid body
        bool a = true;

        for (int j = 0; j < data->RigidBodies[0].nMarkers; j++){
            double _x = data->RigidBodies[0].Markers[j][0];
            double _y = data->RigidBodies[0].Markers[j][1];
            double _z = data->RigidBodies[0].Markers[j][2];

            double diffX = abs(x - _x);
            double diffY = abs(y - _y);
            double diffZ = abs(z - _z);

            //qDebug() << diffX << " " << diffY << diffZ;
            if (diffX < 0.001 && diffY < 0.001 && diffZ < 0.001)
                a = false;
        }

        //if (a){
            QVector3D vec = QVector3D(x, y, z);
            double distance = vec.distanceToPoint(handPalmPosition);

           // if(distance > 1.7 && distance < 8.0){
                handFingers.push_back(QVector3D(x, y, z));
            //}



        //}
    }

    if(handFingers.size() >= 3)   {
        double d1 = handFingers[0].distanceToPoint(handFingers[1]);
        double d2 = handFingers[1].distanceToPoint(handFingers[2]);
        double d3 = handFingers[2].distanceToPoint(handFingers[0]);
        if(d1 > 30 || d2 > 30 || d3 > 30) return;

        fingerRadius = calculateCircleRadius(handFingers[0], handFingers[1], handFingers[2]); // 5.5 is the radius of a marker
        fingerRadius = (fingerRadius > 200) ? 0 : fingerRadius;
        fingerRadius = (fingerRadius < 1.25) ? 1.25 : fingerRadius;

        handFingersVector.push_back(handFingers);
    }

}

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

    return ErrorCode_OK;
}
#endif

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
