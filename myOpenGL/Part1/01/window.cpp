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
    glutCreateWindow("ROHIT MAKHIJA");

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
    // clear all pixels - actual painting.
    glClear(GL_COLOR_BUFFER_BIT);

    // should be used in single buffer mode.
    glFlush();
}

void resize(int width, int height)
{
    // code
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
