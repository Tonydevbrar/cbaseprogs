//#
//# Ready to compile program
//# IF YOU DON'T KNOW WHAT YOU'RE DOING: DO YES EDIT (you're alowed to edit and see what happens).
//# gcc [filename] -lX11 to compile. (Can't be made static on some OSes)
//# X11 c template for UNIX.
//#
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
//#
//# Background
//#

void draw_gradient(Display *d, Window w, GC gc, int width, int height) {
    int i;
    int steps = height;
    for (i = 0; i < steps; i++) {
        unsigned short dgray = 15000 - (i * 12000 / steps);
        XColor color;
        color.red = dgray;
        color.green = dgray;
        color.blue = dgray;
        XAllocColor(d, DefaultColormap(d, DefaultScreen(d)), &color);
        XSetForeground(d, gc, color.pixel);
        XDrawLine(d, w, gc, 0, i, width, i);
    }
}
//# end of background

//#
//# Main block
//#
int main() {
    //# Opening display (X11)
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Uh oh, it looks like Tony forgot to debug something...\n");
        exit(1);
    }
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(
        display, RootWindow(display, screen), 
        10, 10, 800, 600, 1,
        BlackPixel(display, screen), WhitePixel(display, screen)
    );
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XSetWindowBackgroundPixmap(display, window, None);
    XMapWindow(display, window);
    //# Setting up variables
    GC gc = XCreateGC(display, window, 0, NULL);
    //# Main window loop
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose){
            //# Setting up window
            XWindowAttributes gwa;
            XGetWindowAttributes(display, window, &gwa);
            //# Calling Background
            draw_gradient(display, window, gc, gwa.width, gwa.height);
        }
        // Checking if Esc was pressed
        if (event.type == KeyPress){
            if (XLookupKeysym(&event.xkey, 0) == XK_Escape){
                break;
            }
        }
    }
    //# Closing window
    XClearWindow(display,window);
    XDestroyWindow(display,window);
    XCloseDisplay(display);
    return 0;
}
