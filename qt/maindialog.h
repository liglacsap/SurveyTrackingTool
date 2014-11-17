#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QApplication>
#include <QDesktopWidget>

#include "studyonedialog.h"
#include "studytwodialog.h"
#include "calibrationdialog.h"
#include "../testdialog.h"

namespace Ui {
class MainDialog;
}

class MainDialog  : public TrackingDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = 0);
    ~MainDialog();

private slots:
    void on_buttonTest_clicked();

    void on_buttonCalibrate_clicked();

    void on_buttonStudy1_clicked();

    void on_buttonStudy2_clicked();

    void on_listWidget_currentTextChanged(const QString &currentText);

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::MainDialog *ui;

    QList<Condition> conditions;

    float offset;
};

#endif // MAINDIALOG_H
