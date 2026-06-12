/* gcc C90 pedantic-errors free example */
/* header */
#ifndef HEADER_H
#define HEADER_H
/* -- macros */
#include <string.h>
#define SON8_CSTRSIZE( str ) ( strlen( str ) + 1u )
/* ---- extern c */
#ifdef __cplusplus
#define SON8_EXTERN_CBEG extern "C" {
#define SON8_EXTERN_CEND }
#else
#define SON8_EXTERN_CBEG
#define SON8_EXTERN_CEND
#endif/* extern c */
/* -- includes */
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
/* -- fixed integer types */
/* ---- char */
#if CHAR_BIT != 8u || UCHAR_MAX != 0xFFu
#error "1 byte integer is not supported or exotic architecture"
#else
#define SON8INT0 signed char
#define SON8UNT0 unsigned char
#define SON8CHAR unsigned char
#endif
/* ---- short */
#if USHRT_MAX == 0xFFFFu
#define SON8INT1 signed short
#define SON8UNT1 unsigned short
#elif UINT_MAX == 0xFFFFu
#define SON8INT1 signed int
#define SON8UNT1 unsigned int
#else
#error "2 byte integer is not supported or TODO platform specific integer definition"
#endif
/* ---- int */
#if UINT_MAX == 0xFFFFFFFFu
#define SON8INT2 signed int
#define SON8UNT2 unsigned int
#elif ULONG_MAX == 0xFFFFFFFFul
#define SON8INT2 signed long
#define SON8UNT2 unsigned long
#else
#error "4 byte integer is not supported or TODO platform specific integer definition"
#endif
/* ---- long */
#if ULONG_MAX <= 0xFFFFFFFFul
#error "8 byte integer is not supported or TODO platform specific integer definition"
#else
#define SON8INT3 signed long
#define SON8UNT3 unsigned long
#endif
/* -- aliases */
typedef SON8CHAR Son8Char;
typedef SON8INT0 Son8Int0;
typedef SON8UNT0 Son8Unt0;
typedef SON8INT1 Son8Int1;
typedef SON8UNT1 Son8Unt1;
typedef SON8INT2 Son8Int2;
typedef SON8UNT2 Son8Unt2;
typedef SON8INT3 Son8Int3;
typedef SON8UNT3 Son8Unt3;
typedef void Son8Void;
typedef Son8Unt2 Son8Bool;
typedef size_t Son8Size;
typedef char *Son8CPtr;
typedef char const *Son8CStr;

#endif/*HEADER_H*/
/* source */
#include <X11/Xlib.h>
#include <X11/Xutil.h> /* XSizeHints */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define APP_XSIZE 640
#define APP_YSIZE 360
#define APP_ERROR_HANDLER_TEXTSIZE 8u
#define APP_ERROR_HANDLER_TEXTLAST ( APP_ERROR_HANDLER_TEXTSIZE - 1u )

SON8_EXTERN_CBEG

enum Error {
    Error_None,
    Error_Argc,
    Error_Log,
    Error_Init,
    Error_Window,
    Error_Last_
};

static Son8CStr Text_Error[Error_Last_] = {
    "none",
    "arguments count failed",
    "fopen failed",
    "XOpenDisplay failed",
    "XCreateSimpleWindow failed",
};

struct AppFrameBuffer {
    Son8Unt2 data[APP_XSIZE][APP_YSIZE];
};

struct AppX11 {
    Display *display;
    int screen;
    Window root;
    Window window;
    XEvent event;
    Son8Bool isClosing;
    XSizeHints *sizeHints;
    Atom wmCloseAtom;
    Son8Unt3 currFrame;
    XImage *image;
    Visual *imageVisual;
    int imageDepth;
    struct AppFrameBuffer frameBuffers[2];
    struct AppFrameBuffer *waitBuffer;
    struct AppFrameBuffer *drawBuffer;
    GC graphicContext;
};
/* globals */
int App_Error_X11;
struct AppX11 App_X11;
/* declarations */
int  app_error_handler_x11( Display *dpy, XErrorEvent *err );
void app_draw( struct AppX11 *x11Write );

SON8_EXTERN_CEND

int main( int argc, char *argv[] ) {
    /* declarations, not mix with code */
    time_t timeBegSec, timeEndSec, timeDiffSec;
    FILE *logFile;
    Son8Unt3 prevFrame = 0u;
    struct AppX11 *x11Write = &App_X11;
    struct AppX11 const *x11 = &App_X11;
    Son8Size error = Error_None;
    x11Write->drawBuffer = &x11Write->frameBuffers[0];
    x11Write->waitBuffer = &x11Write->frameBuffers[1];
    /* code */
    puts( argv[0] );
    /* checking arguments */
    if ( argc > 1 && ( argv[1][0] != '-' || argv[1][1] != 'l' || argv[1][2] != '\0' ) && ( error = Error_Argc ) ) goto error_argc_;
    if ( argc == 2 ) {
        /* open log file */
        logFile = fopen( "temp.clog.txt", "w" );
        if ( !logFile && (error = Error_Log ) ) goto error_log_;
        puts( "logging into <temp.clog.txt> file success" );
    } else logFile = stdout;
    /* init display */
    x11Write->display = XOpenDisplay( NULL );
    if ( x11->display == NULL && ( error = Error_Init ) ) goto error_init_;
    puts( "XOpenDisplay success" );
    XSetErrorHandler( app_error_handler_x11 );
    /* creating window */
    x11Write->screen = DefaultScreen( x11->display );
    x11Write->root = RootWindow( x11->display, x11->screen );
    x11Write->window = XCreateSimpleWindow( x11->display, x11->root, 0, 0, APP_XSIZE, APP_YSIZE, 0, 0, 0);
    XStoreName( x11->display, x11->window, "X11 Window" );
    XSync( x11->display, False );
    if ( App_Error_X11 && ( error = Error_Window ) ) goto error_window_;
    puts( "XCreateSimpleWindow success" );
    XSetErrorHandler( NULL );

    x11Write->sizeHints = XAllocSizeHints( );
    if ( x11->sizeHints ) {
        x11Write->sizeHints->flags = PMinSize | PMaxSize;
        x11Write->sizeHints->min_width = x11Write->sizeHints->max_width = 640;
        x11Write->sizeHints->min_height = x11Write->sizeHints->max_height = 360;
        XSetWMNormalHints(x11->display, x11->window, x11->sizeHints );
        XFree( x11->sizeHints );
    }
    x11Write->wmCloseAtom = XInternAtom( x11->display, "WM_DELETE_WINDOW", False );
    XSetWMProtocols( x11->display, x11->window, &x11Write->wmCloseAtom, 1 );
    /* processing events */
    XSelectInput( x11->display, x11->window, ExposureMask | KeyPressMask | ButtonPressMask );

    x11Write->imageVisual = DefaultVisual( x11->display, x11->screen );
    x11Write->imageDepth = DefaultDepth( x11->display, x11->screen );
    x11Write->image = XCreateImage( x11->display, x11->imageVisual, x11->imageDepth, ZPixmap, 0, (char *)x11Write->waitBuffer->data, APP_XSIZE, APP_YSIZE, 32, 0 );
    x11Write->graphicContext = XCreateGC( x11->display, x11->window, 0, NULL );

    XMapWindow( x11->display, x11->window );
    time( &timeBegSec );
    while ( !x11->isClosing ) {
        /* NOTE: first poll events before rendering */
        while ( XPending( x11->display ) ) {
            XNextEvent( x11->display, &x11Write->event );
            switch ( x11->event.type ) {
            case Expose: break;
            case KeyPress: break;
            case ButtonPress: break;
            case ClientMessage: {
                if ( (Son8Unt3)x11->event.xclient.data.l[0] == x11->wmCloseAtom ) x11Write->isClosing = 1;
                    break;
            }
                default: break;
            }
        } /* all pending events processed */
        /* NOTE: mezzo in between process rendering */
        /* -- TODO: find a way make it async */
        XPutImage( x11->display, x11->window, x11->graphicContext, x11->image, 0, 0, 0, 0, APP_XSIZE, APP_YSIZE );
        app_draw( x11Write );
        /* NOTE: final swap buffers after rendering */
        x11Write->waitBuffer = &x11Write->frameBuffers[x11->currFrame & 1u];
        x11Write->drawBuffer = &x11Write->frameBuffers[(x11->currFrame + 1) & 1u];
        x11Write->image->data = (char *)x11Write->waitBuffer->data;
        /* timer */
        time( &timeEndSec );
        timeDiffSec = (intmax_t)timeEndSec - (intmax_t)timeBegSec;
        if ( timeDiffSec ) {
            fprintf( logFile, "fps (TODO: more precision): %ld\n", ( x11->currFrame - prevFrame ) / (intmax_t)timeDiffSec );
            fflush( logFile );
            prevFrame = x11->currFrame;
            timeBegSec = timeEndSec;
        }
        ++x11Write->currFrame;
    }
    /* cleaning */
    XDestroyWindow( x11->display, x11->window );
error_window_:
    XCloseDisplay( x11->display );
error_init_:
    fclose( logFile );
error_log_:
error_argc_:
    assert( error < Error_Last_ );
    if ( error != Error_None ) {
        fprintf( stderr, "error: %s\n", Text_Error[error] );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

SON8_EXTERN_CBEG

/* -- app definitions */
void app_draw( struct AppX11 *x11Write ) {
    Son8Size x, y;
    /* TODO: write NASM function to handle logic of loop and writing */
    for ( x = 0u; x < APP_XSIZE; ++x ) {
        for ( y = 0u; y < APP_YSIZE; ++y ) {
            x11Write->drawBuffer->data[x][y] = 0xFF1A6868u;
        }
    }
}

int app_error_handler_x11( Display *dpy, XErrorEvent *err ) {
    char text[APP_ERROR_HANDLER_TEXTSIZE];
    XGetErrorText( dpy, err->error_code, text, sizeof( text ) );
    text[APP_ERROR_HANDLER_TEXTLAST] = 0;
    fprintf( stderr, "app_error_handler_x11 error text: %s", text );
    App_Error_X11 = err->error_code;
    assert( App_Error_X11 && "App Error X11 must be not equal zero" );
    return 0;
}

SON8_EXTERN_CEND
