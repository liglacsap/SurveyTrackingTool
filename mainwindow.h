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
#include "conditiondialog.h"
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

#include <QProgressBar>
#include <QSpacerItem>

#include "udpsocket.h"

using namespace std;

namespace Ui {
class MainWindow;
}

struct Marker{
    float position[3];
    string name;
};

struct ConditionIndex{
    int x;
    int y;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    CapturedHand* capturedHand;
    CapturedConditionHandData* capturedConditionHandData;
    QList<Condition>* conditions;

    QList<QList<int>> conditionsMatrix;
    ConditionIndex currentConditionIndex;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setCapturedHand(CapturedHand* hand);
    void setConditions(QList<Condition>* conditions);
    void setCapturedConditionHandData(CapturedConditionHandData* data);

    Condition getCurrentCondition();
private slots:
    void on_actionExit_triggered();

    void on_actionCalibration_triggered();

    void on_actionNew_triggered();

    void on_actionTake_Properties_triggered();

    void update();

    void gotoNextCondition();


    void on_gotoNextCondition_clicked();

    void on_userSelectedBallLineEdit_textChanged(const QString &arg1);

public slots:
    void dataAdded(CapturedHand* hand);
private:
    Ui::MainWindow *ui;
    QProgressBar* bar;

    QTimer timer;

    // for the communication with the Arduino Board / EMS System
    UDPSocket *socket;

    // current user. Used for the identification later in the study
    unsigned int user;

    /**
     * @brief Saves the current Condition in an CSV file.
     */
    void saveCondition();

    void shuffleConditions();


    EMSTransmission transmission;
    ConfigurationDialog dialog;

};

#endif // MAINWINDOW_H
