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
    float distances[5];
    distances[0] = hand.fingers[4].distanceToPoint(hand.fingers[3]);
    distances[1] = hand.fingers[4].distanceToPoint(hand.fingers[2]);
    distances[2] = hand.fingers[3].distanceToPoint(hand.fingers[2]);

    //qDebug() << (abs(distances[0] - distances[1]) + abs(distances[0] - distances[2]) + abs(distances[2] -distances[0])) / 3.0f;
}

void CalibrationDialog::on_pushButton_clicked()
{

    this->close();
}

void CalibrationDialog::on_lineEdit_3_textChanged(const QString &arg1)
{
    this->offset = arg1.toFloat();
}
