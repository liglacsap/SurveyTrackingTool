#ifndef CONDITIONDIALOG_H
#define CONDITIONDIALOG_H

#include <QDialog>
#include <QDebug>
#include "../csvfilehandler.h"


namespace Ui {
class ConditionDialog;
}

class ConditionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConditionDialog(QWidget *parent = 0);
    ~ConditionDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:
    Ui::ConditionDialog *ui;

    vector<Condition> Conditions;
};

#endif // ConditionDIALOG_H
