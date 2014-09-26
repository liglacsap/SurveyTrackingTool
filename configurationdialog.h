#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QTime>
#include <QTimer>

#include "udpsocket.h"

#include "emstransmission.h"

namespace Ui {
class ConfigurationDialog;
}

/**
 * @brief A dialog to configure the ems intensity for every user.
 *
 */
class ConfigurationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(QWidget *parent = 0);
    ~ConfigurationDialog();

    /**
     * @brief Sets the Socket for communication
     * @param socket
     */
    void setSocket(UDPSocket* socket);

    /**
     * @brief setEMSTransmission
     * @param transmission
     */
    void setEMSTransmission(EMSTransmission* transmission);

private slots:
    void update();

    void on_onTimeBox_valueChanged(int arg1);
    void on_minimalChangeTimeBox_valueChanged(int arg1);
    void on_minCalibration_editingFinished();
    void on_minCalibration_valueChanged(int arg1);
    void on_minCalibrationSlider_sliderReleased();
    void on_onTimeSlider_sliderReleased();
    void on_minimalChangeTimeSlider_sliderReleased();
    void on_maxCalibrationSlider_sliderReleased();
    void on_onButton_clicked();
    void on_pad1Button_clicked();
    void on_pad2Button_clicked();
    void on_offButton_clicked();
    void increaseCalibration();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

    void on_offTimeBox_valueChanged(int arg1);

private:
    Ui::ConfigurationDialog *ui;



    QTimer *timer;
    QTimer *calibrationTimer;
    Pad pads[2];



     bool valueChanged;

     int currentChannel;

     QTime time;

     UDPSocket* socket;
     EMSTransmission* transmission;
};

#endif // CONFIGURATIONDIALOG_H
