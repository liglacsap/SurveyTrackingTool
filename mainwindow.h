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
#include <QKeyEvent>
#include <QInputDialog>

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
    QList<Condition>* conditions;

    QList<QList<int>> conditionsMatrix;
    ConditionIndex currentConditionIndex;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setConditions(QList<Condition>* conditions);

    Condition getCurrentCondition();
private slots:
    void on_actionExit_triggered();

    void on_actionCalibration_triggered();

    void on_actionNew_triggered();

    void on_actionTake_Properties_triggered();

    void update();

    void gotoNextCondition();
    void on_MainWindow_destroyed();

public slots:
    void handCaptured(CapturedHand hand);

protected:
    void keyPressEvent(QKeyEvent* event);
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

    SurveyUserFileHandler* file;
};

#endif // MAINWINDOW_H
