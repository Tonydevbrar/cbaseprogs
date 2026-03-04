#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
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
    XMapWindow(display, window);
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == KeyPress){
            if (XLookupKeysym(&event.xkey, 0) == XK_Escape){
                break;
            }
        }
    }
    XCloseDisplay(display);
    return 0;
}
