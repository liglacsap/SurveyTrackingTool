#ifndef TESTDIALOG_H
#define TESTDIALOG_H

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


#include "qt/trackingdialog.h"

namespace Ui {
class TestDialog;
}

class TestDialog : public TrackingDialog
{
    Q_OBJECT

public:
    explicit TestDialog(QWidget *parent = 0);
    ~TestDialog();
public slots:
        void update();
        void handCaptured(CapturedHand hand);
private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_TestDialog_destroyed();

    void on_pushButton_clicked();


    void on_pushButton_2_toggled(bool checked);

private:
    Ui::TestDialog *ui;

    int intensity;
    QTimer timer;
    bool checked = false;
};

#endif // TESTDIALOG_H
