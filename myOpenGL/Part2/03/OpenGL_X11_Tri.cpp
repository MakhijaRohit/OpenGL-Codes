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

// global
bool bFullScreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColorMap;
Window gWindow;
int giWindowWidth = 1280;
int giWindowHeight = 720;

// GLX value variable
GLXContext gGLXContext;

// Function Declarations
void CreateWindow(void);
void ToggleFullScreen(void);
void Uninitialize(void);
void Initialize(void);
void display(void);
void Resize(int, int);

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
                    Resize(WinWidth, WinHeight);
                break;

                case 33:
                    bDone = true;
                break;

                default:
                    break;
            }// end of switch(event.type)

        }// end od while(1)
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
        GLX_RGBA, GLX_RED_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_ALPHA_SIZE, 1, None
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

    XStoreName(gpDisplay, gWindow, "ROHIT MAKHIJA OpenGL XWindow Triangle");
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

    XSendEvent(gpDisplay, RootWindow(gpDisplay, gpXVisualInfo->screen), False, StructureNotifyMask, &XEvent);
}

void Initialize(void)
{
    gGLXContext = glXCreateContext(gpDisplay, gpXVisualInfo, NULL, GL_TRUE);
    glXMakeCurrent(gpDisplay, gWindow, gGLXContext);
    glClearColor(0.0f, 1.0, 0.0f, 0.0f);
    Resize(giWindowWidth,giWindowHeight);
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

    glFlush();
}

void Resize(int width, int height)
{
    if(height == 0)
    {
        height = 1;
    }
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
