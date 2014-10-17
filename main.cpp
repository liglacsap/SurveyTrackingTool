
#include "qt/maindialog.h"
#include "qt/calibrationdialog.h"
#include <QApplication>
#include <QMessageBox>
#include <QString>
#include <QTranslator>
#include <QVector3D>
#include <QObject>

#include "tracking.h"


MainDialog* w;

int main(int argc, char *argv[])
{
    srand (time(NULL));

    QApplication a(argc, argv);

    // 0 ist der Pfad
    //a.arguments()[1]



    w = new MainDialog();
    //w->setConditions(&Conditions);
    w->show();

    // qt related stuff
    qRegisterMetaType<CapturedHand>("CapturedHand");



#ifdef _WIN32
    int result = Tracking::getInstance().createClient();
    if (result != ErrorCode_OK){
        QMessageBox msg;
        msg.setText("The connection to the OptiTrack tracking system couldn't be established \n");

        QString detailed;

        switch(result){
        case 1:
            detailed = "An internal error occured. Make sure, that no other application uses the OptiTrack system right now.";
            break;
        case 2:
            detailed = "An external error occured. Please verify the connections to all cameras and hubs of the OptiTrack system.";
            break;
        case 3:
            detailed = "A network error occured. Check all options in Motive:Tracker and make sure that the pc is connected to the right network and uses a available ip adress";
            break;
        }

        msg.setDetailedText(detailed);

        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Save);
        msg.setIcon(QMessageBox::Critical);
        int res = msg.exec();

        // if the user cancels
        if(res==QMessageBox::Cancel) return -1;
    }
#endif

    return a.exec();
}
