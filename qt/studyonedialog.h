#ifndef STUDYONEDIALOG_H
#define STUDYONEDIALOG_H

#include <QDialog>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QFileDialog>

#ifdef _WIN32
    #include <NatNetTypes.h>
    #include <NatNetClient.h>
#endif

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

#include "configurationdialog.h"
#include "conditiondialog.h"


#include "trackingdialog.h"

using namespace std;

namespace Ui {
class StudyOneDialog;
}


class StudyOneDialog : public TrackingDialog
{
    Q_OBJECT


public:
    explicit StudyOneDialog(QWidget *parent = 0);
    ~StudyOneDialog();

    void setConditions(QList<Condition> *conditions);
    void setUser(User user);
    void setFeedback(QString feedback);
protected:
    void keyPressEvent(QKeyEvent* event);
private slots:
    void on_actionExit_triggered();

    void on_actionCalibration_triggered();

    void on_actionTake_Properties_triggered();

    void update();


    void on_StudyOneDialog_destroyed();

    void on_horizontalSlider_sliderMoved(int position);

protected slots:
        void gotoNextCondition();
public slots:
        void handCaptured(CapturedHand hand);

private:
    Ui::StudyOneDialog *ui;
    QTimer timer;

    int v;
    qint64 lastTimeStamp;

    ConfigurationDialog dialog;
};

#endif // MAINWINDOW_H
