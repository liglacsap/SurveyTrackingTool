#ifndef TAKEDIALOG_H
#define TAKEDIALOG_H

#include <QDialog>
#include <QDebug>
#include "globals.h"
#include "csvfilehandler.h"

namespace Ui {
class TakeDialog;
}

class TakeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TakeDialog(QWidget *parent = 0);
    ~TakeDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:
    Ui::TakeDialog *ui;

    vector<Take> takes;
};

#endif // TAKEDIALOG_H
