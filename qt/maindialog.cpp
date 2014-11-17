#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(QWidget *parent) :
    TrackingDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    conditions = loadConditionsFromFile("../../balls.csv");

    ui->buttonStudy1->setEnabled(false);
    ui->buttonStudy2->setEnabled(false);
    ui->buttonCalibrate->setEnabled(false);
    ui->buttonTest->setEnabled(false);

    socket = new UDPSocket(this);
    transmission.clearMessage();
    transmission.setIntensity(0);
    socket->write(transmission.getMessage());
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_buttonTest_clicked()
{
    TestDialog dialog;

    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     &dialog, SLOT(handCaptured(CapturedHand)));



    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     &dialog, SLOT(handCaptured(CapturedHand)));

    dialog.setConditions(&conditions);

    User user;
    user.number = ui->lineEdit->text().toInt();
    user.current = ui->lineEdit_2->text().toFloat();
    user.level = ui->lineEdit_3->text().toFloat();

    QRect res = QApplication::desktop()->screenGeometry(1);
    dialog.move(QPoint(res.x()+res.width()/2-dialog.width()/2, res.y()+res.height()/2-dialog.height()/2));

    dialog.setUser(user);
    dialog.setOffset(offset);
    dialog.setFeedback(ui->listWidget->currentItem()->text());

    transmission.clearMessage();
    transmission.setOff(0);
    transmission.setIntensity(0);
    socket->write(transmission.getMessage());

    dialog.exec();


}

void MainDialog::on_buttonCalibrate_clicked()
{
    //ui->buttonCalibrate->setEnabled(false);
    CalibrationDialog dialog;


    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     &dialog, SLOT(handCaptured(CapturedHand)));

    QRect res = QApplication::desktop()->screenGeometry(0);
    dialog.move(QPoint(res.x()+res.width()/2-dialog.width()/2, res.y()+res.height()/2-dialog.height()/2));
    dialog.exec();
    offset = dialog.getOffset();

    ui->buttonStudy1->setEnabled(true);
    ui->buttonStudy2->setEnabled(true);
}

void MainDialog::on_buttonStudy1_clicked()
{
    ui->buttonStudy1->setEnabled(false);
    StudyOneDialog dialog;


    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     &dialog, SLOT(handCaptured(CapturedHand)));

   // QRect screenres = QApplication::desktop()->screenGeometry(1/*screenNumber*/);

    dialog.setConditions(&conditions);

    User user;
    user.number = ui->lineEdit->text().toInt();
    user.current = ui->lineEdit_2->text().toFloat();
    user.level = ui->lineEdit_3->text().toFloat();

    QRect res = QApplication::desktop()->screenGeometry(0);
    dialog.move(QPoint(res.x()+res.width()/2-dialog.width()/2, res.y()+res.height()/2-dialog.height()/2));
    dialog.setUser(user);
    dialog.setOffset(offset);
    dialog.setFeedback(ui->listWidget->currentItem()->text());
    dialog.exec();
}

void MainDialog::on_buttonStudy2_clicked()
{
    ui->buttonStudy2->setEnabled(false);
    StudyTwoDialog dialog;


    QObject::connect(&Tracking::getInstance(), SIGNAL(handCaptured(CapturedHand)),
                     &dialog, SLOT(handCaptured(CapturedHand)));

    dialog.setConditions(&conditions);

    User user;
    user.number = ui->lineEdit->text().toInt();
    user.current = ui->lineEdit_2->text().toFloat();
    user.level = ui->lineEdit_3->text().toFloat();

    QRect res = QApplication::desktop()->screenGeometry(1);
    dialog.move(QPoint(res.x()+res.width()/2-dialog.width()/2, res.y()+res.height()/2-dialog.height()/2));
    dialog.setUser(user);
    dialog.setOffset(this->offset);
    dialog.setFeedback(ui->listWidget->currentItem()->text());
    dialog.exec();
}

void MainDialog::on_listWidget_currentTextChanged(const QString &currentText)
{
    if(currentText != "" && ui->lineEdit->text() != ""){
        ui->buttonCalibrate->setEnabled(true);
        ui->buttonTest->setEnabled(true);
    }

    //ui->lineEdit->setEnabled(false);
    //ui->listWidget->setEnabled(false);
}

void MainDialog::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1 != "" && ui->listWidget->selectedItems().size() > 0){
        ui->buttonCalibrate->setEnabled(true);
        ui->buttonTest->setEnabled(true);
    }
}
