#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&timer, SIGNAL(timeout()), ui->paintWidget, SLOT(update()));
    timer.start(10);
    user = 1;




    socket = new UDPSocket(this);
    dialog.setSocket(socket);
    dialog.setEMSTransmission(&transmission);


    bar=new QProgressBar(this);
    bar->setMaximumWidth(300);
    bar->setStyleSheet(" QProgressBar::chunk {background-color: #27ae60;width: 1px;} QProgressBar {border: 1px solid grey;border-radius: 0px;text-align: center;}");

    QWidget* empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    this->ui->mainToolBar->addWidget(empty);

    this->ui->mainToolBar->addWidget(bar);

    transmission.clearMessage();
    transmission.setMinimalChangeTime(1);

    file = new SurveyUserFileHandler("../final.csv");
    file->openFileForWriting();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setConditions(QList<Condition> *conditions)
{
    this->conditions = conditions;

    conditionsMatrix = shuffled(this->conditions->length(), 5); //latinSquare(this->conditions->length());
    currentConditionIndex.x = 0;
    currentConditionIndex.y = 0;

    ui->conditionBallRadius->setText(QString::number(conditions->at(0).size));

    ui->paintWidget->setCondition(getCurrentCondition());
}

Condition MainWindow::getCurrentCondition()
{
    return conditions->at(conditionsMatrix[currentConditionIndex.x][currentConditionIndex.y]);
}



/**
 * @brief Close the tracking tool
 */
void MainWindow::on_actionExit_triggered()
{
    this->close();
}

/**
 * @brief Show the calibration dialog
 */
void MainWindow::on_actionCalibration_triggered()
{
    dialog.exec();
}


/**
 * @brief Updates all informations in the statusbar like Condition and elapsed time in current frame
 */
void MainWindow::update(){

}

void MainWindow::gotoNextCondition()
{
    if(currentConditionIndex.x == conditionsMatrix.length()-1){

        if(currentConditionIndex.y == conditionsMatrix.length()-1){
            saveCondition();
        }else{
            currentConditionIndex.y++;
            currentConditionIndex.x=0;
        }

    }else
        currentConditionIndex.x++;

    float max = (conditionsMatrix.length()*conditionsMatrix.length()-1)*2;
    float value = (currentConditionIndex.x + (currentConditionIndex.y*conditionsMatrix.length()));
    bar->setValue((int)(value / max * 100));

    ui->surveyConditionLabel->setText(QString::number( currentConditionIndex.x ));
    ui->surveyTrialLabel->setText(QString::number( currentConditionIndex.y ));

    QString msg = "Condition ";
    msg.append(QString::number(currentConditionIndex.x));
    msg.append(" on run ");
    msg.append(QString::number(currentConditionIndex.y));

    this->ui->statusBar->showMessage(msg);

    ui->paintWidget->setCondition(getCurrentCondition());



    // update the Condition information for the ui
    ui->conditionBallRadius->setText(QString::number(getCurrentCondition().size));


}

void MainWindow::handCaptured(CapturedHand hand)
{
    ui->userFingerRadiusLineEdit->setText(QString::number(hand.fingerRadius));

    ui->userFingerRadiusPercentLineEdit->setText(QString::number((int)(hand.fingerRadius * 100 / getCurrentCondition().size)));

    ui->paintWidget->setCapturedHand(hand);

    CapturedCondition condition;
    condition.hand = hand;
    condition.user = user;
    condition.run = currentConditionIndex.y;
    condition.condition = currentConditionIndex.x;
    file->writeCapturedHand(condition);
}


/**
 * @brief Creates a csv text file with all collected data from the current frame. The file name
 * will always be "Condition" current Condition (number) "_user_" current user (number) __timestamp__".csv."
 */
void MainWindow::saveCondition(){
    file->closeFileForWriting();
/*
    QVector< QVector<QString> > values;

    // Create Captions for each column.
    QVector<QString> row;
    row.push_back("User");
    row.push_back("Condition");
    row.push_back("Run");

    char chr[3] = {'X', 'Y', 'Z'};
    for(int i=0; i<5; i++){
        for(int j=0; j<3; j++){
            QString value = chr[j];
            value.push_back("_F");
            value.push_back(QString::number(i));

            row.push_back(value);
        }
    }
    row.push_back("X_Hand_Palm");
    row.push_back("Y_Hand_Palm");
    row.push_back("Z_Hand_Palm");
    row.push_back("Radius_Hand");
    row.push_back("Radius_Ball");
    row.push_back("Ball_Hardness");
    row.push_back("EMS_Intensity");
    row.push_back("User_Selected_Ball");
    values.push_back(row);

    // Concatenate several strings to the final filename
    QDateTime time(QDateTime::currentDateTime());

    QString fileName = "../user_";
    fileName.append(QString::number(user));
    fileName.append("__");
    fileName.append(QString::number(time.toTime_t()));
    fileName.append("__");
    fileName.append(".csv");

    // save all captured finger positions
    for(int i=0; i<capturedConditionHandData->hands.size(); i++){
        row.clear();

        CapturedHand hand = capturedConditionHandData->hands[i];
        row.push_back(QString::number(hand.condition));
        row.push_back(QString::number(hand.run));

        for(int j=0; j<5; j++){
            if(hand.fingers.size() >= j){
                row.push_back(QString::number(hand.fingers[j].x()));
                row.push_back(QString::number(hand.fingers[j].y()));
                row.push_back(QString::number(hand.fingers[j].z()));
            }
            else    // save placeholder in case that less than three fingers where captured
            {
                row.push_back(QString::number(0));
                row.push_back(QString::number(0));
                row.push_back(QString::number(0));
                row.push_back(QString::number(0));
                row.push_back(QString::number(0));
            }
        }

        // save user index
        row.push_back(QString::number(hand.user));

        // save hand palm position
        row.push_back(QString::number(hand.palmPosition.x()));
        row.push_back(QString::number(hand.palmPosition.y()));
        row.push_back(QString::number(hand.palmPosition.z()));

        // radius hand
        row.push_back(QString::number(hand.fingerRadius));

        // radius of ball
        row.push_back(QString::number(getCurrentCondition().size));

        // hardness ball
        row.push_back(QString::number(getCurrentCondition().hardness));

        // ems intensity
        row.push_back(QString::number(hand.emsIntensity));

        // user selected ball index
        row.push_back(QString::number(hand.userSelectedBall));

        values.push_back(row);
    }

    QString msg = "File ";
    msg.append(fileName);
    msg.append(" saved");
    this->ui->statusBar->showMessage(msg);

    CSVFileHandler::saveFile(fileName, values);
*/
}


/**
 * @brief Clears all captured values and increase the user number by one
 */
void MainWindow::on_actionNew_triggered()
{
    user++;



    ui->conditionBallRadius->setText(QString::number(getCurrentCondition().size));
}

void MainWindow::on_actionTake_Properties_triggered()
{
/*
    ConditionDialog ConditionDialog;
    ConditionDialog.exec();
    */
}
void MainWindow::keyPressEvent(QKeyEvent* event){
    int key = event->key();
    if(key == Qt::Key_Enter || key == Qt::Key_Return){
        bool ok;
        int i = QInputDialog::getInt(this, tr("QInputDialog::getInteger()"),
                                     tr("Ball:"), 0, 1, 12, 0, &ok);
        if (ok){
            qDebug() << i;
            gotoNextCondition();

        }

    }
    if(key == Qt::Key_Y){

    }

    if(key == Qt::Key_X){

    }
}

void MainWindow::on_MainWindow_destroyed()
{
    // make sure that the file gets closed. Otherwise the file will be write protected until reboot by a zombieprocess
    file->closeFileForWriting();
}
