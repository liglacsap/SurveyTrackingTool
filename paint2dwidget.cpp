#include "paint2dwidget.h"

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


    painter.setPen(Qt::darkGray);
    painter.setBrush(QBrush(QColor("#ecf0f1")));
    painter.drawEllipse(QPoint(this->width() / 2, this->height() / 2), this->width() / 3, this->height() / 3);

    QPen pen;

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setBrush(Qt::darkGreen);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor("#95a5a6")));

    painter.drawEllipse(QPoint(this->width() / 2, this->height() / 2), this->width() / 4, this->height() / 4);

    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setBrush(QColor("#c0392b"));
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.setBrush(QColor("#bdc3c7"));
    painter.drawEllipse(QPoint(this->width() / 2, this->height() / 2), this->width() / 5, this->height() / 5);
}
