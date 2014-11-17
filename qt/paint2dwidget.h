#ifndef PAINT2DWIDGET_H
#define PAINT2DWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "../structures.h"


class Paint2DWidget : public QWidget
{
    Q_OBJECT

private:
    Condition condition;
    CapturedHand hand;
public:
    explicit Paint2DWidget(QWidget *parent = 0);


    Condition getCondition() const;
    void setCondition(const Condition &value);
    void setCapturedHand(CapturedHand hand);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // PAINT2DWIDGET_H
