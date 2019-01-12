#include <iostream>

// C
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

// X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

// Bridging API
#include </usr/include/GL/gl.h>
#include </usr/include/GL/glx.h>
#include </usr/include/GL/glu.h>

// global
bool bFullScreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColorMap;
Window gWindow;
int giWindowWidth = 1266;
int giWindowHeight = 720;

GLfloat angleTriangle = 0.0f;   // rotation of triangle
GLfloat angleSquare = 0.0f;     // rotation of square

// Lights
bool bLight = false;
GLfloat LightAmbient[]  = { 0.5f, 0.5f, 0.5f , 1.0f};
GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f , 1.0f};
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f , 1.0f};

// GLX value variable
GLXContext gGLXContext;

// Function Declarations
void CreateWindow(void);
void ToggleFullScreen(void);
void Uninitialize(void);
void Initialize(void);
void display(void);
void resize(int, int);
void spin(void);

int main()
{
    // variables
    int WinWidth = giWindowWidth;
    int WinHeight = giWindowHeight;
    bool bDone = false;

    CreateWindow();
    Initialize();

    // Message Loop
    XEvent event;
    KeySym keysym;

    while(bDone == false)
    {
        while(XPending(gpDisplay)) // Parallel to Peek Message
        {
            XNextEvent(gpDisplay, &event);
            switch(event.type)
            {
                case MapNotify:
                break;

                case KeyPress:
                keysym = XkbKeycodeToKeysym(gpDisplay, event.xkey.keycode, 0, 0);

                switch(keysym)
                {
                    case XK_Escape:
                        bDone = true;
                    break;

                    case XK_F:
                    case XK_f:
                        if(bFullScreen == false)
                        {
                            ToggleFullScreen();
                            bFullScreen = true;
                        }
                        else
                        {
                            ToggleFullScreen();
                            bFullScreen = false;
                        }
                    break;

                case XK_L:
                case XK_l:
                {
                    if(bLight == false)
                    {
                        bLight = true;
                        glEnable(GL_LIGHTING);
                    }
                    else
                    {
                        bLight = false;
                        glDisable(GL_LIGHTING);
                    }
                }
                break;

                    default:
                        break;

                }// end of switch(keysym)
                break;

                case ButtonPress:
                {
                    switch(event.xbutton.button)
                    {
                        // cases
                    }
                }// case ButtonPress
                break;

                case ConfigureNotify:
                    WinWidth = event.xconfigure.width;
                    WinHeight = event.xconfigure.height;
                    resize(WinWidth, WinHeight);
                break;

                case 33:
                    bDone = true;
                break;

                default:
                    break;
            }// end of switch(event.type)

        }// end of while(1)
        spin();
        display();
    }
    Uninitialize();
    return(0);
}

void CreateWindow(void)
{
    //variable declarations
    XSetWindowAttributes WinAttribs;
    int DefaultScreen;
    int DefaultDepth;
    int StyleMask;

    // Parallel to PFD members
    static int frameBufferAttributes[]=
    {
        GLX_RGBA, GLX_DOUBLEBUFFER, true, GLX_DEPTH_SIZE, 24, GLX_RED_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_ALPHA_SIZE, 8, None
    };

    gpDisplay=XOpenDisplay(NULL);
    if(gpDisplay==NULL)
    {
        printf("ERROR : Unable To Open X Display....");
        Uninitialize();
        exit(1);
    }

    DefaultScreen = XDefaultScreen(gpDisplay);

    gpXVisualInfo = glXChooseVisual(gpDisplay, DefaultScreen, frameBufferAttributes);

    WinAttribs.border_pixel = 0;
    WinAttribs.background_pixmap = 0;
    WinAttribs.colormap = XCreateColormap(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen), gpXVisualInfo->visual, AllocNone);
    gColorMap = WinAttribs.colormap;
    WinAttribs.background_pixel = WhitePixel(gpDisplay, DefaultScreen);
    WinAttribs.event_mask = ExposureMask | VisibilityChangeMask | ButtonPressMask | KeyPressMask | PointerMotionMask | StructureNotifyMask;
    StyleMask = CWBorderPixel | CWBackPixel | CWEventMask | CWColormap;

    gWindow = XCreateWindow(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen), 0, 0, giWindowWidth, giWindowHeight, 0, gpXVisualInfo->depth, InputOutput, gpXVisualInfo->visual, StyleMask, &WinAttribs);
    if(!gWindow)
    {
        printf("ERROR : Unable To Create Window....");
        Uninitialize();
        exit(1);
    }

    XStoreName(gpDisplay, gWindow, "ROHIT MAKHIJA OpenGL XWindow Triangle Square Perspective Color Spin");
    Atom WindowManagerDelete = XInternAtom(gpDisplay, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(gpDisplay, gWindow, &WindowManagerDelete, 1);
    XMapWindow(gpDisplay, gWindow);
}


void ToggleFullScreen(void)
{
    //variable declarations
    Atom WM_State;
    Atom FullScreen;
    XEvent XEvent = {0};

    WM_State = XInternAtom(gpDisplay, "_NET_WM_STATE", False);
    memset(&XEvent, 0, sizeof(XEvent));

    XEvent.type = ClientMessage;
    XEvent.xclient.window = gWindow;
    XEvent.xclient.message_type = WM_State;
    XEvent.xclient.format = 32;
    XEvent.xclient.data.l[0] = bFullScreen ? 0 : 1;

    FullScreen = XInternAtom(gpDisplay, "_NET_WM_STATE_FULLSCREEN", False);
    XEvent.xclient.data.l[1] = FullScreen;

    XSendEvent(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen), False, StructureNotifyMask | KeyPressMask , &XEvent);
}

void Initialize(void)
{
    gGLXContext = glXCreateContext(gpDisplay, gpXVisualInfo, NULL, GL_TRUE);
    glXMakeCurrent(gpDisplay, gWindow, gGLXContext);
    // lights
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);

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

    resize(giWindowWidth,giWindowHeight);
}

void display(void)
{
    // shape
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

    /* Pyramid */

        glLoadIdentity();
        glTranslatef(-1.5f, 0.0f, -6.0f);
        glRotatef(angleTriangle, 0.0f, 1.0f, 0.0f);

        // begin
        glBegin(GL_TRIANGLES);

            // Front
            glNormal3f(-1.0, 0.0f, 0.0f);      // normal
            glVertex3f(0.0f, 1.0f, 0.0f);      // apex
            glVertex3f(-1.0f, -1.0f, 1.0f);    // left bottom
            glVertex3f(1.0f, -1.0f, 1.0f);     // right bottom

            // Back
            glNormal3f(0.0, 1.0f, 0.0f);       // normal
            glVertex3f(0.0f, 1.0f, 0.0f);      // apex
            glVertex3f(1.0f, -1.0f, -1.0f);     // left bottom
            glVertex3f(-1.0f, -1.0f, -1.0f);    // right bottom

            // Right
            glNormal3f(0.0, 0.0f, 1.0f);      // normal
            glVertex3f(0.0f, 1.0f, 0.0f);      // apex
            glVertex3f(1.0f, -1.0f, 1.0f);    // left bottom
            glVertex3f(1.0f, -1.0f, -1.0f);   // right bottom

            // Left
            glNormal3f(0.0, 0.0f, -1.0f);      // normal
            glVertex3f(0.0f, 1.0f, 0.0f);      // apex
            glVertex3f(-1.0f, -1.0f, -1.0f);   // left bottom
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
            glNormal3f(0.0, 1.0f, 0.0f);      // normal
            glVertex3f(1.0f, 1.0f, -1.0f);      // right-top
            glVertex3f(-1.0f, 1.0f, -1.0f);     // left-top
            glVertex3f(-1.0f, 1.0f, 1.0f);      // left-bottom
            glVertex3f(1.0f, 1.0f, 1.0f);       // right-bottom

            // Bottom
            glNormal3f(0.0, -1.0f, 0.0f);      // normal
            glVertex3f(1.0f, -1.0f, -1.0f);     // right-top
            glVertex3f(-1.0f, -1.0f, -1.0f);    // left-top
            glVertex3f(-1.0f, -1.0f, 1.0f);     // left-bottom
            glVertex3f(1.0f, -1.0f, 1.0f);      // right-bottom

            // Front
            glNormal3f(0.0, 0.0f, 1.0f);      // normal
            glVertex3f(1.0f, 1.0f, 1.0f);       // right-top
            glVertex3f(-1.0f, 1.0f, 1.0f);      // left-top
            glVertex3f(-1.0f, -1.0f, 1.0f);     // left-bottom
            glVertex3f(1.0f, -1.0f, 1.0f);      // right-bottom

            // Back
            glNormal3f(0.0, 0.0f, -1.0f);      // normal
            glVertex3f(1.0f, 1.0f, -1.0f);      // right-top
            glVertex3f(-1.0f, 1.0f, -1.0f);     // left-top
            glVertex3f(-1.0f, -1.0f, -1.0f);    // left-bottom
            glVertex3f(1.0f, -1.0f, 1.0f);      // right-bottom

            // Left
            glNormal3f(-1.0, 0.0f, 0.0f);      // normal
            glVertex3f(-1.0f, 1.0f, 1.0f);      // right-top
            glVertex3f(-1.0f, 1.0f, -1.0f);     // left-top
            glVertex3f(-1.0f, -1.0f, -1.0f);    // left-bottom
            glVertex3f(-1.0f, -1.0f, 1.0f);     // right-bottom

            // Right
            glNormal3f(1.0, 0.0f, 0.0f);      // normal
            glVertex3f(1.0f, 1.0f, -1.0f);      // right-top
            glVertex3f(1.0f, 1.0f, 1.0f);       // left-top
            glVertex3f(1.0f, -1.0f, 1.0f);      // left-bottom
            glVertex3f(1.0f, -1.0f, -1.0f);     // right-bottom

        // complete drawing
        glEnd();

    glXSwapBuffers(gpDisplay, gWindow);
    //    glFlush();
}

void resize(int width, int height)
{
    if(height == 0)
    {
        height = 1;
    }
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
    angleTriangle = angleTriangle + 1.0f;
    if(angleTriangle >= 360.0f)
    {
        angleTriangle = angleTriangle - 360.0f;
    }

    angleSquare = angleSquare + 1.0f;
    if(angleSquare >= 360.0f)
    {
        angleSquare = angleSquare - 360.0f;
    }

}

void Uninitialize(void)
{
    GLXContext currentGLXContext;
    currentGLXContext = glXGetCurrentContext();

    if(currentGLXContext != NULL && currentGLXContext == gGLXContext)
    {
        glXMakeCurrent(gpDisplay, 0, 0);
    }

    if(gGLXContext)
    {
        glXDestroyContext(gpDisplay, gGLXContext);
    }

    if(gWindow)
    {
        XDestroyWindow(gpDisplay,gWindow);
    }

    if(gColorMap)
    {
        XFreeColormap(gpDisplay,gColorMap);
    }

    if(gpXVisualInfo)
    {
        free(gpXVisualInfo);
        gpXVisualInfo = NULL;
    }

    if(gpDisplay)
    {
        XCloseDisplay(gpDisplay);
        gpDisplay = NULL;
    }
}
