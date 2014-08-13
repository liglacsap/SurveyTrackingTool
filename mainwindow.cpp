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

    QTimer *ctimer = new QTimer(this);
    connect(ctimer, SIGNAL(timeout()), this->ui->widget_3, SLOT(updateGL()));
    connect(ctimer, SIGNAL(timeout()), this->ui->widget_4, SLOT(update()));
    ctimer->start(10);

    socket = new UDPSocket(this);
    dialog.setSocket(socket);
    dialog.setEMSTransmission(&transmission);

    for(int i=0; i<10; i++){
        Take t = {0, rand() % 10 + 1+(rand() % 10 / 10.0f)};

        takes.push_back(t);
    }

    ui->lineEdit->setText(QString::number(takes.at(0).size));

    transmission.clearMessage();
    transmission.setMinimalChangeTime(1);

    ui->widget_4->setTake(takes[take]);
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

    float ps = fingerRadius / takes[take].size;
//
    int v = 50;//exp((1-ps) * 100 / 10)*20;
    //qDebug() << v; //exp(ps * (-4));
    //v = (v > 127) ? 127 : v;
    transmission.clearMessage();
    transmission.setIntensity(v);
    //socket->write(transmission.getMessage());
}

/**
 * @brief Stops recording of current frame and save the received data in a textfile
 */
void MainWindow::on_actionFinish_take_triggered()
{
    timer.stop();
}

/**
 * @brief Saves current frame in a text file and loads the previous frame if available
 */
void MainWindow::on_actionPrevious_take_triggered()
{
    if(take > 1)
        take--;

    saveTake();
    handFingersVector.clear();
    ui->widget_4->setTake(takes[take]);

    ui->lineEdit->setText(QString::number(takes.at(take-1).size));
}

/**
 * @brief Saves current frame in a text file and loads the next frame if available
 */
void MainWindow::on_actionNext_take_triggered()
{
    if(take >= takes.size())
        return;

    timer.stop();

    take++;
    ui->widget_4->setTake(takes[take]);

    saveTake();
    handFingersVector.clear();

    ui->lineEdit->setText(QString::number(takes.at(take-1).size));
}

/**
 * @brief Creates a csv text file with all collected data from the current frame. The file name
 * will always be "take" current take (number) "_user_" current user (number) ".csv."
 */
void MainWindow::saveTake(){
    string Result;
    ostringstream convert;
    convert << take;

    string fileName = "take";
    fileName.append(convert.str());
    fileName.append("_user_");

    convert.clear();
    convert << user;
    fileName.append(convert.str());
    fileName.append(".csv");

    ofstream csvfile;
    csvfile.open (fileName);
    csvfile << "X;" << "Y;" << "Z;";
    csvfile << "X2;" << "Y2;" << "Z2;";
    csvfile << "X3;" << "Y3;" << "Z3;" << "Radius \n";

    srand(0);
    for(unsigned int i=0; i<handFingersVector.size(); i++){
        for(unsigned int j=0; j<3; j++){
            if(handFingersVector[i].size() > j){
                csvfile << handFingersVector[i][j].x() << ";";
                csvfile << handFingersVector[i][j].y() << ";";
                csvfile << handFingersVector[i][j].z();

                csvfile << ((j==3) ? "\n" : ";");
            }else{
                csvfile << 0 << ";";
                csvfile << 0 << ";";
                csvfile << 0 << ";";

                csvfile << ((j==3) ? "\n" : ";");
            }
        }
    }

    csvfile.close();
}

/**
 * @brief Not implemented yet
 */
void MainWindow::on_actionSave_triggered()
{
}

/**
 * @brief Clears all captured values and increament the user number by one
 */
void MainWindow::on_actionNew_triggered()
{
    user++;
    handFingersVector.clear();
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
