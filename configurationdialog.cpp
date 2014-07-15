#include "configurationdialog.h"
#include "ui_configurationdialog.h"

ConfigurationDialog::ConfigurationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigurationDialog)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(50);

    calibrationTimer = new QTimer(this);
    connect(calibrationTimer, SIGNAL(timeout()), this, SLOT(increaseCalibration()));

    for(int i=0; i<2; i++){
        pads[i].onTime = 0;
        pads[i].minChangeTime = 0;
        pads[i].intensity = 0;
        pads[i].channel = i;
    }


    currentChannel = 0;
    valueChanged = false;

    socket = NULL;
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}

void ConfigurationDialog::setSocket(UDPSocket *socket)
{
    this->socket = socket;
}

void ConfigurationDialog::setEMSTransmission(EMSTransmission* transmission)
{
    this->transmission = transmission;
}

/**
 * @brief MainWindow::update
 * Fetches for each pad all values and sends them as a udp package to the digital circuit via WiFi
 */
void ConfigurationDialog::update(){
    if(this->isVisible())
        socket->write(transmission->getMessage());
}




/**
 * @brief MainWindow::initSocket
 * Creates and initializes the socket on port 454545 and bind the readyRead signal to the ui to listen
 * for pending messages
 * @return
 */

void ConfigurationDialog::on_onTimeBox_valueChanged(int arg1)
{
    pads[0].onTime = arg1;

    pads[0].state = ON;
    transmission->clearMessage();
    transmission->setIntensity(arg1);
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));

    //valueChanged = true;
}

void ConfigurationDialog::on_minimalChangeTimeBox_valueChanged(int arg1)
{
    pads[0].minChangeTime = arg1;
    transmission->clearMessage();
    transmission->setMinimalChangeTime(arg1);

    //valueChanged = true;
}



void ConfigurationDialog::on_minCalibration_editingFinished()
{
    //transmission.clearMessage();
    transmission->setMinimumCalibration(currentChannel, ui->minCalibration->value());
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));
}

void ConfigurationDialog::on_minCalibration_valueChanged(int arg1)
{
}

void ConfigurationDialog::on_minCalibrationSlider_sliderReleased()
{
    //transmission.clearMessage();
    transmission->setMinimumCalibration(currentChannel, ui->minCalibration->value());
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));
}

void ConfigurationDialog::on_maxCalibrationSlider_sliderReleased()
{
    //transmission.clearMessage();
    transmission->setMaximumCalibration(currentChannel, ui->maxCalibration->value());
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));
}


void ConfigurationDialog::on_onTimeSlider_sliderReleased()
{
    //transmission.clearMessage();
    //transmission.setOnTime(ui->onTimeBox->value());
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));
}

void ConfigurationDialog::on_offTimeSlider_sliderReleased()
{
    transmission->setOffTime(ui->offTimeBox->value());
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));
}

void ConfigurationDialog::on_minimalChangeTimeSlider_sliderReleased()
{
    transmission->setMinimalChangeTime(ui->minimalChangeTimeBox->value());
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));
}


void ConfigurationDialog::on_onButton_clicked()
{
    if(ui->pad1Button->isChecked()){
        pads[0].state = ON;
    }else{
        pads[1].state = ON;
    }

    ui->stateWidget->setStyleSheet("background-color:#27ae60");

    transmission->clearMessage();
    transmission->setOn(currentChannel);
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));

    valueChanged = true;
}

void ConfigurationDialog::on_offButton_clicked()
{

    if(ui->pad1Button->isChecked()){
        pads[0].state = OFF;
    }else{
        pads[1].state = OFF;
    }

    ui->stateWidget->setStyleSheet("background-color:#c0392b");

    transmission->clearMessage();
    transmission->setOff(currentChannel);
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));

    valueChanged = true;
}


void ConfigurationDialog::on_pad1Button_clicked()
{
    currentChannel = 0;
    if(pads[0].state == ON)
        ui->stateWidget->setStyleSheet("background-color:#27ae60");
    else
        ui->stateWidget->setStyleSheet("background:#c0392b");
}

void ConfigurationDialog::on_pad2Button_clicked()
{
    currentChannel = 1;

    if(pads[1].state == ON)
        ui->stateWidget->setStyleSheet("background-color:#27ae60");
    else
        ui->stateWidget->setStyleSheet("background:#c0392b");
}

void ConfigurationDialog::increaseCalibration(){
    if(!ui->minCalibrationChecked->isChecked())
        ui->minCalibrationSlider->setValue(ui->minCalibrationSlider->value()+1);

    ui->maxCalibrationSlider->setValue(ui->maxCalibrationSlider->value()+1);

    ui->minCalibration->setValue(ui->minCalibrationSlider->value());
    ui->maxCalibration->setValue(ui->maxCalibrationSlider->value());

    transmission->clearMessage();
    transmission->setIntensity(ui->maxCalibration->value());
    //ui->lineEdit->setText(QString::fromStdString(transmission.getMessage()));

}

void ConfigurationDialog::on_pushButton_2_clicked()
{
    calibrationTimer->start(200);
}

void ConfigurationDialog::on_pushButton_3_clicked()
{
    calibrationTimer->stop();
}

