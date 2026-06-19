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

#define APP_XSIZE 640u
#define APP_YSIZE 360u
#define APP_ERROR_HANDLER_TEXTSIZE 8u
#define APP_ERROR_HANDLER_TEXTLAST ( APP_ERROR_HANDLER_TEXTSIZE - 1u )

SON8_EXTERN_CBEG

typedef enum AppError {
    App_Error_None,
    App_Error_Argc,
    App_Error_Log,
    App_Error_Init,
    App_Error_Window,
    App_Error_Unknown_X11,
    App_Error_Image_X11,
    App_Error_Graphic_X11,
    App_Error_Last_
} AppError;

static Son8CStr App_Text_Error[App_Error_Last_] = {
    "none",
    "arguments count failed",
    "fopen failed",
    "XOpenDisplay failed",
    "XCreateSimpleWindow failed",
    "unknown x11 initialization error after window creation",
    "XCreateImage failed",
    "XCreateGC failed"
};

typedef struct AppFrameBuffer {
    Son8Unt2 data[APP_XSIZE][APP_YSIZE];
} AppFrameBuffer;

typedef struct AppX11 {
    Display *display;
    int screen;
    Window root;
    Window window;
    XEvent event;
    XSizeHints *sizeHints;
    Atom wmCloseAtom;
    XImage *image;
    Visual *imageVisual;
    int imageDepth;
    GC graphicContext;
} AppX11;

typedef struct App {
    AppFrameBuffer frameBuffers[2];
    AppFrameBuffer *waitBuffer;
    AppFrameBuffer *drawBuffer;
    Son8Unt3 currFrame;
    AppX11 x11;
} App;
/* globals */
App App_Global;
AppError App_Global_Error;
Son8Bool App_Is_Closing;
/* declarations */
int  app_error_handler_x11( Display *dpy, XErrorEvent *err );

void     x11_free( AppX11 const *outApp );
AppError x11_init( AppX11 *outApp );
void     x11_poll( AppX11 *outX11 );
void     app_sync( App *outApp );
void     app_draw( App *outApp );
void     app_show( App *outApp );

SON8_EXTERN_CEND

int main( int argc, char *argv[] ) {
    /* decl */
    time_t timeBegSec, timeEndSec, timeDiffSec;
    FILE *logFile;
    Son8Unt3 prevFrame = 0u;
    App *outApp = &App_Global;
    App const *app = &App_Global;
    AppX11 *outX11 = &outApp->x11;
    AppX11 const *x11 = &app->x11;
    AppError error = App_Error_None;
    outApp->drawBuffer = &outApp->frameBuffers[0];
    outApp->waitBuffer = &outApp->frameBuffers[1];
    /* code */
    puts( argv[0] );
    /* checking arguments */
    if ( argc > 1 && ( argv[1][0] != '-' || argv[1][1] != 'l' || argv[1][2] != '\0' ) && ( error = App_Error_Argc ) ) goto error_argc_;
    if ( argc == 2 ) {
        /* open log file */
        logFile = fopen( "temp.clog.txt", "w" );
        if ( !logFile && (error = App_Error_Log ) ) goto error_log_;
        puts( "logging into <temp.clog.txt> file success" );
    } else logFile = stdout;
    /* init application */
    XSetErrorHandler( app_error_handler_x11 );
    if ( ( error = x11_init( outX11 ) ) ) goto error_init_;
    puts( "x11_init success" );

    outX11->imageVisual = DefaultVisual( x11->display, x11->screen );
    outX11->imageDepth = DefaultDepth( x11->display, x11->screen );
    outX11->image = XCreateImage( x11->display, x11->imageVisual, x11->imageDepth, ZPixmap, 0, (char *)outApp->waitBuffer->data, APP_XSIZE, APP_YSIZE, 32, 0 );
    if ( outX11->image == NULL && ( error = App_Error_Image_X11 ) ) goto error_image_;
    outX11->graphicContext = XCreateGC( x11->display, x11->window, 0, NULL );
    if ( App_Global_Error && ( error = App_Error_Graphic_X11 ) ) goto error_graphic_;

    time( &timeBegSec );

    while ( !App_Is_Closing ) {
        /* NOTE: poll events before rendering */
        x11_poll( outX11 );
        /* NOTE: synchronize before rendering */
        /* -- TODO: find a way make it async */
        app_sync( outApp );
        /* NOTE: in between process rendering */
        app_draw( outApp );
        /* NOTE: present-swap after rendering */
        app_show( outApp );
        /* timer */
        time( &timeEndSec );
        timeDiffSec = (intmax_t)timeEndSec - (intmax_t)timeBegSec;
        if ( timeDiffSec ) {
            fprintf( logFile, "fps (TODO: more precision): %ld\n", ( app->currFrame - prevFrame ) / (intmax_t)timeDiffSec );
            fflush( logFile );
            prevFrame = app->currFrame;
            timeBegSec = timeEndSec;
        }
        ++outApp->currFrame;
    }
    /* cleaning */
    XFreeGC( x11->display, x11->graphicContext );
error_graphic_:
    outX11->image->data = NULL;
    XDestroyImage( x11->image );
error_image_:
    x11_free( x11 );
error_init_:
    fclose( logFile );
error_log_:
error_argc_:
    assert( error < App_Error_Last_ );

    if ( error != App_Error_None ) {
        fprintf( stderr, "error: %s\n", App_Text_Error[error] );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

SON8_EXTERN_CBEG

/* definitions */
void x11_free( AppX11 const *x11 ) {
    XDestroyWindow( x11->display, x11->window );
    XCloseDisplay( x11->display );
}

AppError x11_init( AppX11 *outX11 ) {
    AppError error = App_Error_None;
    AppX11 const *x11 = outX11;
    outX11->display = XOpenDisplay( NULL );
    if ( x11->display == NULL && ( error = App_Error_Init ) ) goto error_init_;
    puts( "XOpenDisplay success" );
    /* creating window */
    outX11->screen = DefaultScreen( x11->display );
    outX11->root = RootWindow( x11->display, x11->screen );
    outX11->window = XCreateSimpleWindow( x11->display, x11->root, 0, 0, APP_XSIZE, APP_YSIZE, 0, 0, 0);
    XStoreName( x11->display, x11->window, "X11 Window" );
    XSync( x11->display, False );
    if ( App_Global_Error && ( error = App_Error_Window ) ) goto error_window_;
    puts( "XCreateSimpleWindow success" );

    outX11->sizeHints = XAllocSizeHints( );
    if ( x11->sizeHints ) {
        outX11->sizeHints->flags = PMinSize | PMaxSize;
        outX11->sizeHints->min_width = outX11->sizeHints->max_width = APP_XSIZE;
        outX11->sizeHints->min_height = outX11->sizeHints->max_height = APP_YSIZE;
        XSetWMNormalHints(x11->display, x11->window, x11->sizeHints );
        XFree( x11->sizeHints );
    }
    outX11->wmCloseAtom = XInternAtom( x11->display, "WM_DELETE_WINDOW", False );
    XSetWMProtocols( x11->display, x11->window, &outX11->wmCloseAtom, 1 );
    /* processing events */
    XSelectInput( x11->display, x11->window, ExposureMask | KeyPressMask | ButtonPressMask );

    XMapWindow( x11->display, x11->window );
    printf( "%d", outX11->screen );

    if ( App_Global_Error && ( error = App_Error_Unknown_X11 ) ) goto error_unknown_;

    assert( error == App_Error_None );
    return error;
    /* cleaning */
error_unknown_:
    XDestroyWindow( x11->display, x11->window );
error_window_:
    XCloseDisplay( x11->display );
error_init_:
    return error;
}

void x11_poll( AppX11 *outX11 ) {
    AppX11 const *x11 = outX11;

    while ( XPending( x11->display ) ) {
        XNextEvent( x11->display, &outX11->event );
        switch ( x11->event.type ) {
        case Expose: break;
        case KeyPress: break;
        case ButtonPress: break;
        case ClientMessage: {
            if ( (Son8Unt3)x11->event.xclient.data.l[0] == x11->wmCloseAtom ) App_Is_Closing = 1;
                break;
        }
            default: break;
        }
    }

}

void app_sync( App *outApp ) {
    AppX11 const *x11 = &outApp->x11;
    XPutImage( x11->display, x11->window, x11->graphicContext, x11->image, 0, 0, 0, 0, APP_XSIZE, APP_YSIZE );
}

void app_draw( App *outApp ) {
    Son8Size x, y;
    /* TODO: write NASM function to handle logic of loop and writing */
    for ( x = 0u; x < APP_XSIZE; ++x ) {
        for ( y = 0u; y < APP_YSIZE; ++y ) {
            outApp->drawBuffer->data[x][y] = 0xFF1A6868u;
        }
    }
}

void app_show( App *outApp ) {
    App *app = outApp;
    outApp->waitBuffer = &outApp->frameBuffers[app->currFrame & 1u];
    outApp->drawBuffer = &outApp->frameBuffers[(app->currFrame + 1) & 1u];
    outApp->x11.image->data = (char *)outApp->waitBuffer->data;
}

int app_error_handler_x11( Display *dpy, XErrorEvent *err ) {
    char text[APP_ERROR_HANDLER_TEXTSIZE];
    XGetErrorText( dpy, err->error_code, text, sizeof( text ) );
    text[APP_ERROR_HANDLER_TEXTLAST] = 0;
    fprintf( stderr, "app_error_handler_x11 error text: %s", text );
    App_Global_Error = err->error_code;
    assert( App_Global_Error && "App Error X11 must be not equal zero" );
    return 0;
}

SON8_EXTERN_CEND
