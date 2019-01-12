#include </usr/local/include/GL/freeglut.h>

// global declarations
bool bFullScreen = false;
GLfloat angleTriangle = 0.0f;   // rotation of triangle
GLfloat angleSquare = 0.0f;     // rotation of square

// function prototype
void initialize(void);
void display(void);
void resize(int, int);
void keyboard(unsigned char, int , int);
void spin(void);

int main(int argc, char** argv)
{
    // initialize glut - this line start windowing session
    glutInit(&argc, argv);

    // glutInitDisplayMode(buffer | Color space);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(0,0);

    // Open the window with Text on the title.
    glutCreateWindow("ROHIT MAKHIJA Pyramid Cube 3D Spin");

    // initiate OpenGL
    initialize();

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(spin);

    glutMainLoop();
}

void initialize(void)
{
    glShadeModel(GL_SMOOTH);
    // background color
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // set Depth Buffer
    glClearDepth(1.0f);

    // enable depth test
    glEnable(GL_DEPTH_TEST);

    // depth test
    glDepthFunc(GL_LEQUAL);

    // set nice perspective
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Pyramid */
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);
    glRotatef(angleTriangle, 0.0f, 1.0f, 0.0f);

    // begin
    glBegin(GL_TRIANGLES);

        // Front
        glColor3f(1.0f, 0.0f, 0.0f);       // red
        glVertex3f(0.0f, 1.0f, 0.0f);      // apex
        glColor3f(0.0f, 1.0f, 0.0f);       // green
        glVertex3f(-1.0f, -1.0f, 1.0f);     // left bottom
        glColor3f(0.0f, 0.0f, 1.0f);       // blue
        glVertex3f(1.0f, -1.0f, 1.0f);     // right bottom

        // Right
        glColor3f(1.0f, 0.0f, 0.0f);       // red
        glVertex3f(0.0f, 1.0f, 0.0f);      // apex
        glColor3f(0.0f, 0.0f, 1.0f);       // blue
        glVertex3f(1.0f, -1.0f, 1.0f);     // left bottom
        glColor3f(0.0f, 1.0f, 0.0f);       // green
        glVertex3f(1.0f, -1.0f, -1.0f);    // right bottom

        // Back
        glColor3f(1.0f, 0.0f, 0.0f);       // red
        glVertex3f(0.0f, 1.0f, 0.0f);      // apex
        glColor3f(0.0f, 1.0f, 0.0f);       // green
        glVertex3f(1.0f, -1.0f, -1.0f);    // left bottom
        glColor3f(0.0f, 0.0f, 1.0f);       // blue
        glVertex3f(-1.0f, -1.0f, -1.0f);   // right bottom

        // Left
        glColor3f(1.0f, 0.0f, 0.0f);       // red
        glVertex3f(0.0f, 1.0f, 0.0f);      // apex
        glColor3f(0.0f, 0.0f, 1.0f);       // blue
        glVertex3f(-1.0f, -1.0f, -1.0f);   // left bottom
        glColor3f(0.0f, 1.0f, 0.0f);       // green
        glVertex3f(-1.0f, -1.0f, 1.0f);    // right bottom

    // complete drawing triangle
    glEnd();

    /* Cube */
    glLoadIdentity();
    glTranslatef(1.5f, 0.0f, -6.0f);
    glScalef(0.75f, 0.75f, 0.75f);
    glRotatef(angleSquare, 1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);

        // Top
        glColor3f(1.0f, 0.0f, 0.0f);        // red
        glVertex3f(1.0f, 1.0f, -1.0f);      // right-top
        glVertex3f(-1.0f, 1.0f, -1.0f);     // left-top
        glVertex3f(-1.0f, 1.0f, 1.0f);      // left-bottom
        glVertex3f(1.0f, 1.0f, 1.0f);       // right-bottom

        // Bottom
        glColor3f(0.0f, 1.0f, 0.0f);        // green
        glVertex3f(1.0f, -1.0f, -1.0f);     // right-top
        glVertex3f(-1.0f, -1.0f, -1.0f);    // left-top
        glVertex3f(-1.0f, -1.0f, 1.0f);     // left-bottom
        glVertex3f(1.0f, -1.0f, 1.0f);      // right-bottom

        // Front
        glColor3f(0.0f, 0.0f, 1.0f);        // blue
        glVertex3f(1.0f, 1.0f, 1.0f);       // right-top
        glVertex3f(-1.0f, 1.0f, 1.0f);      // left-top
        glVertex3f(-1.0f, -1.0f, 1.0f);     // left-bottom
        glVertex3f(1.0f, -1.0f, 1.0f);      // right-bottom

        // Back
        glColor3f(1.0f, 1.0f, 0.0f);        // yellow
        glVertex3f(1.0f, 1.0f, -1.0f);      // right-top
        glVertex3f(-1.0f, 1.0f, -1.0f);     // left-top
        glVertex3f(-1.0f, -1.0f, -1.0f);    // left-bottom
        glVertex3f(1.0f, -1.0f, 1.0f);      // right-bottom

        // Left
        glColor3f(0.0f, 1.0f, 1.0f);        // cyan
        glVertex3f(-1.0f, 1.0f, 1.0f);      // right-top
        glVertex3f(-1.0f, 1.0f, -1.0f);     // left-top
        glVertex3f(-1.0f, -1.0f, -1.0f);    // left-bottom
        glVertex3f(-1.0f, -1.0f, 1.0f);     // right-bottom

        // Right
        glColor3f(1.0f, 0.0f, 1.0f);        // magenta
        glVertex3f(1.0f, 1.0f, -1.0f);      // right-top
        glVertex3f(1.0f, 1.0f, 1.0f);       // left-top
        glVertex3f(1.0f, -1.0f, 1.0f);      // left-bottom
        glVertex3f(1.0f, -1.0f, -1.0f);     // right-bottom

    // complete drawing
    glEnd();

    // to process buffer
    glutSwapBuffers();
}

void resize(int width, int height)
{
    if(height == 0)
    {
        height = 1;
    }

    // glViewport(x, y, width, height);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // sets projectoin matrix as current matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    // gluPerspective(Field of View angle Y, w/h, near, far);
    gluPerspective(45.0f, (GLfloat) width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void spin(void)
{
    angleTriangle = angleTriangle + 2.0f;
    if(angleTriangle >= 360.0f)
    {
        angleTriangle = angleTriangle - 360.0f;
    }

    angleSquare = angleSquare + 2.0f;
    if(angleSquare >= 360.0f)
    {
        angleSquare = angleSquare - 360.0f;
    }

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // ASCII value for Es key
        case 27:
        {
            glutLeaveMainLoop();
        }
        break;

        case 'f':
        case 'F':
        {
            if(bFullScreen == false)
            {
                glutFullScreen();
                bFullScreen = true;
            }
            else
            {
                glutLeaveFullScreen();
                bFullScreen = false;
            }
        }
        break;

    default:
        break;
    }
}
