#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
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
int main() {
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
    GC gc = XCreateGC(display, window, 0, NULL);
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose){
            XWindowAttributes gwa;
            XGetWindowAttributes(display, window, &gwa);
            draw_gradient(display, window, gc, gwa.width, gwa.height);
        }
        if (event.type == KeyPress){
            if (XLookupKeysym(&event.xkey, 0) == XK_Escape){
                break;
            }
        }
    }
    XCloseDisplay(display);
    return 0;
}
