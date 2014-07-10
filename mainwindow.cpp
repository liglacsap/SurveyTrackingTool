#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));

    timer.start(100);
    take = 1;
    user = 1;

    qsrand(qrand());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionCalibration_triggered()
{
    ConfigurationDialog dialog;
    dialog.exec();
}

void MainWindow::on_actionStart_take_triggered()
{

    time.start();
   // time.restart();

    timer.start();
}

void MainWindow::update(){
    QString msg = "Take ";
    msg.append(QString::number(take));
    msg.append(" Time elapsed ");
    msg.append(QString::number(time.elapsed() / 1000.0f));

    ui->statusBar->showMessage(msg);
}

void MainWindow::on_actionFinish_take_triggered()
{

    timer.stop();
}


void MainWindow::on_actionPrevious_take_triggered()
{
    if(take > 1)
        take--;
}

void MainWindow::on_actionNext_take_triggered()
{
    take++;

    saveTake();
}

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

    ofstream myfile;
    myfile.open (fileName);
    myfile << "X;" << "Y;" << "Z;";
    myfile << "X2;" << "Y2;" << "Z2;";
    myfile << "X3;" << "Y3;" << "Z3;" << "Radius \n";

    srand(0);
    for(int i=0; i<10000; i++){
        for(int j=0; j<4; j++){
            myfile << rand() / static_cast<double>( RAND_MAX ) << ";";
            myfile << rand() / static_cast<double>( RAND_MAX ) << ";";
            myfile << rand() / static_cast<double>( RAND_MAX );

            if(j == 3)
                myfile << "\n";
            else
                myfile << ";";
        }
    }

    myfile.close();
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "",
                               tr("Tables (*.xlsx)"));

    vector<float*> position;




}

void MainWindow::on_actionNew_triggered()
{
    user++;
}
