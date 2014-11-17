#ifndef STUDYTWOMASTERDIALOG_H
#define STUDYTWOMASTERDIALOG_H

#include <QDialog>

namespace Ui {
class StudyTwoMasterDialog;
}

class StudyTwoMasterDialog : public QDialog
{
    Q_OBJECT

private:
    void setAction(QString action);
public:
    explicit StudyTwoMasterDialog(QWidget *parent = 0);
    ~StudyTwoMasterDialog();

    void setFeedback(QString);
    void setActionTakeBall();
    void setActionPutBall(int index);
    void setTurn(int turn);
private:
    Ui::StudyTwoMasterDialog *ui;

    int ballIndex;
    int count = 0;
};

#endif // STUDYTWOMASTERDIALOG_H
