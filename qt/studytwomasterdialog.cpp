#include "studytwomasterdialog.h"
#include "ui_studytwomasterdialog.h"

void StudyTwoMasterDialog::setAction(QString action)
{
    ui->label_3->setText(action);
}

StudyTwoMasterDialog::StudyTwoMasterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudyTwoMasterDialog)
{
    ui->setupUi(this);
}

StudyTwoMasterDialog::~StudyTwoMasterDialog()
{
    delete ui;
}

void StudyTwoMasterDialog::setFeedback(QString a)
{
    ui->label_2->setText(a);
}

void StudyTwoMasterDialog::setActionTakeBall()
{
    QString text = "LEGE Ball";
    text.append(QString::number(ballIndex));
    text.append("zurück");
    this->setAction(text);
}

void StudyTwoMasterDialog::setActionPutBall(int index)
{
    ui->label_6->setText(QString::number(count % 2));
    count++;
    ballIndex = index;
    QString text = "Ball ";
    text.append(QString::number(index+1));
    text.append("");
    this->setAction(text);
}

void StudyTwoMasterDialog::setTurn(int turn)
{
    ui->label_8->setText(QString::number(turn));
}
