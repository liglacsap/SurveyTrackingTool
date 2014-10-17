#ifndef STUDYTWODIALOG_H
#define STUDYTWODIALOG_H


#include <QMainWindow>
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


#include <qt/trackingdialog.h>

#define SIMULATE_BALL1 0
#define SIMULATE_BALL2 1
#define SIMULATE_NONE 2

namespace Ui {
class StudyTwoDialog;
}

class StudyTwoDialog : public TrackingDialog
{
    Q_OBJECT
protected:
    void gotoNextCondition(int guessedBall);
public:
    explicit StudyTwoDialog(QWidget *parent = 0);
    ~StudyTwoDialog();

    void setConditions(QList<Condition> *conditions);
    void setUser(int user);
    void setFeedback(QString feedback);
public slots:
    void handCaptured(CapturedHand hand);

    void update();
private slots:


    void on_ballOneSelectedButton_clicked();

    void on_ballTwoSelectedButton_clicked();

    void swapRandom();
    void on_simulateBall2Button_clicked();

    void on_showChooseDialogButton_clicked();

private:
    Ui::StudyTwoDialog *ui;

    int turns = 0;
    int round = 0;
    bool guessedRight = false;
    int state = SIMULATE_BALL1;

    QTimer timer;

    int intensity;
};

#endif // MAINWINDOW_H
