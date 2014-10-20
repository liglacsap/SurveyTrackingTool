#include "studytwodialog.h"
#include "ui_studytwodialog.h"

StudyTwoDialog::StudyTwoDialog(QWidget *parent) :
    TrackingDialog(parent),
    ui(new Ui::StudyTwoDialog)
{
    ui->setupUi(this);

    socket = new UDPSocket(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(10);


    transmission.clearMessage();
    transmission.setMinimalChangeTime(1);


    ui->surveyUserLabel->setText(QString::number(condition.user));
    ui->surveyFeedbackLabel->setText(condition.feedback);

    ui->chooseWidget->hide();
    ui->ballWidget->show();

    conditionCounter = 0;

    lastTimeStamp = QDateTime::currentMSecsSinceEpoch();
}

void StudyTwoDialog::setUser(int user)
{
    this->user = user;
    ui->surveyUserLabel->setText(QString::number(this->user));

    QString path = "../Study_Results/study2_"+QString::number(user);
    path.append(".csv");
    qDebug() << path;
    file = new Study2FileHandler(path);
    qDebug() << ((Study2FileHandler*)file)->openFileForWriting();

}
void StudyTwoDialog::setConditions(QList<Condition> *conditions){
    currentConditionIndex.x = 0;
    currentConditionIndex.y = conditions->size()-1;

    swapRandom();

    this->conditions = conditions;

    ui->conditionBallRadius->setText(QString::number(conditions->at(currentConditionIndex.x).size));
    ui->conditionBall2Radius->setText(QString::number(conditions->at(currentConditionIndex.y).size));

}

StudyTwoDialog::~StudyTwoDialog()
{
    delete ui;
}
void StudyTwoDialog::on_StudyTwoDialog_destroyed()
{
    // make sure that the file gets closed. Otherwise the file will be write protected until reboot by a zombieprocess
    ((Study2FileHandler*)file)->closeFileForWriting();
}


void StudyTwoDialog::gotoNextCondition(int guessedBall){
    QVector<QString> row;
    row.push_back(QString::number(this->user));
    row.push_back(QString::number(currentConditionIndex.x));
    row.push_back(QString::number(currentConditionIndex.y));
    row.push_back(this->feedback);

    row.push_back(QString::number(conditions->at(currentConditionIndex.x).size));
    row.push_back(QString::number(conditions->at(currentConditionIndex.y).size));

    row.push_back(QString::number(feedbackTimeBall[0]));
    row.push_back(QString::number(feedbackTimeBall[1]));

    row.push_back(QString::number(guessedBall+1));
    ((Study2FileHandler*)file)->writeRow(row);

    bool res;
    if(guessedBall == 0){
        res = conditions->at(currentConditionIndex.x).size >= conditions->at(currentConditionIndex.y).size;
    }else{
        res = conditions->at(currentConditionIndex.x).size <= conditions->at(currentConditionIndex.y).size;
    }

    if(res){
        // user has guessed already one time the right ball. Therefore change the ball size
        // two a new one between the reference ball and the old one.
        if(guessedRight){
            if(currentConditionIndex.y < currentConditionIndex.x)
                currentConditionIndex.y++;
            else
                currentConditionIndex.x++;
            // = rand() % (currentConditionIndex.y+1 - currentConditionIndex.x) + currentConditionIndex.x;


            guessedRight = false;
            turns++;

        }else{
            guessedRight = true;
        }
    }else{
        guessedRight = false;

        if(currentConditionIndex.y < currentConditionIndex.x)
            currentConditionIndex.y = rand() % (currentConditionIndex.y+1);
        else
            currentConditionIndex.x = rand() % (currentConditionIndex.x+1);

        turns++;
    }

    swapRandom();

    ui->conditionBallRadius->setText(QString::number(conditions->at(currentConditionIndex.x).size));
    ui->conditionBall2Radius->setText(QString::number(conditions->at(currentConditionIndex.y).size));

    ui->ballWidget->show();
    ui->chooseWidget->hide();
    state = SIMULATE_BALL1;

    qDebug() << turns;
    if(turns == 8 && round == 0){
        QMessageBox msgBox;
        QString text = "New Study Part ";
        text.append(this->feedback);
        msgBox.setStyleSheet("background-color: #2c3e50 ; color : #f1c40f");
        msgBox.setText(text);
        msgBox.exec();

        round = 1;
        turns = 0;

        this->feedback = (this->feedback == FEEDBACK_EMS) ? FEEDBACK_REAL : FEEDBACK_EMS;
    }else if(turns == 8 && round == 1){
        QMessageBox msgBox;
        QString text = "Study 2 finisched";
        msgBox.setStyleSheet("background-color: #2c3e50 ; color : #f1c40f");
        msgBox.setText(text);
        msgBox.exec();

        saveCondition();
        this->close();
    }



}


void StudyTwoDialog::setFeedback(QString feedback){
    this->feedback = feedback;
    ui->surveyFeedbackLabel->setText(feedback);
}

void StudyTwoDialog::on_ballOneSelectedButton_clicked()
{
    gotoNextCondition(0);
}

void StudyTwoDialog::on_ballTwoSelectedButton_clicked()
{
    gotoNextCondition(1);
}

void StudyTwoDialog::swapRandom()
{
    if(rand() % 100 > 50){
        int dummy = currentConditionIndex.x;
        currentConditionIndex.x = currentConditionIndex.y;
        currentConditionIndex.y = dummy;
    }
}


void StudyTwoDialog::handCaptured(CapturedHand hand)
{
    hand.fingerRadius = hand.fingerRadius*2 - offset;
    ui->userFingerRadiusLineEdit->setText(QString::number(hand.fingerRadius));

    float size = 0;
    if(state == SIMULATE_BALL1){
        size = conditions->at(currentConditionIndex.x).size;
    }else if(state == SIMULATE_BALL2){
        size = conditions->at(currentConditionIndex.y).size;
    }else{
        intensity = 0;
        return;
    }

    float ps = hand.fingerRadius / size;
    intensity = exp(120.0f*(1-ps));
    intensity = (intensity > 127) ? 127 : intensity;
    if(intensity ==INT_MIN) intensity  = 127;
}

void StudyTwoDialog::update(){
    if(condition.feedback == FEEDBACK_EMS){
        transmission.clearMessage();
        transmission.setIntensity(intensity);
        socket->write(transmission.getMessage());
    }
}


void StudyTwoDialog::on_simulateBall2Button_clicked()
{
    state= SIMULATE_BALL2;

    ui->simulateLabel->setText("Simulate Ball 2");
    ui->simulateBall2Button->setEnabled(false);
    ui->showChooseDialogButton->setEnabled(true);

    feedbackTimeBall[0] = QDateTime::currentMSecsSinceEpoch() - lastTimeStamp;
    lastTimeStamp = QDateTime::currentMSecsSinceEpoch();
}

void StudyTwoDialog::on_showChooseDialogButton_clicked()
{
    state= SIMULATE_NONE;

    ui->simulateBall2Button->setEnabled(true);
    ui->showChooseDialogButton->setEnabled(false);

    ui->ballWidget->hide();
    ui->chooseWidget->show();

    ui->simulateLabel->setText("Simulate Ball 1");

    feedbackTimeBall[1] = QDateTime::currentMSecsSinceEpoch() - lastTimeStamp;
    lastTimeStamp = QDateTime::currentMSecsSinceEpoch();
}
