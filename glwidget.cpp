#include "glwidget.h"


GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
}

void GLWidget::initializeGL()
{
    //qglClearColor(qtPurple.dark());

    //glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
    glClearColor(1.0f, 1.0f, 1.0f, 0.5f);				// Black Background
    glClearDepth(1.0f);									// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
     glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations






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

    static int cnt = 0;
    cnt ++;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glLoadIdentity();

    gluLookAt(-0.0f, 0.0f,-30.0f, 0, 0, 0, 0, 1, 0);

    GLfloat mat_specular[] = { 1.0, 0.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {50 };
    glShadeModel (GL_SMOOTH);

    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    GLfloat LightAmbient[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat LightDiffuse[]=		{ 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat LightPosition[]=	{ 0.0f, 0.0f, -20.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    glEnable(GL_DEPTH_TEST);

    //drawBox(QVector3D(0, 0, 0), 6);
    glColor3f(1, 0, 0);
    drawSphere(QVector3D(0, 0, 0), 6, 30, 30);
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

    glBegin(GL_QUADS);
    glNormal3f( 0.0f, 0.0f, 1.0f);                  // Normal Pointing Towards Viewer
     glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 1 (Front)
     glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 2 (Front)
     glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Front)
     glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 4 (Front)
     // Back Face
     glNormal3f( 0.0f, 0.0f,-1.0f);                  // Normal Pointing Away From Viewer
     glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Back)
     glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 2 (Back)
     glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 3 (Back)
     glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 4 (Back)
     // Top Face
     glNormal3f( 0.0f, 1.0f, 0.0f);                  // Normal Pointing Up
     glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 1 (Top)
     glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 2 (Top)
     glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Top)
     glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 4 (Top)
     // Bottom Face
     glNormal3f( 0.0f,-1.0f, 0.0f);                  // Normal Pointing Down
     glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Bottom)
     glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 2 (Bottom)
     glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 3 (Bottom)
     glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 4 (Bottom)
     // Right face
     glNormal3f( 1.0f, 0.0f, 0.0f);                  // Normal Pointing Right
     glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Point 1 (Right)
     glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Point 2 (Right)
     glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Point 3 (Right)
     glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Point 4 (Right)
     // Left Face
     glNormal3f(-1.0f, 0.0f, 0.0f);                  // Normal Pointing Left
     glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Point 1 (Left)
     glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Point 2 (Left)
     glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Point 3 (Left)
     glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Point 4 (Left)

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
