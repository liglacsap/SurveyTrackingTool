#ifndef TRACKINGWINDOW_H
#define TRACKINGWINDOW_H

#include <QDialog>


#include "../ems/emstransmission.h"
#include "../ems/udpsocket.h"

#include "../csvfilehandler.h"

#include <QProgressBar>
#include <QSpacerItem>
#include <QKeyEvent>
#include <QInputDialog>

struct Marker{
    double position[3];
    string name;
};

struct ConditionIndex{
    int x;
    int y;
};

struct User{
    int number;
    float current;
    float level;
    float resistance;
};



class TrackingDialog : public QDialog{
    Q_OBJECT
public:
     inline void setConditions(QList<Condition> *conditions){
         this->conditions = conditions;
     }

     inline explicit TrackingDialog(QWidget *parent) :
         QDialog(parent)
     {
     }


     inline void setUser(User user){
        this->user = user;
     }
     inline void setOffset(double offset){
        this->offset = offset;
     }
     inline void setFeedback(QString feedback){
        this->feedback = feedback;
     }

protected:
    QList<Condition>* conditions;

    QList<QList<int>> conditionsMatrix;
    ConditionIndex currentConditionIndex;
    CapturedCondition condition;

    int conditionCounter;




    inline Condition getCurrentCondition(){
        return conditions->at(conditionsMatrix[currentConditionIndex.y][currentConditionIndex.x]);
    }

protected slots:
       inline void gotoNextCondition(){
            if(currentConditionIndex.x == conditionsMatrix[1].length()-1){

                if(currentConditionIndex.y == conditionsMatrix.length()-1){
                    condition.feedback = (condition.feedback == FEEDBACK_EMS) ? FEEDBACK_REAL : FEEDBACK_EMS;

                    if(conditionCounter-1 == (conditionsMatrix[1].length()*conditionsMatrix.length()-1)*2 ){
                        QMessageBox msgBox;
                        QString text = "Study 1 finisched ";
                        text.append(condition.feedback);
                        msgBox.setStyleSheet("background-color: #2c3e50 ; color : #f1c40f");
                        msgBox.setText(text);
                        msgBox.exec();

                        saveCondition();
                        this->close();
                    }else{
                        QMessageBox msgBox;
                        QString text = "New Study Part ";
                        text.append(condition.feedback);
                        msgBox.setStyleSheet("background-color: #2c3e50 ; color : #f1c40f");
                        msgBox.setText(text);
                        msgBox.exec();
                    }

                    currentConditionIndex.x=0;
                    currentConditionIndex.y=0;
                }else{
                    currentConditionIndex.y++;
                    currentConditionIndex.x=0;
                }

            }else
                currentConditionIndex.x++;


        }

public slots:
    inline void handCaptured(CapturedHand hand){

    }
protected:
    /**
     * @brief Saves the current Condition in an CSV file.
     */
    inline void saveCondition(){
            ((Study1FileHandler*)file)->closeFileForWriting();
    }

protected:
    // for the communication with the Arduino Board / EMS System
    UDPSocket *socket;

    // current user. Used for the identification later in the study
    User user;
    double offset;
    QString feedback;



    EMSTransmission transmission;

    CSVFileHandler* file;
};

#endif // TRACKINGWINDOW_H




