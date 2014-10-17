#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    TrackingDialog(parent),
    ui(new Ui::CalibrationDialog)
{
    ui->setupUi(this);
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::handCaptured(CapturedHand hand){

    double offset = hand.fingerRadius*2-this->ui->lineEdit_2->text().toDouble();
    this->ui->lineEdit->setText(QString::number(hand.fingerRadius*2));
    this->ui->lineEdit_3->setText(QString::number((int)(offset*100+0.5)/100.0));

    this->offset = offset;
}

void CalibrationDialog::on_pushButton_clicked()
{

    this->close();
}
