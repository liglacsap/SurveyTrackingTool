#include "studyonedialog.h"
#include "ui_studyonedialog.h"

StudyOneDialog::StudyOneDialog(QWidget *parent) :
    TrackingDialog(parent),
    ui(new Ui::StudyOneDialog)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&timer, SIGNAL(timeout()), ui->paintWidget, SLOT(update()));
    timer.start(10);





    socket = new UDPSocket(this);
    dialog.setSocket(socket);
    dialog.setEMSTransmission(&transmission);




    transmission.clearMessage();
    transmission.setMinimalChangeTime(1);

    transmission.clearMessage();
    transmission.setIntensity(0);
    transmission.setOn(0);
    socket->write(transmission.getMessage());


    ui->surveyUserLabel->setText(QString::number(condition.user));
    ui->surveyFeedbackLabel->setText(condition.feedback);

    conditionCounter = 0;

    lastTimeStamp = QDateTime::currentMSecsSinceEpoch();
}

StudyOneDialog::~StudyOneDialog()
{
    delete ui;
}


void StudyOneDialog::setConditions(QList<Condition> *conditions)
{
    TrackingDialog::setConditions(conditions);

    this->conditions = conditions;

    conditionsMatrix = shuffled(this->conditions->length(), 5); //latinSquare(this->conditions->length());

    currentConditionIndex.x = 0;
    currentConditionIndex.y = 0;

    ui->conditionBallRadius->setText(QString::number(conditions->at(conditionsMatrix[0][0]).size));
    ui->paintWidget->setCondition(getCurrentCondition());
}

void StudyOneDialog::setUser(User user)
{
    this->user = user;
    ui->surveyUserLabel->setText(QString::number(this->user.number));

    QString path = "../../Study_Results/"+QString::number(user.number);
    path.append("_ems_vs_real_");
    path.append(QString::number(QDateTime::currentMSecsSinceEpoch()));
    path.append(".csv");
    qDebug() << path;
    file = new Study1FileHandler(path);
    ((Study1FileHandler*)file)->openFileForWriting();
}

void StudyOneDialog::setFeedback(QString feedback){
    this->feedback = feedback;
    condition.feedback = feedback;
    ui->surveyFeedbackLabel->setText(feedback);
}

void StudyOneDialog::gotoNextCondition(){
    if(currentConditionIndex.x == conditionsMatrix[1].length()-1){

        if(currentConditionIndex.y == conditionsMatrix.length()-1){
            condition.feedback = (condition.feedback == FEEDBACK_EMS) ? FEEDBACK_REAL : FEEDBACK_EMS;

            if(conditionCounter-1 == (conditionsMatrix[1].length()*conditionsMatrix.length()-1) ){
            }else{
                QMessageBox msgBox;
                QString text = "Study 2 finished";
                msgBox.setStyleSheet("background-color: #2c3e50 ; color : #f1c40f");
                msgBox.setText(text);
                msgBox.exec();

                saveCondition();
                this->close();
            }

            currentConditionIndex.x=0;
            currentConditionIndex.y=0;
        }else{
            currentConditionIndex.y++;
            currentConditionIndex.x=0;
        }

    }else
        currentConditionIndex.x++;



    //float max = (conditionsMatrix[0].length()*conditionsMatrix.length()-1);
    //float value = (currentConditionIndex.x + (currentConditionIndex.y*conditionsMatrix[0].length()));


    ui->surveyConditionLabel->setText(QString::number( currentConditionIndex.x ));
    ui->surveyTrialLabel->setText(QString::number( currentConditionIndex.y ));

    ui->paintWidget->setCondition(getCurrentCondition());



    // update the Condition information for the ui
    ui->conditionBallRadius->setText(QString::number(getCurrentCondition().size));

    ui->surveyFeedbackLabel->setText(condition.feedback);

    conditionCounter++;
}



/**
 * @brief Close the tracking tool
 */
void StudyOneDialog::on_actionExit_triggered()
{
    this->close();
}

/**
 * @brief Show the calibration dialog
 */
void StudyOneDialog::on_actionCalibration_triggered()
{
    dialog.exec();
}


/**
 * @brief Updates all informations in the statusbar like Condition and elapsed time in current frame
 */
void StudyOneDialog::update(){
    if(condition.feedback == FEEDBACK_EMS){
    transmission.clearMessage();
    transmission.setIntensity(v);
    socket->write(transmission.getMessage());
    }
}



void StudyOneDialog::handCaptured(CapturedHand hand)
{
    hand.fingerRadius = hand.fingerRadius*2 - offset;

    ui->userFingerRadiusLineEdit->setText(QString::number(hand.fingerRadius));

    ui->userFingerRadiusPercentLineEdit->setText(QString::number((int)(hand.fingerRadius * 100 / getCurrentCondition().size)));

    ui->paintWidget->setCapturedHand(hand);


    float ps = hand.fingerRadius /getCurrentCondition().size;
    int f1 = exp(120.0f*(1-ps));

    v = f1;
    v = (v > 100) ? 100 : v;
    if(v==INT_MIN)
    v = 100;



       ui->userIntensityLabel->setText(QString::number(v));


    condition.ballSize = getCurrentCondition().size;
    condition.intensity = v;
    condition.hand = hand;
    condition.user = user.number;
    condition.run = currentConditionIndex.y;
    condition.condition = currentConditionIndex.x;
    ((Study1FileHandler*)file)->writeCapturedHand(condition);
}



void StudyOneDialog::on_actionTake_Properties_triggered()
{
/*
    ConditionDialog ConditionDialog;
    ConditionDialog.exec();
    */
}



void StudyOneDialog::on_StudyOneDialog_destroyed()
{
    // make sure that the file gets closed. Otherwise the file will be write protected until reboot by a zombieprocess
    ((Study1FileHandler*)file)->closeFileForWriting();
}


void StudyOneDialog::keyPressEvent(QKeyEvent* event){
    int key = event->key();
    if(key == Qt::Key_Enter || key == Qt::Key_Return){



        condition.time = QDateTime::currentMSecsSinceEpoch()-lastTimeStamp;
        lastTimeStamp = QDateTime::currentMSecsSinceEpoch();

        bool ok;
        int i = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"),
                                     tr("Ball:"), 0, 1, 12, 0, &ok);

        condition.userSelectedBall = i;
        if (ok){
            gotoNextCondition();
        }

    }
    if(key == Qt::Key_Y){

    }

    if(key == Qt::Key_X){

    }
}

void StudyOneDialog::on_horizontalSlider_sliderMoved(int position)
{
    float ps = position/100.0f /getCurrentCondition().size;
    int f1 = exp(120.0f*(1-ps));

    v = f1;
    v = (v > 100) ? 100 : v;
    if(v==INT_MIN)
    v = 100;

    qDebug() << ps << "   " << f1;
}
