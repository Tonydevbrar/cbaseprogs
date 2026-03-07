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
#include <string.h>
//#
//# Background
//#

void draw_gradient(Display *d, Window w, GC gc, int width, int height) {
    int i;
    int steps = height;
    for (i = 0; i < steps; i++) {
        unsigned short dgray = 0x3898 - (i * 0x2EE0 / steps);
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
    XWindowAttributes gwa;
    //# Opening display (X11)
    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Uh oh, it looks like Tony forgot to debug something...\n");
        exit(1);
    }
    XSynchronize(display, True);
    //# Setting up variables
    int screen_num;
    unsigned long black_pixel, white_pixel;
    XFontStruct* font_info;
    int x, y;
    screen_num = DefaultScreen(display);
    black_pixel = BlackPixel(display, screen_num);
    white_pixel = WhitePixel(display, screen_num);
    int screen = DefaultScreen(display);
    //# Base color palette (active, inactive, disabled)
    int holo_dark = 0x202020;
    int holo_blue = 0x33B5E5;
    int active = 0xfafafa;
    int inactive = 0xadadad;
    int disabled = 0x7f7f7f;
    //# formule for creating window half the size of screen (initial size)
    //# feel free to resize with cursor, size and pos boundaries are unset
    Display* disp = XOpenDisplay(NULL);
    Screen* screen_size = DefaultScreenOfDisplay(disp);
    int sheight = screen_size->height;
    int swidth = screen_size->width;
    int window_width = swidth * 0.5, window_height = sheight * 0.5;
    int window_xpos = (swidth - window_width) * 0.5, window_ypos = (sheight - window_height) * 0.5;
    //# Creating window (X11 simple window)
    Window window = XCreateSimpleWindow(
        display, RootWindow(display, screen), 
        10, 10, 1, 1, 1,
        BlackPixel(display, screen), WhitePixel(display, screen)
    );
    Colormap cmap = DefaultColormap(display, screen_num);
    //# Window title (X11 window title), and setup.
    XStoreName(display, window, "Android 4 styled window");
    XSelectInput(display, window, ExposureMask | StructureNotifyMask | KeyPressMask);
    XSetWindowBackgroundPixmap(display, window, StaticGray);
    XMapWindow(display, window);
    Visual *visual ;
    Drawable drawable;
    GC gc = XCreateGC(display,window,0,NULL);
    XGetWindowAttributes(display, window, &gwa);
    //# Setting up font (style and size)
    XFontStruct *font = XLoadQueryFont(display, "-b&h-lucida-medium-r-normal-sans-20-140-100-100-p-114-iso10646-1");
    if (font) {
        XSetFont(display, gc, font->fid);
    }
    //# Main window loop
    XMoveResizeWindow(display, window, window_xpos, window_ypos, window_width, window_height);
    XMapWindow(display, window);
    XEvent event;
    while (1) {
        XNextEvent(display, &event);
        if (event.type == Expose){
            //# Setting up window
            XClearWindow(display, window);
            XGetWindowAttributes(display, window, &gwa);
            //# Calling BAckground
            draw_gradient(display, window, gc, gwa.width, gwa.height);
            //# Setting up variables
            int wwidth = gwa.width ;
            int wheight = gwa.height ;
            int paragraph = wwidth * 0.1;
            int center = wwidth * 0.5 ;
            int end = wwidth * 0.8 ;
            int charsize = 20 ;
            //# Drawing Background (centered box)
            XSetForeground(display, gc, holo_dark);
            XFillRectangle(display, window, gc, wwidth* 0.05,-1, wwidth*0.90,wheight);
            XSetForeground(display, gc, holo_blue);
            XFillRectangle(display, window, gc, wwidth* 0.05,40, wwidth*0.90,3 );
            //# Drawing text
            XDrawString(display, window ,gc ,paragraph * 0.9 ,10 + charsize ,"Android 4 text test (dynamic positioning)", strlen("Android 4 text test (dynamic positioning)"));
            XSetForeground(display ,gc ,active);
            XDrawString(display, window ,gc ,paragraph ,20 + (charsize * 3) ,"Start (paragraph)", 17);
            XDrawString(display, window ,gc ,center ,20 + (charsize * 3) ,"Center", 6);
            XDrawString(display, window ,gc ,end ,20 + (charsize * 3) ,"End", 3);
            XSetForeground(display ,gc ,inactive);
            XDrawString(display, window ,gc ,paragraph ,20 + (charsize * 5) ,"Start (paragraph)", 17);
            XDrawString(display, window ,gc ,center ,20 + (charsize * 5) ,"Center", 6);
            XDrawString(display, window ,gc ,end ,20 + (charsize * 5) ,"End", 3);
            XSetForeground(display ,gc ,disabled);
            XDrawString(display, window ,gc ,paragraph ,20 + (charsize * 7) ,"Start (paragraph)", 17);
            XDrawString(display, window ,gc ,center ,20 + (charsize * 7) ,"Center", 6);
            XDrawString(display, window ,gc ,end ,20 + (charsize * 7) ,"End", 3);
        }
        XFlush(display);
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
    exit(0);
}
