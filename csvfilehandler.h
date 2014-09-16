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
using namespace std;


/**
 * @brief Provides the ability to load and save csv files. In a CSV file all values are seperated by a semicolon and each line ends with a "\n".
 * If you want to use the ';' in a value, you need to escape this value for example test;1 will become "test;1"
 */
class CSVFileHandler
{
public:
    CSVFileHandler();

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

#endif // CSVFILEHANDLER_H
