#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QFileDialog>

#ifdef _WIN32
    #include <NatNetTypes.h>
    #include <NatNetClient.h>
#endif
#include "configurationdialog.h"
#include "takedialog.h"
#include "globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <sstream>
#include <random>
#include <chrono>

#include "udpsocket.h"

using namespace std;

inline int myrandom (int i) { return std::rand()%i;}

namespace Ui {
class MainWindow;
}

struct Marker{
    float position[3];
    string name;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    CapturedHand* capturedHand;
    CapturedTakeHandData* capturedTakeHandData;
    QList<Take>* takes;

    CapturedHandDataListener* listener;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setCapturedHand(CapturedHand* hand);
    void setTakes(QList<Take>* takes);
    void setCapturedTakeHandData(CapturedTakeHandData* data);
    void setCapturedHandDataListener(CapturedHandDataListener* listener);

    Take getCurrentTake();
private slots:
    void on_actionExit_triggered();

    void on_actionCalibration_triggered();

    void on_actionStart_take_triggered();

    void update();
    void on_actionFinish_take_triggered();

    void on_actionPrevious_take_triggered();

    void on_actionNext_take_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

    void on_actionRestart_take_triggered();

    void on_actionTake_Properties_triggered();

    void on_horizontalSlider_valueChanged(int value);
public slots:
    void dataAdded(CapturedHand* hand);
private:
    Ui::MainWindow *ui;

    QTimer timer;
    QTime time;

    QTimer *ctimer;

    // for the communication with the Arduino Board / EMS System
    UDPSocket *socket;

    // current take. Every user has several takes to accoumplish
    unsigned int take;

    // current user. Used for the identification later in the study
    unsigned int user;

    /**
     * @brief Saves the current take in an CSV file.
     */
    void saveTake();

    void shuffleTakes();


    EMSTransmission transmission;
    ConfigurationDialog dialog;

};

#endif // MAINWINDOW_H
