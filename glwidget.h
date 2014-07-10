#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QDebug>

#include <gl/GLU.h>

#ifndef M_PI
#define M_PI    3.14159265358979323846f
#endif

#include "globals.h"


extern QVector3D handPalmPosition;
extern std::vector<QVector3D> handFingers;
extern double fingerRadius;
extern unsigned int markerCount;



class GLWidget : public QGLWidget
{
    Q_OBJECT
private:
    std::vector<Ball> balls;
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int w, int h);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void drawBox(QVector3D pos, float size);
    void drawSphere(QVector3D pos, double r, int lats, int longs);
    std::vector<Ball> getBalls() const;
    void setBalls(const std::vector<Ball> &value);

protected:
    void paintGL();
signals:

public slots:

};

#endif // GLWIDGET_H
