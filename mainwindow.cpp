#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));

    timer.start(100);
    take = 0;
    user = 1;

    ctimer = new QTimer(this);
    connect(ctimer, SIGNAL(timeout()), ui->paintWidget, SLOT(update()));
    ctimer->start(10);

    socket = new UDPSocket(this);
    dialog.setSocket(socket);
    dialog.setEMSTransmission(&transmission);




    transmission.clearMessage();
    transmission.setMinimalChangeTime(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setCapturedHand(CapturedHand *hand)
{
    this->capturedHand = hand;
}

void MainWindow::setTakes(QList<Take> *takes)
{
    this->takes = takes;
    shuffleTakes();

    ui->takeBallRadius->setText(QString::number(takes->at(0).size));
    ui->takeHardnessLineEdit->setText(QString::number(takes->at(0).hardness));
    ui->takeNameLineEdit->setText(takes->at(0).name);

    ui->paintWidget->setTake(takes->at(take));
}

void MainWindow::setCapturedTakeHandData(CapturedTakeHandData* data)
{
    capturedTakeHandData = data;
}

void MainWindow::setCapturedHandDataListener(CapturedHandDataListener *listener)
{
    this->listener = listener;
}

Take MainWindow::getCurrentTake()
{
    return takes->at(take);
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
 * @brief Clears all previous saved tracked hand positions and starts a new frame
 */
void MainWindow::on_actionStart_take_triggered()
{
    this->findChildren<QAction*>("actionNew")[0]->setEnabled(false);
    this->findChildren<QAction*>("actionNext_take")[0]->setEnabled(false);
    this->findChildren<QAction*>("actionPrevious_take")[0]->setEnabled(false);
    this->findChildren<QAction*>("actionStart_take")[0]->setEnabled(false);
    this->findChildren<QAction*>("actionFinish_take")[0]->setEnabled(true);

    //handFingersVector.clear();
    time.start();
    timer.start();
}

/**
 * @brief Updates all informations in the statusbar like take and elapsed time in current frame
 */
void MainWindow::update(){
    QString msg = "Take ";
    msg.append(QString::number(take));
    msg.append(" Time elapsed ");
    msg.append(QString::number(time.elapsed() / 1000.0f));

    ui->statusBar->showMessage(msg);

    float ps = capturedHand->fingerRadius / takes->at(take).size * 10.0f;




    ui->userFingerRadiusLineEdit->setText(QString::number(capturedHand->fingerRadius));




    int f1 =  exp(120.0f*(1-ps));

    int v = f1;
    v = (v > 127) ? 127 : v;

    if(v==INT_MIN)
        v = 127;

    ui->label_6->setText(QString::number(v));

    transmission.clearMessage();
    transmission.setIntensity(v);
    socket->write(transmission.getMessage());
}

/**
 * @brief Stops recording of current frame and save the received data in a textfile
 */
void MainWindow::on_actionFinish_take_triggered()
{
    this->findChildren<QAction*>("actionNew")[0]->setEnabled(true);
    this->findChildren<QAction*>("actionNext_take")[0]->setEnabled(true);
    this->findChildren<QAction*>("actionPrevious_take")[0]->setEnabled(true);
    this->findChildren<QAction*>("actionStart_take")[0]->setEnabled(true);
    this->findChildren<QAction*>("actionFinish_take")[0]->setEnabled(false);


    timer.stop();
}

/**
 * @brief Saves current frame in a text file and loads the previous frame if available
 */
void MainWindow::on_actionPrevious_take_triggered()
{
    if(take > 0)
        take--;

    ui->paintWidget->setTake(takes->at(take));

    saveTake();
    ui->paintWidget->setTake(takes->at(take));

    ui->takeBallRadius->setText(QString::number(takes->at(take).size));
    ui->takeHardnessLineEdit->setText(QString::number(takes->at(take).hardness));
    ui->takeNameLineEdit->setText(takes->at(take).name);

    if(take == 0){
        this->findChildren<QAction*>("actionPrevious_take")[0]->setEnabled(false);
    }

    this->findChildren<QAction*>("actionNext_take")[0]->setEnabled(true);
}

/**
 * @brief Saves current frame in a text file and loads the next frame if available
 */
void MainWindow::on_actionNext_take_triggered()
{
    this->findChildren<QAction*>("actionPrevious_take")[0]->setEnabled(true);

    if(take >= (uint)takes->size()-1){
        this->findChildren<QAction*>("actionNext_take")[0]->setEnabled(false);

        return;
    }


    timer.stop();
    saveTake();


    take++;
    ui->paintWidget->setTake(takes->at(take));



    // delete all captured finger positions
    capturedTakeHandData->hands.clear();

    // update the take information for the ui
    ui->takeBallRadius->setText(QString::number(takes->at(take).size));
    ui->takeHardnessLineEdit->setText(QString::number(takes->at(take).hardness));
    ui->takeNameLineEdit->setText(takes->at(take).name);
}

/**
 * @brief Creates a csv text file with all collected data from the current frame. The file name
 * will always be "take" current take (number) "_user_" current user (number) __timestamp__".csv."
 */
void MainWindow::saveTake(){

    QVector< QVector<QString> > values;

    // Create Captions for each column.
    QVector<QString> row;
    char chr[3] = {'X', 'Y', 'Z'};
    for(int i=0; i<5; i++){
        for(int j=0; j<3; j++){
            QString value = chr[j];
            value.push_back("(F");
            value.push_back(QString::number(i));
            value.push_back(")");

            row.push_back(value);
        }
    }
    row.push_back("X(Hand Palm)");
    row.push_back("Y(Hand Palm)");
    row.push_back("Z(Hand Palm)");
    row.push_back("Radius Hand");
    row.push_back("Radius Ball");
    row.push_back("Ball Hardness");
    row.push_back("EMS Intensity");
    values.push_back(row);

    // Concatenate several strings to the final filename
    QDateTime time(QDateTime::currentDateTime());

    QString fileName = "take";
    fileName.append(QString::number(take));
    fileName.append("_user_");
    fileName.append(QString::number(user));
    fileName.append("__");
    fileName.append(QString::number(time.toTime_t()));
    fileName.append("__");
    fileName.append(".csv");


    // save all captured finger positions
    for(int i=0; i<capturedTakeHandData->hands.size(); i++){
        row.clear();

        CapturedHand hand = capturedTakeHandData->hands[i];
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

        // save hand palm position
        row.push_back(QString::number(hand.palmPosition.x()));
        row.push_back(QString::number(hand.palmPosition.y()));
        row.push_back(QString::number(hand.palmPosition.z()));

        // radius hand
        row.push_back(QString::number(hand.fingerRadius));

        // radius ball
        row.push_back(QString::number(takes->at(take).size));

        // hardness ball
        row.push_back(QString::number(takes->at(take).hardness));

        // hardness ball
        row.push_back(QString::number(hand.emsIntensity));

        values.push_back(row);
    }

    QString msg = "File ";
    msg.append(fileName);
    msg.append(" saved");
    this->ui->statusBar->showMessage(msg);

    CSVFileHandler::saveFile(fileName, values);

}

void MainWindow::shuffleTakes()
{

    QList<Take> dummy = *takes;


    while((dummy == *takes)){
        int r = rand() % 15 + 1;

        for(int j=0; j<r; j++)
            std::random_shuffle ( takes->begin(), takes->end());

    }
}

/**
 * @brief Not implemented yet
 */
void MainWindow::on_actionSave_triggered()
{
}

/**
 * @brief Clears all captured values and increase the user number by one
 */
void MainWindow::on_actionNew_triggered()
{

    take = 0;

    user++;
    capturedTakeHandData->hands.clear();

    shuffleTakes();


    ui->takeBallRadius->setText(QString::number(takes->at(take).size));
    ui->takeHardnessLineEdit->setText(QString::number(takes->at(take).hardness));
    ui->takeNameLineEdit->setText(takes->at(take).name);

}

/**
 * @brief Clears all captured values and restart the frame time
 */
void MainWindow::on_actionRestart_take_triggered()
{
    capturedTakeHandData->hands.clear();
    time.start();
}

void MainWindow::on_actionTake_Properties_triggered()
{
    TakeDialog takeDialog;
    takeDialog.exec();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    capturedHand->fingerRadius = value / 100.0f;
}

void MainWindow::dataAdded(CapturedHand* hand)
{
    hand->emsIntensity = 666;
}
