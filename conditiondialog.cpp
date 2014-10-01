#include "conditiondialog.h"
#include "ui_conditiondialog.h"

ConditionDialog::ConditionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConditionDialog)
{
    ui->setupUi(this);

    QVector< QVector<QString> > values = CSVFileHandler::loadFile("../balls.csv");

    qDebug() << values;
    this->ui->tableWidget->setRowCount(values.length());


    this->ui->tableWidget->clear();
    Conditions.clear();
    for(int row=0; row<values.length(); row++){
        for(int column=0; column<3; column++){
            QTableWidgetItem *newItem = new QTableWidgetItem(values.at(row).at(column));
            this->ui->tableWidget->setItem(row, column, newItem);
        }

        Condition Condition;
        Condition.size = values.at(row).at(0).toDouble();
        Condition.hardness = values.at(row).at(1).toDouble();
        Condition.name = values.at(row).at(2);
        Conditions.push_back(Condition);

    }


}

ConditionDialog::~ConditionDialog()
{
    delete ui;
}

void ConditionDialog::on_pushButton_2_clicked()
{
    this->ui->tableWidget->setRowCount(this->ui->tableWidget->rowCount()+1);
}

void ConditionDialog::on_pushButton_4_clicked()
{
    QVector< QVector<QString> > values;

    for(int row=0; row<ui->tableWidget->rowCount(); row++){
        QVector<QString> line;
        for(int column=0; column<ui->tableWidget->columnCount(); column++){
            line.push_back(ui->tableWidget->item(row, column)->text());
        }

        values.push_back(line);
    }

    CSVFileHandler::saveFile("../balls.csv", values);


}

void ConditionDialog::on_pushButton_clicked()
{
    QList<QTableWidgetItem*> selection = ui->tableWidget->selectedItems();
    for(int i=0; i<selection.length(); i++){
        ui->tableWidget->removeRow(selection.at(i)->row());
    }
}
