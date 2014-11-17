#include "testdialog.h"
#include "ui_testdialog.h"

TestDialog::TestDialog(QWidget *parent) :
    TrackingDialog(parent),
    ui(new Ui::TestDialog)
{
    ui->setupUi(this);


    socket = new UDPSocket(this);
    transmission.clearMessage();
    transmission.setMaximumCalibration(0, 100);
    transmission.setMaximumCalibration(1, 0);
    transmission.setIntensity(100);
    socket->write(transmission.getMessage());

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(10);


}

TestDialog::~TestDialog()
{
    delete ui;
}

void TestDialog::handCaptured(CapturedHand hand)
{

}
void TestDialog::update()
{
    //qDebug() << checked << "      " << QString::fromStdString(transmission.getMessage());
    if(checked){
       // transmission.clearMessage();
        //transmission.setIntensity(intensity);
        //socket->write(transmission.getMessage());
    }
}

void TestDialog::on_horizontalSlider_sliderMoved(int position)
{
    transmission.clearMessage();

    transmission.setMaximumCalibration(0, position);
    transmission.setIntensity(100);
    //socket->write(transmission.getMessage());
    //transmission.clearMessage();
   // transmission.setIntensity(100);
    //socket->write(transmission.getMessage());
    qDebug() << QString::fromStdString(transmission.getMessage());
    socket->write(transmission.getMessage());

    ui->label->setText(QString::number(position));

}

void TestDialog::on_horizontalSlider_2_sliderMoved(int position)
{
    transmission.clearMessage();
    transmission.setMaximumCalibration(1, position);
    transmission.setIntensity(100);

    qDebug() << QString::fromStdString(transmission.getMessage());
    socket->write(transmission.getMessage());

    ui->label_2->setText(QString::number(position));

}
void TestDialog::on_horizontalSlider_valueChanged(int value)
{

}

void TestDialog::on_horizontalSlider_2_valueChanged(int value)
{

}


void TestDialog::on_TestDialog_destroyed()
{
    transmission.clearMessage();
    transmission.setIntensity(0);
    socket->write(transmission.getMessage());
}

void TestDialog::on_pushButton_clicked()
{
    transmission.clearMessage();
    transmission.setIntensity(0);
    socket->write(transmission.getMessage());

    this->close();
}


void TestDialog::on_pushButton_2_toggled(bool checked)
{
    transmission.clearMessage();
    if(checked){
       // ui->horizontalSlider->setEnabled(true);
       // ui->horizontalSlider_2->setEnabled(true);
        //ui->pushButton_2->setStyleSheet("background: #27ae60");
        transmission.setOn(0);
    }else{
       // ui->horizontalSlider->setEnabled(false);
       // ui->horizontalSlider_2->setEnabled(false);
        //ui->pushButton_2->setStyleSheet("background: #c0392b");
        transmission.setOff(0);
    }
    socket->write(transmission.getMessage());
}
