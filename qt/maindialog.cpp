#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    conditions = loadConditionsFromFile("../../balls.csv");

    ui->buttonStudy1->setEnabled(false);
    ui->buttonStudy2->setEnabled(false);
    ui->buttonCalibrate->setEnabled(false);
    ui->buttonTest->setEnabled(false);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_buttonTest_clicked()
{

}

void MainDialog::on_buttonCalibrate_clicked()
{
    ui->buttonCalibrate->setEnabled(false);
    CalibrationDialog dialog;


    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     &dialog, SLOT(handCaptured(CapturedHand)));

    dialog.exec();
    offset = dialog.getOffset();

    ui->buttonStudy1->setEnabled(true);
    ui->buttonStudy2->setEnabled(true);
}

void MainDialog::on_buttonStudy1_clicked()
{
    ui->buttonStudy1->setEnabled(false);
    StudyOneDialog dialog;


    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     &dialog, SLOT(handCaptured(CapturedHand)));

    dialog.setConditions(&conditions);
    dialog.setUser(ui->lineEdit->text().toInt());
    dialog.setOffset(offset);
    dialog.setFeedback(ui->listWidget->currentItem()->text());
    dialog.exec();
}

void MainDialog::on_buttonStudy2_clicked()
{
    ui->buttonStudy2->setEnabled(false);
    StudyTwoDialog dialog;


    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     &dialog, SLOT(handCaptured(CapturedHand)));

    dialog.setConditions(&conditions);
    dialog.setUser(ui->lineEdit->text().toInt());
    dialog.setOffset(this->offset);
    dialog.setFeedback(ui->listWidget->currentItem()->text());
    dialog.exec();
}

void MainDialog::on_listWidget_currentTextChanged(const QString &currentText)
{
    if(currentText != "" && ui->lineEdit->text() != ""){
        ui->buttonCalibrate->setEnabled(true);
        ui->buttonTest->setEnabled(true);
    }

    ui->lineEdit->setEnabled(false);
    ui->listWidget->setEnabled(false);
}

void MainDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->listWidget->selectedItems().size() > 0){
        ui->buttonCalibrate->setEnabled(true);
        ui->buttonTest->setEnabled(true);
    }
}
