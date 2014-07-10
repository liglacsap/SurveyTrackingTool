#ifndef PAINT2DWIDGET_H
#define PAINT2DWIDGET_H

#include <QWidget>
#include <QPainter>

class Paint2DWidget : public QWidget
{
    Q_OBJECT
public:
    explicit Paint2DWidget(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // PAINT2DWIDGET_H
