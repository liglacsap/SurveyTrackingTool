#ifndef CSVFILEHANDLER_H
#define CSVFILEHANDLER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <QString>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include "tracking.h"
using namespace std;


/**
 * @brief Provides the ability to load and save csv files. In a CSV file all values are seperated by a semicolon and each line ends with a "\n".
 * If you want to use the ';' in a value, you need to escape this value for example test;1 will become "test;1"
 */
class CSVFileHandler
{
public:
    CSVFileHandler();

    /**
     * @brief Saves values to a CSV file
     * @param fileName
     * @param values
     * @return
     */
    static inline bool saveFile(QString fileName, QVector< QVector<QString> > values){
        QString line;
        for(int i=0; i<values.length(); i++){
            for(int j=0; j<values.at(i).length(); j++){
                line.append(values.at(i).at(j));

                if(j<values.at(i).length()-1)
                    line.append(";");
                else
                    line.append("\n");
            }
        }

        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::information(0, "Error while opening the balls.csv file for writing", file.errorString());
            return false;
        }

        QTextStream textStream(&file);
        textStream << line;
        file.close();

        return true;
    }

    /**
     * @brief Loads an CSV Fil
     * @param fileName
     * @return
     */
    static inline QVector< QVector<QString> > loadFile(QString fileName){
        QVector< QVector<QString> > result;

        QFile file(fileName);
        if(!file.open(QIODevice::ReadOnly)){
            QMessageBox::information(0, "Error while opening the balls.csv file for reading", file.errorString());
            return result;
        }

        QTextStream textStream(&file);


        while(!textStream.atEnd()){
            QString line = textStream.readLine();
            QStringList cells = line.split(";");

            result.push_back(cells.toVector());
        }

        return result;
    }
};

class SurveyUserFileHandler{
    QFile* file;

public:
    SurveyUserFileHandler(QString fileName){
        file = new QFile(fileName);
    }


    bool openFileForWriting(){
        if(!file->open(QIODevice::WriteOnly | QIODevice::Text)){
            QMessageBox::information(0, "Error while opening the specific file for writing", file->errorString());
            return false;
        }


        // Create Captions for each column.
        QVector<QString> row;
        row.push_back("User");
        row.push_back("Condition");
        row.push_back("Run");

        char chr[3] = {'X', 'Y', 'Z'};
        for(int i=0; i<5; i++){
            for(int j=0; j<3; j++){
                QString value = chr[j];
                value.push_back("_F");
                value.push_back(QString::number(i));

                row.push_back(value);
            }
        }
        row.push_back("X_Hand_Palm");
        row.push_back("Y_Hand_Palm");
        row.push_back("Z_Hand_Palm");
        row.push_back("Radius_Hand");
        row.push_back("Radius_Ball");
        row.push_back("EMS_Intensity");
        row.push_back("User_Selected_Ball");

        writeRow(row);

        return true;
    }

    void writeCapturedHand(CapturedCondition condition){
            QString str = QString::number(condition.user);
            str.append(";");
            str.append(QString::number(condition.condition));
            str.append(";");
            str.append(QString::number(condition.run));
            str.append(";");

            for(int i=0; i<condition.hand.fingers.size(); i++){
                str.append(QString::number(condition.hand.fingers[i].x()));
                str.append(";");
                str.append(QString::number(condition.hand.fingers[i].y()));
                str.append(";");
                str.append(QString::number(condition.hand.fingers[i].z()));
                str.append(";");
            }

            str.append(QString::number(condition.hand.palmPosition.x()));
            str.append(";");
            str.append(QString::number(condition.hand.palmPosition.y()));
            str.append(";");
            str.append(QString::number(condition.hand.palmPosition.z()));
            str.append(";");

            str.append(QString::number(condition.hand.fingerRadius));
            str.append(";");
            str.append(QString::number(123456789));
            str.append(";");
            str.append(QString::number(condition.hand.emsIntensity));
            str.append(";");
            str.append(QString::number(0));
            str.append("\n");

            writeRow(str);
    }

    void writeRow(QString row){
        if(!file->isOpen()) return;

        QTextStream textStream(file);
        textStream << row;

    }
    void writeRow(QVector<QString> values){
        if(!file->isOpen()) return;

        QString row;
        for(int i=0; i<values.length(); i++){
            row.append(values.at(i));

            if(i<values.length()-1)
                row.append(";");
            else
                row.append("\n");
        }

        QTextStream textStream(file);
        textStream << row;


    }

    void closeFileForWriting(){
        file->close();
    }

};

#endif // CSVFILEHANDLER_H
