
#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QTranslator>
#include <QVector3D>
#include <QObject>

#include "globals.h"
#include "tracking.h"



CapturedConditionHandData capturedConditionHandData;

MainWindow* w;



// The Optitrack system is only available on Windows. In order to use this software with linux the values must be send via
// network connection.
#ifdef _WIN32



/**
 * @brief DataHandler of the OptiTrack. Retrieves all data and calculates the hand palm position and all hand specific stuff
 * @param data The Data provided by the OptiTrack
 */
void __cdecl DataHandler(sFrameOfMocapData* data, void* pUserData)
{
    /*
    NatNetClient* pClient = (NatNetClient*)pUserData;

    // disable all debug information provided by the OptiTrack system
    pClient->SetVerbosityLevel(0);

    // Return if hand palm rigid body was not detected
    if (data->nRigidBodies == 0) return;


    capturedHand.palmPosition = calculateHandPalmPosition(data);


    // handFingers is a vector to save all captured finger positions within a Condition
    capturedHand.fingers.clear();

    QList<QVector3D> marker = getAllMarker(data, capturedHand.palmPosition);

    QListIterator<QVector3D> i(marker);
    while (i.hasNext())
         capturedHand.fingers.push_back(i.next());


    if(capturedHand.fingers.size() == 5){
        for(int i=0; i<data->nRigidBodies; i++){
            if(getNameOfRigidBody(data->RigidBodies[i].ID)==RIGHT_HAND_STRING){
                capturedHand.type = RIGHT_HAND_STRING;

                qSort(capturedHand.fingers.begin(), capturedHand.fingers.end(), vectorXGreaterThan);
                break;
            }else if(getNameOfRigidBody(data->RigidBodies[i].ID)==LEFT_HAND_STRING){
                capturedHand.type = LEFT_HAND_STRING;

                qSort(capturedHand.fingers.begin(), capturedHand.fingers.end(), vectorXLessThan);
                break;
            }
        }




        thumb = capturedHand.fingers[0];

        qSort(capturedHand.fingers.begin(), capturedHand.fingers.end(), distanceToThumbLessThan);

        capturedHand.fingerRadius = calculateCircleRadius(capturedHand.fingers[0], capturedHand.fingers[1], capturedHand.fingers[2]); // 5.5 is the radius of a marker
        capturedHand.emsIntensity = calculateIntensity();
        capturedConditionHandData.hands.push_back(capturedHand);
    }
    */
}


#endif

/**
 * @brief Loads all Conditions saved in the file ../balls.csv.
 */
QList<Condition> loadConditionsFromFile(QString filename){
    QVector< QVector<QString> > values = CSVFileHandler::loadFile("../balls.csv");
    QList<Condition> Conditions;
    for(int row=0; row<values.length(); row++){
        Condition Condition;
        Condition.size = values.at(row).at(0).toDouble();
        Conditions.push_back(Condition);
    }

    return Conditions;
}

int main(int argc, char *argv[])
{
    srand (time(NULL));

    QApplication a(argc, argv);

    QList<Condition> Conditions = loadConditionsFromFile("../balls.csv");

    w = new MainWindow();
    w->setConditions(&Conditions);
    w->show();

    // qt related stuff
    qRegisterMetaType<CapturedHand>("CapturedHand");

    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     w, SLOT(handCaptured(CapturedHand)));

#ifdef _WIN32
    int result = Tracking::getInstance().createClient();
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
