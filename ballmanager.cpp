#include "objectmanager.h"

ObjectManager::ObjectManager(QObject *parent) :
    QObject(parent)
{
    currentObjectIndex = 0;
}

void ObjectManager::loadFromFile()
{
    QVector< QVector<QString> > values = CSVFileHandler::loadFile("../balls.csv");

    qDebug() << values;
    this->ui->tableWidget->setRowCount(values.length());


    this->ui->tableWidget->clear();
    takes.clear();
    for(int row=0; row<values.length(); row++){
        for(int column=0; column<3; column++){
            QTableWidgetItem *newItem = new QTableWidgetItem(values.at(row).at(column));
            this->ui->tableWidget->setItem(row, column, newItem);
        }

        Take take;
        take.size = values.at(row).at(0).toDouble();
        take.hardness = values.at(row).at(1).toDouble();
        take.name = values.at(row).at(2);
        takes.push_back(take);

    }
}
