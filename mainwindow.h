#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QFileDialog>

#include <NatNetTypes.h>
#include <NatNetClient.h>

#include "configurationdialog.h"
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

using namespace std;


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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

private:
    Ui::MainWindow *ui;

    QTimer timer;
    QTime time;

    unsigned int take;
    unsigned int user;

    void saveTake();


};

#endif // MAINWINDOW_H
