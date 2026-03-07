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
//# Main block
//#
int main() {
    //# Opening display (X11)
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Uh oh, it looks like Tony forgot to debug something...\n");
        exit(1);
    }
    //# Creating window (X11 simple window)
    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(
        display, RootWindow(display, screen), 
        10, 10, 800, 600, 1,
        BlackPixel(display, screen), WhitePixel(display, screen)
    );
    XSelectInput(display, window, ExposureMask | KeyPressMask);
    //# Main window loop
    XMapWindow(display, window);
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == KeyPress){
            // Checking if Esc was pressed
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
