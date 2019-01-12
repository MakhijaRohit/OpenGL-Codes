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

// global
bool bFullScreen = false;
Display *gpDisplay = NULL;
XVisualInfo *gpXVisualInfo = NULL;
Colormap gColorMap;
Window gWindow;
int giWindowWidth = 1280;
int giWindowHeight = 720;

// Function Declarations
void CreateWindow(void);
void ToggleFullScreen(void);
void Uninitialize(void);

int main()
{
    // variables
    int WinWidth = giWindowWidth;
    int WinHeight = giWindowHeight;

    CreateWindow();

    // Message Loop
    XEvent event;
    KeySym keysym;

    while(1)
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
                Uninitialize();
                exit(0);

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
            break;

            case 33:
                Uninitialize();
                exit(0);
            break;

            default:
                break;
        }// end of switch(event.type)

    }// end od while(1)
    return(0);
}

void CreateWindow(void)
{
    //variable declarations
    XSetWindowAttributes WinAttribs;
    int DefaultScreen;
    int DefaultDepth;
    int StyleMask;

    gpDisplay=XOpenDisplay(NULL);
    if(gpDisplay==NULL)
    {
        printf("ERROR : Unable To Open X Display....");
        Uninitialize();
        exit(1);
    }

    DefaultScreen = XDefaultScreen(gpDisplay);
    DefaultDepth = XDefaultDepth(gpDisplay, DefaultScreen);
    gpXVisualInfo = (XVisualInfo *)malloc(sizeof(XVisualInfo));
    if(gpXVisualInfo==NULL)
    {
        printf("ERROR : Unable To Open X Display....");
        Uninitialize();
        exit(1);
    }

    if(XMatchVisualInfo(gpDisplay, DefaultScreen, DefaultDepth, TrueColor, gpXVisualInfo) == 0)
    {
        printf("ERROR : Unable To Open X Display....");
        Uninitialize();
        exit(1);
    }

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

    XStoreName(gpDisplay, gWindow, "ROHIT MAKHIJA XWindow");
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

void Uninitialize(void)
{
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






























