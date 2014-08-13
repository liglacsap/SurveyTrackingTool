#include "paint2dwidget.h"


Take Paint2DWidget::getTake() const
{
    return take;
}

void Paint2DWidget::setTake(const Take &value)
{
    take = value;
}
Paint2DWidget::Paint2DWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette palette(Paint2DWidget::palette());
    palette.setColor(backgroundRole(), Qt::white);
    setPalette(palette);
}

void Paint2DWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

//	pw = gw / 100 * ps;
  //  ps = pw * 100 / gw;

    float ps = fingerRadius / 3;
    ps = (ps > 2) ? 2 : ps;

    painter.setPen(Qt::darkGray);
    painter.setBrush(QBrush(QColor("#ecf0f1")));
    int size = this->height() / 4 * ps;
    painter.drawEllipse(QPoint(this->width() / 2, this->height() / 2), size, size);


    int hardSize = this->height() / 4;
    QPen pen;
    if(ps < 1){
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        pen.setBrush(Qt::darkGreen);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
        painter.setBrush(QBrush(QColor("#95a5a6")));


        painter.drawEllipse(QPoint(this->width() / 2, this->height() / 2), this->height() / 4, this->height() / 4);

        hardSize = this->height() / 4 * ps;
        hardSize = (hardSize < this->height() / 6) ? this->height() / 6 : hardSize;
    }



    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setBrush(QColor("#c0392b"));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(QColor("#bdc3c7"));
    painter.drawEllipse(QPoint(this->width() / 2, this->height() / 2), hardSize, hardSize);
}
