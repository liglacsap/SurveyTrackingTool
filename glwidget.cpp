#include "glwidget.h"


std::vector<Ball> GLWidget::getBalls() const
{
    return balls;
}

void GLWidget::setBalls(const std::vector<Ball> &value)
{
    balls = value;
}
GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void GLWidget::initializeGL()
{
    //qglClearColor(qtPurple.dark());

    //glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
     glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

/*
     GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
     GLfloat mat_shininess[] = { 250.0 };
     GLfloat light_position[] = { 0.0, 0.0, -100.0, 0.0 };
     glClearColor (0.0, 0.0, 0.0, 0.0);
     glShadeModel (GL_SMOOTH);

     glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
     glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
     glLightfv(GL_LIGHT0, GL_POSITION, light_position);

     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_DEPTH_TEST);
     */
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport(0,0,width,height);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,500.0f);

    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
    glLoadIdentity();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glLoadIdentity();									// Reset The Current Modelview Matrix
    //gluLookAt(-40, 170, 150, -40, 170, 0, 0, 1, 0);
    //glTranslatef(-1.5f,0.0f,-6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
    //glColor3f(1, 0, 0);




    //

   // glTranslatef(-1.5f,0.0f,-6.0f);
    gluLookAt(-160,80.0f,0.0f, 0, 80, 0 ,0, 1, 0);
    glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
        glVertex3f( 0.0f, 1.0f, 0.0f);              // Top
        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
    glEnd();

    drawBox(QVector3D(0, 0, 0), 6);

    for(int i=0; i<handFingers.size(); i++){
        drawSphere(handFingers.at(i), 2, 20, 20);
    }
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void GLWidget::drawBox(QVector3D pos, float size)
{
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());
    glScalef(size, size, size);

    glBegin(GL_QUADS);									// Draw A Quad
        glColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
        glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Top)
        glVertex3f(-1.0f, 1.0f, 1.0f);					// Bottom Left Of The Quad (Top)
        glVertex3f( 1.0f, 1.0f, 1.0f);					// Bottom Right Of The Quad (Top)

        glVertex3f( 1.0f,-1.0f, 1.0f);					// Top Right Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f, 1.0f);					// Top Left Of The Quad (Bottom)
        glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Bottom)
        glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Bottom)

        glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Front)
        glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Front)
        glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Front)
        glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Front)

        glVertex3f( 1.0f,-1.0f,-1.0f);					// Top Right Of The Quad (Back)
        glVertex3f(-1.0f,-1.0f,-1.0f);					// Top Left Of The Quad (Back)
        glVertex3f(-1.0f, 1.0f,-1.0f);					// Bottom Left Of The Quad (Back)
        glVertex3f( 1.0f, 1.0f,-1.0f);					// Bottom Right Of The Quad (Back)

        glVertex3f(-1.0f, 1.0f, 1.0f);					// Top Right Of The Quad (Left)
        glVertex3f(-1.0f, 1.0f,-1.0f);					// Top Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f,-1.0f);					// Bottom Left Of The Quad (Left)
        glVertex3f(-1.0f,-1.0f, 1.0f);					// Bottom Right Of The Quad (Left)

        glVertex3f( 1.0f, 1.0f,-1.0f);					// Top Right Of The Quad (Right)
        glVertex3f( 1.0f, 1.0f, 1.0f);					// Top Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f, 1.0f);					// Bottom Left Of The Quad (Right)
        glVertex3f( 1.0f,-1.0f,-1.0f);					// Bottom Right Of The Quad (Right)
    glEnd();											// Done Drawing The Quad
    glPopMatrix();
}
void GLWidget::drawSphere(QVector3D pos, double r, int lats, int longs) {
    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());

     int i, j;
     for(i = 0; i <= lats; i++) {
         double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
        double z0  = sin(lat0) * r;
        double zr0 =  cos(lat0) * r;

        double lat1 = M_PI * (-0.5 + (double) i / lats);
        double z1 = sin(lat1) * r;
        double zr1 = cos(lat1) * r;

        glBegin(GL_QUAD_STRIP);
        for(j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double) (j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }

    glPopMatrix();
}
