#include </usr/local/include/GL/freeglut.h>

// global declarations
bool bFullScreen = false;

// function prototype
void initialize(void);
void display(void);
void resize(int, int);
void keyboard(unsigned char, int , int);

int main(int argc, char** argv)
{
    // initialize glut - this line start windowing session
    glutInit(&argc, argv);

    // glutInitDisplayMode(buffer | Color space);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(0,0);

    // Open the window with Text on the title.
    glutCreateWindow("ROHIT MAKHIJA Triangle");

    // initiate OpenGL
    initialize();

    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}

void initialize(void)
{
    // background color
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    /* Triangle */
    glBegin(GL_TRIANGLES);

    // white color of triangle
    glColor3f(1.0f, 1.0f, 1.0f);

        // co-ordinates of triangle
        // we can also use glVertex2f : 2D Drawing
        // apex
        glVertex3f(0.0f, 50.0f, 0.0f);
        // left bottom vertex
        glVertex3f(-50.0f, -50.0f, 0.0f);
        // right bottom vertex
        glVertex3f(50.0f, -50.0f, 0.0f);

    // complete drawing
    glEnd();

    // to process buffer
    glFlush();
}

void resize(int width, int height)
{
    // code

    if(height == 0)
    {
        height = 1;
    }

    // glViewport(x, y, width, height);
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    // sets projectoin matrix as current matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if(width <= height)
    {
        glOrtho(-100.0f, 100.0f, (-100.0f * (height/width)), (-100.0f * (height/width)), -100.0f, 100.0f);
    }
    else
    {
        glOrtho((-100.0f * (width/height)), (100.0f * (width/height)), -100.0f, 100.0f, -100.0f, 100.0f);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
