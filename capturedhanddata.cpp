#include "capturedhanddata.h"

CapturedHandDataListener::CapturedHandDataListener(QObject *parent) :
    QObject(parent)
{
    qDebug() << capturedData.size();
}

void CapturedHandDataListener::save()
{
    qDebug() << capturedData.size();
    return;

    QVector< QVector<QString> > values;

    // Create Captions for each column.
    QVector<QString> row;
    char chr[3] = {'X', 'Y', 'Z'};
    for(int i=0; i<5; i++){
        for(int j=0; j<3; j++){
            QString value = chr[j];
            value.push_back("(F");
            value.push_back(QString::number(i));
            value.push_back(")");

            row.push_back(value);
        }
    }
    row.push_back("X(Hand Palm)");
    row.push_back("Y(Hand Palm)");
    row.push_back("Z(Hand Palm)");
    row.push_back("Radius Hand");
    row.push_back("Radius Ball");
    row.push_back("Ball Hardness");
    row.push_back("EMS Intensity");
    values.push_back(row);

    // Concatenate several strings to the final filename
    QDateTime time(QDateTime::currentDateTime());

    QString fileName = "take";
    fileName.append(QString::number(123));
    fileName.append("_user_");
    fileName.append(QString::number(123));
    fileName.append("__");
    fileName.append(QString::number(time.toTime_t()));
    fileName.append("__");
    fileName.append(".csv");

    qDebug() << capturedData.size();

    // save all captured finger positions
    for(unsigned int i=0; i<capturedData.size(); i++){
        row.clear();

        CapturedHand hand = capturedData[i];
        for(unsigned int j=0; j<5; j++){
            if(hand.fingers.size() >= j){
                row.push_back(QString::number(hand.fingers[j].x()));
                row.push_back(QString::number(hand.fingers[j].y()));
                row.push_back(QString::number(hand.fingers[j].z()));
            }
            else    // save placeholder in case that less than three fingers where captured
            {
                row.push_back(QString::number(0));
                row.push_back(QString::number(0));
                row.push_back(QString::number(0));
                row.push_back(QString::number(0));
                row.push_back(QString::number(0));
            }
        }

        // save hand palm position
        row.push_back(QString::number(hand.palmPosition.x()));
        row.push_back(QString::number(hand.palmPosition.y()));
        row.push_back(QString::number(hand.palmPosition.z()));

        // radius hand
        row.push_back(QString::number(hand.fingerRadius));

        row.push_back(QString::number(0));
        row.push_back(QString::number(0));
        // radius ball
        //row.push_back(QString::number(takes->at(take).size));

        // hardness ball
        //row.push_back(QString::number(takes->at(take).hardness));

        values.push_back(row);
    }

    CSVFileHandler::saveFile(fileName, values);

}

void CapturedHandDataListener::appendData(CapturedHand hand)
{
    capturedData.push_back(hand);

    emit(dataAdded(&hand));
}

