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

    QTimer *ctimer = new QTimer(this);
    connect(ctimer, SIGNAL(timeout()), this->ui->widget_3, SLOT(updateGL()));
    connect(ctimer, SIGNAL(timeout()), this->ui->widget_2, SLOT(update()));
    ctimer->start(1000/120);
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
    handFingersVector.clear();
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

    saveTake();
    handFingersVector.clear();
}

void MainWindow::on_actionNext_take_triggered()
{
    timer.stop();
    take++;

    saveTake();
    handFingersVector.clear();
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
    for(int i=0; i<handFingersVector.size(); i++){

        for(int j=0; j<3; j++){
            if(handFingersVector[i].size() > j){
                qDebug() << handFingersVector[i].size() << "  " << j;
                myfile << handFingersVector[i][j].x() << ";";
                myfile << handFingersVector[i][j].y() << ";";
                myfile << handFingersVector[i][j].z();

                myfile << (j==3) ? "\n" : ";";
            }else{
                myfile << 0 << ";";
                myfile << 0 << ";";
                myfile << 0 << ";";

                myfile << (j==3) ? "\n" : ";";
            }
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
    handFingersVector.clear();
}

void MainWindow::on_actionRestart_take_triggered()
{
    handFingersVector.clear();
    //timer.stop();
    time.start();

}
