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

    masterDialog.setFeedback(feedback);
    ui->widget_2->hide();

    transmission.clearMessage();
    transmission.setIntensity(0);
    socket->write(transmission.getMessage());

    masterDialog.show();
}

void StudyTwoDialog::setUser(User user)
{
    this->user = user;
    ui->surveyUserLabel->setText(QString::number(this->user.number));

    QString path = "../../Study_Results/"+QString::number(user.number);
    path.append("_jnd_");
    path.append(QString::number(QDateTime::currentMSecsSinceEpoch()));
    path.append(".csv");
    qDebug() << path;
    file = new Study2FileHandler(path);
    qDebug() << ((Study2FileHandler*)file)->openFileForWriting();

}
void StudyTwoDialog::setConditions(QList<Condition> *conditions){
    referenceBallIndex = conditions->size()-1;
    secondBallIndex = 0;

    ballsSwapped = swapRandom();
    ui->userFingerRadiusLineEdit->setText(ballsSwapped ? "TRUE": "FALSE");

    this->conditions = conditions;

    if(!ballsSwapped){
        ui->conditionBallRadius->setText(QString::number(conditions->at(referenceBallIndex).size));
        ui->conditionBall2Radius->setText(QString::number(conditions->at(secondBallIndex).size));
    }else{
        ui->conditionBall2Radius->setText(QString::number(conditions->at(referenceBallIndex).size));
        ui->conditionBallRadius->setText(QString::number(conditions->at(secondBallIndex).size));
    }

    masterDialog.setActionPutBall((!ballsSwapped) ? referenceBallIndex : secondBallIndex);
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
    ui->simulateBall2Button->setEnabled(true);

    QVector<QString> row;
    row.push_back(QString::number(this->user.number));

    row.push_back(QString::number(user.current));
    row.push_back(QString::number(user.level));
    row.push_back(QString::number(offset));
    row.push_back(this->feedback);
    row.push_back(QString::number(stepSize));
    row.push_back(QString::number(QDateTime::currentMSecsSinceEpoch()));

    if(!ballsSwapped){
        row.push_back(QString::number(referenceBallIndex));
        row.push_back(QString::number(secondBallIndex));
    }else{
        row.push_back(QString::number(secondBallIndex));
        row.push_back(QString::number(referenceBallIndex));
    }


    if(!ballsSwapped){
        row.push_back(QString::number(conditions->at(referenceBallIndex).size));
        row.push_back(QString::number(conditions->at(secondBallIndex).size));
    }else{
        row.push_back(QString::number(conditions->at(secondBallIndex).size));
        row.push_back(QString::number(conditions->at(referenceBallIndex).size));
    }

    row.push_back(QString::number(abs(secondBallIndex-referenceBallIndex)));
    row.push_back(QString::number(abs(conditions->at(secondBallIndex).size-conditions->at(referenceBallIndex).size)));

    row.push_back(QString::number(feedbackTimeBall[0]));
    row.push_back(QString::number(feedbackTimeBall[1]));

    row.push_back(QString::number(guessedBall+1));



    bool res;
    if(ballsSwapped){
        res = (guessedBall == 0);
    }else{        
        res = (guessedBall == 1);
    }



    row.push_back(res ? "" : "true");
    row.push_back(QString::number(turns));

    ((Study2FileHandler*)file)->writeRow(row);

     if(res && (secondBallIndex != referenceBallIndex)){
        // user has guessed already one time the right ball. Therefore change the ball size
        // too a new one between the reference ball and the old one.
        if(guessedRight){
            secondBallIndex += stepSize;
            if(secondBallIndex > referenceBallIndex)
                secondBallIndex = referenceBallIndex;

            guessedRight = false;
            //turns++;

        }else{
            guessedRight = true;
        }

    }else{

        guessedRight = false;
        stepSize = 1;

        if(secondBallIndex - 1 >= 0)
            secondBallIndex--;
        turns++;

        masterDialog.setTurn(turns);
    }

    ballsSwapped = swapRandom();

    if(!ballsSwapped){
        ui->conditionBallRadius->setText(QString::number(conditions->at(referenceBallIndex).size));
        ui->conditionBall2Radius->setText(QString::number(conditions->at(secondBallIndex).size));
    }else{
        ui->conditionBallRadius->setText(QString::number(conditions->at(secondBallIndex).size));
        ui->conditionBall2Radius->setText(QString::number(conditions->at(referenceBallIndex).size));
    }

    ui->userFingerRadiusLineEdit->setText(ballsSwapped ? "TRUE": "FALSE");

    masterDialog.setActionPutBall((!ballsSwapped) ? referenceBallIndex : secondBallIndex);

    ui->ballWidget->show();
    ui->chooseWidget->hide();
    state = SIMULATE_BALL1;

    if(turns == 8 && round == 0){
        referenceBallIndex = conditions->size()-1;
        secondBallIndex = 0;
        stepSize = 2;

        QMessageBox msgBoxUser;
        msgBoxUser.setText("Bitte auf Anweisung warten");
        msgBoxUser.setStyleSheet("background-color: #2c3e50 ; color : #f1c40f; font: 28px bold");
        msgBoxUser.show();

        QMessageBox msgBox;
        QString text = "Studie 1, zweiter Teil: ";
        if(this->feedback == FEEDBACK_EMS)
            text.append("Real");
        else
            text.append("EMS");

        msgBox.setStyleSheet("background-color: #2c3e50 ; color : #f1c40f");
        msgBox.setText(text);
        QRect res = QApplication::desktop()->screenGeometry(0);
        msgBox.move(QPoint(res.x()+res.width()/2-msgBox.width()/2, res.y()+res.height()/2-msgBox.height()/2));
        msgBox.exec();

        round = 1;
        turns = 0;

        this->feedback = (this->feedback == FEEDBACK_EMS) ? FEEDBACK_REAL : FEEDBACK_EMS;
        masterDialog.setFeedback(feedback);
        this->ui->surveyFeedbackLabel->setText(this->feedback);

        if(feedback == FEEDBACK_EMS)
            ui->simulateLabel->setText("<html><head/><body><p><span style=\" font-size:24pt; font-weight:600;\">Simuliere Ball 1,</span></p><p><span style=\" font-size:18pt;\">Bevor du fortfahren kannst, musst du deine Hand </span><span style=\" font-size:18pt; text-decoration: underline;\">öffnen</span><span style=\" font-size:18pt; font-style:italic;\">.</span></p></body></html>");
        else
            ui->simulateLabel->setText("<html><head/><body><p><span style=\" font-size:24pt; font-weight:600;\">Ertaste Ball 1,</span></p><p><span style=\" font-size:18pt;\">Bevor du Ball 2 ertasten kannst, warte bitte auf das Zeichen </span><span style=\" font-size:18pt; font-style:italic;\">.</span></p></body></html>");


    }else if(turns == 8 && round == 1){
        QMessageBox msgBox;
        QString text = "Study 2 finished";
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
    masterDialog.setFeedback(feedback);
}

void StudyTwoDialog::on_ballOneSelectedButton_clicked()
{
    gotoNextCondition(0);
}

void StudyTwoDialog::on_ballTwoSelectedButton_clicked()
{
    gotoNextCondition(1);
}

bool StudyTwoDialog::swapRandom()
{
   return (rand() % 100 > 50);
}


void StudyTwoDialog::handCaptured(CapturedHand hand)
{
    timeStamp = QDateTime::currentMSecsSinceEpoch();

    hand.fingerRadius = hand.fingerRadius*2 - offset;
    ui->userFingerRadiusLineEdit->setText(QString::number(hand.fingerRadius));

    float size = 0;
    if(state == SIMULATE_BALL1){
        if(!ballsSwapped)
            size = conditions->at(referenceBallIndex).size;
        else
            size = conditions->at(secondBallIndex).size;
    }else if(state == SIMULATE_BALL2){
        if(!ballsSwapped)
            size = conditions->at(secondBallIndex).size;
        else
            size = conditions->at(referenceBallIndex).size;

    }else{
        intensity = 0;
        return;
    }

    qDebug() << size;

    if(feedback == FEEDBACK_EMS){
    if(hand.fingerRadius > 10 && state == SIMULATE_BALL1){
      ui->simulateBall2Button->setEnabled(true);
    }
    else
        ui->simulateBall2Button->setEnabled(false);
    }else{
        ui->simulateBall2Button->setEnabled(true);
    }



    float ps = hand.fingerRadius / size;
    intensity = exp(120.0f*(1-ps));
    intensity = (intensity > 100) ? 100 : intensity;
    if(intensity ==INT_MIN) intensity  = 100;
}

void StudyTwoDialog::update(){
    if(this->feedback == FEEDBACK_EMS){
        transmission.clearMessage();

       // qDebug() << QDateTime::currentMSecsSinceEpoch() - timeStamp;
        if(QDateTime::currentMSecsSinceEpoch() - timeStamp > 3000){
        //if(intensity == 0){
            transmission.setIntensity(0);
        }else{
            transmission.setIntensity(intensity);
        }

        //transmission.setIntensity(intensity);
        socket->write(transmission.getMessage());
    }
}


void StudyTwoDialog::on_simulateBall2Button_clicked()
{
    state= SIMULATE_BALL2;

     QString text;
    if(this->feedback == FEEDBACK_EMS){
        text = "<html><head/><body><p><span style=\" font-size:24pt; font-weight:600;\">Simuliere Ball 2</span></p></body></html>";

    }else{
        text = "<html><head/><body><p><span style=\" font-size:24pt; font-weight:600;\">Ertaste Ball 2</span></p></body></html>";
    }
    ui->simulateLabel->setText(text);
    ui->simulateBall2Button->setEnabled(false);


    ui->showChooseDialogButton->setEnabled(true);


    // Beware order is now reversed because we want to display the second ball
    masterDialog.setActionPutBall((ballsSwapped) ? referenceBallIndex : secondBallIndex);

    feedbackTimeBall[0] = QDateTime::currentMSecsSinceEpoch() - lastTimeStamp;
    lastTimeStamp = QDateTime::currentMSecsSinceEpoch();
}

void StudyTwoDialog::on_showChooseDialogButton_clicked()
{
    state= SIMULATE_NONE;

    ui->showChooseDialogButton->setEnabled(false);

    ui->ballWidget->hide();
    ui->chooseWidget->show();

    if(feedback == FEEDBACK_EMS)
        ui->simulateLabel->setText("<html><head/><body><p><span style=\" font-size:24pt; font-weight:600;\">Simuliere Ball 1,</span></p><p><span style=\" font-size:18pt;\">Bevor du fortfahren kannst, musst du deine Hand </span><span style=\" font-size:18pt; text-decoration: underline;\">öffnen</span><span style=\" font-size:18pt; font-style:italic;\">.</span></p></body></html>");
    else
        ui->simulateLabel->setText("<html><head/><body><p><span style=\" font-size:24pt; font-weight:600;\">Ertaste Ball 1,</span></p><p><span style=\" font-size:18pt;\">Bevor du Ball 2 ertasten kannst, warte bitte auf das Zeichen </span><span style=\" font-size:18pt; font-style:italic;\">.</span></p></body></html>");

    feedbackTimeBall[1] = QDateTime::currentMSecsSinceEpoch() - lastTimeStamp;
    lastTimeStamp = QDateTime::currentMSecsSinceEpoch();
}
