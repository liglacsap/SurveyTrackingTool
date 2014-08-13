#ifndef PAINT2DWIDGET_H
#define PAINT2DWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "globals.h"
#include "takedialog.h"

extern double fingerRadius;

class Paint2DWidget : public QWidget
{
    Q_OBJECT

private:
    Take take;
public:
    explicit Paint2DWidget(QWidget *parent = 0);


    Take getTake() const;
    void setTake(const Take &value);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // PAINT2DWIDGET_H
