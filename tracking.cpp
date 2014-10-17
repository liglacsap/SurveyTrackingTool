#include "tracking.h"


/**
 * @brief Finds the corresponding name of a RigidBody
 * @param the id of the RigidBody
 * @return The name of the RigidBody
 */
QString Tracking::getNameOfRigidBody(int id){
    QListIterator<NamedRigidBody> i(namedRigidBodies);

    while (i.hasNext()){
        NamedRigidBody a = i.next();
        if(a.id == id)
                return  QString::fromStdString(a.name);
    }

    return "undefined";
}
/**
 * @brief Return all marker not belonging to any rigid body as a list
 * @param data The Data provided by the OptiTrack and received in DataHandler
 * @param handPalmPosition the center of hand palm. See calculateHandPalmPosition for mor information
 * @return a QList of type QVector3D
 */
QList<QVector3D> Tracking::getAllMarker(sFrameOfMocapData* data, QVector3D handPalmPosition){
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
            if(distance > 1.0 && distance < 100.0){
                result.push_back(QVector3D(x, y, z));
            }
        }
    }

    return result;
}





/**
 * @brief Calculates the hand palm position based on the three markers of the hand rigid body
 * @param data The Data provided by the OptiTrack and received in DataHandler
 * @return The calculated hand center position as QVector3D
 */
QVector3D Tracking::calculateHandPalmPosition(sFrameOfMocapData* data){
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
            //if (marker.size() > 2)
                //result = calculateCircleCenter(marker[0], marker[1], marker[2]) * 100;
        }
    }

    return result;
}


int Tracking::createClient(){
    // release previous server
    if (client){
        client->Uninitialize();
        delete client;
    }

    // create NatNet client
    client = new NatNetClient(ConnectionType_Multicast);


    // Set callback handlers
    int res = client->SetDataCallback(&DataHandlerStatic, client);	// this function will receive data from the server

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

    for (int i=0;i<pDataDefs->nDataDescriptions;i++){
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

void Tracking::DataHandlerStatic(sFrameOfMocapData* data, void* pUserData){
    getInstance().DataHandler(data, pUserData);
}

void Tracking::DataHandler(sFrameOfMocapData* data, void* pUserData){
    NatNetClient* pClient = (NatNetClient*)pUserData;

    // disable all debug information provided by the OptiTrack system
    pClient->SetVerbosityLevel(0);


    // Return if hand palm rigid body was not detected
    //if (data->nRigidBodies == 0) return;

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

        QList<IndexedFingerForThumbOrdering>finger;
        for(int i=0; i<capturedHand.fingers.size(); i++){
            finger.push_back({capturedHand.fingers[i], capturedHand.fingers[0]});
        }

        qSort(finger.begin(), finger.end(), distanceLowerToThumb);


        capturedHand.fingerRadius = calculateCircleRadius(finger[0].position, finger[1].position, finger[2].position); // 5.5 is the radius of a marker

        // kind of stabilization
        //qDebug() << ceil(capturedHand.fingerRadius * 100) / 100;
        //capturedHand.emsIntensity = calculateIntensity();

        emit(handCaptured(capturedHand));
    }

}
