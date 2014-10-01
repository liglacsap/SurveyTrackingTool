#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(&timer, SIGNAL(timeout()), ui->paintWidget, SLOT(update()));
    //timer.start(10);
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

    ui->gotoNextCondition->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCapturedHand(CapturedHand *hand)
{
    this->capturedHand = hand;
}

void MainWindow::setConditions(QList<Condition> *conditions)
{

    this->conditions = conditions;

    conditionsMatrix = latinSquare(this->conditions->length());
    currentConditionIndex.x = 0;
    currentConditionIndex.y = 0;

    ui->conditionBallRadius->setText(QString::number(conditions->at(0).size));
    ui->conditionHardnessLineEdit->setText(QString::number(conditions->at(0).hardness));
    ui->conditionNameLineEdit->setText(conditions->at(0).name);

    ui->paintWidget->setCondition(getCurrentCondition());

    ui->userSelectedBallLineEdit->setValidator(new QIntValidator(1, conditions->size(), this));
}

void MainWindow::setCapturedConditionHandData(CapturedConditionHandData* data)
{
    capturedConditionHandData = data;
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

    float ps = capturedHand->fingerRadius / getCurrentCondition().size * 10.0f;

    ui->userFingerRadiusLineEdit->setText(QString::number(capturedHand->fingerRadius));

    capturedHand->condition = conditionsMatrix[currentConditionIndex.x][currentConditionIndex.y];
    capturedHand->run = currentConditionIndex.x;
    capturedHand->user = user;


    int f1 =  exp(120.0f*(1-ps));

    int v = f1;
    v = (v > 127) ? 127 : v;

    if(v==INT_MIN)
        v = 127;


    transmission.clearMessage();
    transmission.setIntensity(v);
    socket->write(transmission.getMessage());

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

    float max = conditionsMatrix.length()*conditionsMatrix.length()-1;
    float value = (currentConditionIndex.x + (currentConditionIndex.y*conditionsMatrix.length()));
    bar->setValue((int)(value / max * 100));

    QString msg = "Condition ";
    msg.append(QString::number(currentConditionIndex.x));
    msg.append(" on run ");
    msg.append(QString::number(currentConditionIndex.y));

    this->ui->statusBar->showMessage(msg);

    ui->paintWidget->setCondition(getCurrentCondition());



    // update the Condition information for the ui
    ui->conditionBallRadius->setText(QString::number(getCurrentCondition().size));
    ui->conditionHardnessLineEdit->setText(QString::number(getCurrentCondition().hardness));
    ui->conditionNameLineEdit->setText(getCurrentCondition().name);


    for(int i=capturedConditionHandData->hands.size()-1; i>=0; i--){
        if(capturedConditionHandData->hands[i].userSelectedBall == -1){
            capturedConditionHandData->hands[i].userSelectedBall = ui->userSelectedBallLineEdit->text().toInt();
        }else
            break;
    }

}


/**
 * @brief Creates a csv text file with all collected data from the current frame. The file name
 * will always be "Condition" current Condition (number) "_user_" current user (number) __timestamp__".csv."
 */
void MainWindow::saveCondition(){

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

}


/**
 * @brief Clears all captured values and increase the user number by one
 */
void MainWindow::on_actionNew_triggered()
{

    user++;
    capturedConditionHandData->hands.clear();



    ui->conditionBallRadius->setText(QString::number(getCurrentCondition().size));
    ui->conditionHardnessLineEdit->setText(QString::number(getCurrentCondition().hardness));
    ui->conditionNameLineEdit->setText(getCurrentCondition().name);

}

void MainWindow::on_actionTake_Properties_triggered()
{
    ConditionDialog ConditionDialog;
    ConditionDialog.exec();
}


void MainWindow::dataAdded(CapturedHand* hand)
{
    hand->emsIntensity = 666;
}


void MainWindow::on_gotoNextCondition_clicked()
{
    gotoNextCondition();
    ui->userSelectedBallLineEdit->setText("");
    ui->userSelectedBallLineEdit->setFocus();

}

void MainWindow::on_userSelectedBallLineEdit_textChanged(const QString &arg1)
{
     ui->gotoNextCondition->setEnabled(!arg1.isEmpty() && arg1 != "0");
}
