#ifndef PAINT2DWIDGET_H
#define PAINT2DWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "globals.h"
#include "conditiondialog.h"

extern CapturedHand capturedHand;

class Paint2DWidget : public QWidget
{
    Q_OBJECT

private:
    Condition condition;
public:
    explicit Paint2DWidget(QWidget *parent = 0);


    Condition getCondition() const;
    void setCondition(const Condition &value);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // PAINT2DWIDGET_H
