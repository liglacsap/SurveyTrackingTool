#include "takedialog.h"
#include "ui_takedialog.h"

TakeDialog::TakeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TakeDialog)
{
    ui->setupUi(this);

    QVector< QVector<QString> > values = CSVFileHandler::loadFile("../balls.csv");

    this->ui->tableWidget->setRowCount(values.length()-1);

    takes.clear();
    for(int row=0; row<values.length()-1; row++){
        for(int column=0; column<3; column++){
            QTableWidgetItem *newItem = new QTableWidgetItem(values.at(row+1).at(column));
            this->ui->tableWidget->setItem(row, column, newItem);
        }

        Take take;
        take.size = values.at(row+1).at(0).toDouble();
        take.hardness = values.at(row+1).at(1).toDouble();
        take.name = values.at(row+1).at(2).toStdString();
        takes.push_back(take);
    }


}

TakeDialog::~TakeDialog()
{
    delete ui;
}

void TakeDialog::on_pushButton_2_clicked()
{
    this->ui->tableWidget->setRowCount(this->ui->tableWidget->rowCount()+1);
    QString text = "Take ";
    //ui->listWidget->addItem(text);

    //this->ui->textLineEdit->setText(text);
}

void TakeDialog::on_pushButton_4_clicked()
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

void TakeDialog::on_pushButton_clicked()
{
    QList<QTableWidgetItem*> selection = ui->tableWidget->selectedItems();
    for(int i=0; i<selection.length(); i++){
        ui->tableWidget->removeRow(selection.at(i)->row());
    }
}
