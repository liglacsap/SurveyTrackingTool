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

    ui->takeBallRadius->setText(QString::number(takes.at(0).size));
    ui->takeHardnessLineEdit->setText(QString::number(takes.at(0).hardness));
    ui->takeNameLineEdit->setText(takes.at(0).name);

    ui->paintWidget->setTake(takes[take]);

    transmission.clearMessage();
    transmission.setMinimalChangeTime(1);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    handFingersVector.clear();
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

    float ps = fingerRadius / takes[take].size * 10.0f;




    ui->userFingerRadiusLineEdit->setText(QString::number(fingerRadius));




    int f1 =  exp(120.0f*(1-ps));

    int v = f1;
    v = (v > 127) ? 127 : v;

    if(v==INT_MIN)
        v = 127;

    //qDebug() << "PS:" << (1-ps) << " F1:" << v;

   // qDebug() << v; //exp(ps * (-4));
    ui->label_6->setText(QString::number(v));
    //v = (v > 127) ? 127 : v;
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

    ui->paintWidget->setTake(takes[take]);

    saveTake();
    handFingersVector.clear();
    ui->paintWidget->setTake(takes[take]);

    ui->takeBallRadius->setText(QString::number(takes.at(take).size));
    ui->takeHardnessLineEdit->setText(QString::number(takes.at(take).hardness));
    ui->takeNameLineEdit->setText(takes.at(take).name);
}

/**
 * @brief Saves current frame in a text file and loads the next frame if available
 */
void MainWindow::on_actionNext_take_triggered()
{

    if(take >= takes.size()-1)
        return;

   // timer.stop();

    take++;
    ui->paintWidget->setTake(takes[take]);

    saveTake();

    // delete all captured finger positions
    handFingersVector.clear();



    // update the take information for the ui
    ui->takeBallRadius->setText(QString::number(takes.at(take).size));
    ui->takeHardnessLineEdit->setText(QString::number(takes.at(take).hardness));
    ui->takeNameLineEdit->setText(takes.at(take).name);


    //ctimer->start(10);
}

/**
 * @brief Creates a csv text file with all collected data from the current frame. The file name
 * will always be "take" current take (number) "_user_" current user (number) __timestamp__".csv."
 */
void MainWindow::saveTake(){
    QVector< QVector<QString> > values;

    // Create Captions for each column.
    QVector<QString> label;
    char chr[3] = {'X', 'Y', 'Z'};
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            QString value = chr[j];
            value.push_back("(F");
            value.push_back(QString::number(i));
            value.push_back(")");

            label.push_back(value);
        }
    }
    label.push_back("Radius Hand");
    label.push_back("Radius Ball");
    label.push_back("Ball Hardness");
    values.push_back(label);

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
    for(unsigned int i=0; i<handFingersVector.size(); i++){
        label.clear();
        for(unsigned int j=0; j<3; j++){
            if(handFingersVector[i].size() > j){
                label.push_back(QString::number(handFingersVector[i][j].x()));
                label.push_back(QString::number(handFingersVector[i][j].y()));
                label.push_back(QString::number(handFingersVector[i][j].z()));
            }
            else    // save placeholder in case that less than three fingers where captured
            {
                label.push_back(QString::number(0));
                label.push_back(QString::number(0));
                label.push_back(QString::number(0));
            }
        }

        values.push_back(label);
    }

    CSVFileHandler::saveFile(fileName, values);
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
    handFingersVector.clear();

    ui->takeBallRadius->setText(QString::number(takes.at(take).size));
    ui->takeHardnessLineEdit->setText(QString::number(takes.at(take).hardness));
    ui->takeNameLineEdit->setText(takes.at(take).name);
}

/**
 * @brief Clears all captured values and restart the frame time
 */
void MainWindow::on_actionRestart_take_triggered()
{
    handFingersVector.clear();
    time.start();
}

void MainWindow::on_actionTake_Properties_triggered()
{
    TakeDialog takeDialog;
    takeDialog.exec();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    fingerRadius = value / 100.0f;
}
