#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

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

#include "trackingdialog.h"

using namespace std;

namespace Ui {
class CalibrationDialog;
}


class CalibrationDialog : public TrackingDialog
{
    Q_OBJECT


public:
    explicit CalibrationDialog(QWidget *parent = 0);
    ~CalibrationDialog();

    float getOffset(){
        return offset;
    }

public slots:
        void handCaptured(CapturedHand hand);


private slots:
        void on_pushButton_clicked();

        void on_lineEdit_3_textChanged(const QString &arg1);

private:
    Ui::CalibrationDialog *ui;
};

#endif // MAINWINDOW_H
