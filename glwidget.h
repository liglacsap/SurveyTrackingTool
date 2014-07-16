#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QDebug>

#include <GL/glu.h>

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
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int w, int h);

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void drawBox(QVector3D pos, float size);
    void drawSphere(QVector3D pos, double r, int lats, int longs);


protected:
    void paintGL();
signals:

public slots:

};

#endif // GLWIDGET_H
