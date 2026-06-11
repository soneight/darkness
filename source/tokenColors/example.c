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
typedef char *Son8CStr;
typedef char const *Son8CCStr;

SON8_EXTERN_CBEG

#define SON8TEXT_SMALL_SIZE 16u
/* NOTE: size includes null terminated character */
struct Son8Text {
    union {
        struct {
            char buf_[SON8TEXT_SMALL_SIZE];
        } small_;
        struct {
            Son8CStr ptr_;
            Son8Size held_;
        } large_;
    } data_;
    Son8CStr data;
    Son8Size size;
};

typedef struct Son8Text *Son8TextPtr;
typedef struct Son8Text Son8TextVal;

Son8Size    son8text_create( Son8TextPtr outPtr, Son8CCStr ccStr );
Son8TextVal son8text_delete( Son8TextVal val );
Son8TextVal son8text_empty( void );
Son8Size    son8text_copy( Son8TextPtr outPtr, Son8TextVal inVal );
Son8Bool    son8text_valid( Son8TextVal val );

SON8_EXTERN_CEND
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

static Son8CCStr Text_Error[Error_Last_] = {
    "none",
    "arguments count failed",
    "fopen failed",
    "XOpenDisplay failed",
    "XCreateSimpleWindow failed",
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
    Son8Unt3 currFrame, prevFrame;
    time_t timeDiff;
    XImage *image;
    Visual *imageVisual;
    int imageDepth;
    Son8Unt2 frameBuffer[ APP_XSIZE * APP_YSIZE * 4u ];
    GC graphicContext;
    /* TODO: double buffering */
};
/* globals */
int App_Error_X11;
struct AppX11 App_X11;

int app_error_handler_x11( Display *dpy, XErrorEvent *err );
void app_draw( struct AppX11 *x11Write );

SON8_EXTERN_CEND

int main( int argc, char *argv[] ) {
    /* declarations, not mix with code */
    Son8TextVal name;
    time_t timeBeg, timeEnd;
    FILE *logFile;
    struct AppX11 *x11Write = &App_X11;
    struct AppX11 const *x11 = &App_X11;
    Son8Size error = Error_None;
    /* code */
    /* checking arguments */
    if ( argc > 1 && ( error = Error_Argc ) ) goto error_argc_;
    son8text_create( &name, argv[0] );
    puts( name.data );
    /* open log file */
    logFile = fopen( "temp.clog.txt", "w" );
    if ( !logFile && (error = Error_Log ) ) goto error_log_;
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
    x11Write->image = XCreateImage( x11->display, x11->imageVisual, x11->imageDepth, ZPixmap, 0, (char *)x11->frameBuffer, APP_XSIZE, APP_YSIZE, 32, 0 );
    x11Write->graphicContext = XCreateGC( x11->display, x11->window, 0, NULL );

    XMapWindow( x11->display, x11->window );
    while ( !x11->isClosing ) {
        timeBeg = time( NULL );
        /* NOTE: first polling events before rendering */
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
        app_draw( x11Write );
        XPutImage( x11->display, x11->window, x11->graphicContext, x11->image, 0, 0, 0, 0, APP_XSIZE, APP_YSIZE );
        timeEnd = time( NULL );
        x11Write->timeDiff = (intmax_t)timeEnd - (intmax_t)timeBeg;
        if ( x11->timeDiff ) {
            fprintf( logFile, "fps (TODO: more precision): %ld\n", ( x11->currFrame - x11->prevFrame ) / x11->timeDiff );
            fflush( logFile );
            x11Write->prevFrame = x11->currFrame;
        }
        ++x11Write->currFrame;
        /* NOTE: second swapping buffers after rendering */
    }
    /* cleaning */
    XDestroyWindow( x11->display, x11->window );
error_window_:
    XCloseDisplay( x11->display );
error_init_:
    fclose( logFile );
error_log_:
    name = son8text_delete( name );
    assert( son8text_valid( name ) == 0u );
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
    for ( y = 0u; y < APP_YSIZE; ++y ) {
        for ( x = 0u; x < APP_XSIZE; ++x ) {
            x11Write->frameBuffer[ y * APP_XSIZE + x ] = 0xFF104040u;
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

/* -- static */
static Son8TextVal Son8Text_Null;
static Son8TextVal son8text_Error( void )
{ return Son8Text_Null; }
static Son8Size son8text_Error_Size( Son8TextPtr outPtr ) {
    *outPtr = Son8Text_Null;
    return outPtr->size;
}
static Son8Size son8text_Empty_Size( Son8TextPtr outPtr ) {
    *outPtr = son8text_empty( );
    return outPtr->size;
}
static Son8Size son8text_Expand_Held( Son8Size size ) {
    Son8Unt0 shift = SON8TEXT_SMALL_SIZE >> 4u;
    return ( size >> shift ) << ( shift + 1u );
}
/* -- definitions */
Son8Size son8text_create( Son8TextPtr outPtr, Son8CCStr ccStr ) {
    Son8Size capacity;

    assert( ccStr != NULL );

    if ( ccStr[0] == '\0' ) return son8text_Empty_Size( outPtr );

    outPtr->size = strlen( ccStr ) + 1;

    if ( outPtr->size > SON8TEXT_SMALL_SIZE ) {
        outPtr->data_.large_.held_ = son8text_Expand_Held( outPtr->size );
        outPtr->data_.large_.ptr_ = (char *)malloc( outPtr->data_.large_.held_ );

        if ( outPtr->data_.large_.ptr_ == NULL ) return son8text_Error_Size( outPtr );

        outPtr->data = outPtr->data_.large_.ptr_;
        capacity = outPtr->data_.large_.held_;
    } else {
        outPtr->data = outPtr->data_.small_.buf_;
        capacity = SON8TEXT_SMALL_SIZE;
    }

    memcpy( outPtr->data, ccStr, outPtr->size );
    return capacity;
}

Son8TextVal son8text_delete( Son8TextVal val ) {

    if ( val.size > SON8TEXT_SMALL_SIZE ) free( val.data );

    return son8text_Error( );
}

Son8TextVal son8text_empty( void ) {
    Son8TextVal empty = Son8Text_Null;

    empty.size = 1u;
    empty.data = Son8Text_Null.data_.small_.buf_;

    return empty;
}
/* NOTE: on copy allocate upto size, not capacity(held_) */
Son8Size son8text_copy( Son8TextPtr outPtr, Son8TextVal inVal ) {
    Son8Size capacity;
    if ( inVal.data == NULL ) goto error_;

    if ( inVal.size > SON8TEXT_SMALL_SIZE ) {
        outPtr->data_.large_.ptr_ = (Son8CStr)malloc( inVal.size );

        if ( outPtr->data_.large_.ptr_ == NULL ) goto error_;

        outPtr->data_.large_.held_ = inVal.size;
        outPtr->data = outPtr->data_.large_.ptr_;
        capacity = outPtr->data_.large_.held_;
    } else {
        outPtr->data = outPtr->data_.small_.buf_;
        capacity = SON8TEXT_SMALL_SIZE;
    }
    outPtr->size = inVal.size;
    memcpy( outPtr->data, inVal.data, inVal.size );
    return capacity;
error_:
    return son8text_Error_Size( outPtr );
}

Son8Bool son8text_valid( Son8TextVal val )
{ return val.data != NULL && val.size != 0u; }

SON8_EXTERN_CEND
