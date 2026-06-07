/* gcc C90 pedantic-errors free example */
/* header */
#ifndef HEADER_H
#define HEADER_H
/* -- macros */
#define SON8_CSTRSIZE( str ) ( strlen( str ) + 1 )
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
typedef unsigned Son8Bool;
typedef SON8CHAR Son8Char;
typedef SON8INT0 Son8Int0;
typedef SON8UNT0 Son8Unt0;
typedef SON8INT1 Son8Int1;
typedef SON8UNT1 Son8Unt1;
typedef SON8INT2 Son8Int2;
typedef SON8UNT2 Son8Unt2;
typedef SON8INT3 Son8Int3;
typedef SON8UNT3 Son8Unt3;
typedef size_t Son8Size;
typedef char *Son8CStr;
typedef char const *Son8CCStr;

SON8_EXTERN_CBEG

#define SON8TEXT_SMALL_SIZE 16
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
/* TODO: rewrite using X11 directly */
#include <GLFW/glfw3.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SON8_EXTERN_CBEG

enum Error {
    Error_None,
    Error_Argc,
    Error_Init,
    Error_Window,
    Error_Last_
};

static Son8CCStr Text_Error[Error_Last_] = {
    "none",
    "arguments count",
    "glwf init",
    "window create",
};

int main( int argc, char *argv[] ) {
    /* declarations, not mix with code */
    GLFWwindow *window;
    Son8TextVal name;
    /* code */
    Son8Size error = Error_None;
    /* checking arguments */
    if ( argc > 1 && ( error = Error_Argc ) ) goto error_argc_;
    /* initializing GLFW */
    if ( !glfwInit(  ) && ( error = Error_Init ) ) goto error_init_;
    puts( "glfwInit success" );
    /* creating window */
    son8text_create( &name, argv[0] );
    puts( name.data );
    window = glfwCreateWindow( 640, 360, name.data, NULL, NULL );
    if ( !window && ( error = Error_Window ) ) goto error_window_;
    puts( "glfwCreateWindow success" );
    glfwMakeContextCurrent( window );

    while ( !glfwWindowShouldClose( window ) ) {
        /* NOTE: first GLFW call polling events before rendering */
        glfwPollEvents( );
        /* TODO: rewrite using software rendering with only gl call would be DrawPixels */
        glClear( GL_COLOR_BUFFER_BIT );
        glClearColor( .1f, .4f, .4f, 0.f );
        /* NOTE: second GLFW call swapping buffers after rendering */
        glfwSwapBuffers( window );
    }
    /* cleaning */
error_window_:
    name = son8text_delete( name );
    assert( son8text_valid( name ) == 0u );
error_init_:
    glfwTerminate( );
error_argc_:
    assert( error < Error_Last_ );
    if ( error != Error_None ) {
        fprintf( stderr, "error: %s\n", Text_Error[error] );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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
    memcpy( outPtr->data, inVal.data, outPtr->size );
    return capacity;
error_:
    return son8text_Error_Size( outPtr );
}

Son8Bool son8text_valid( Son8TextVal val )
{ return val.data != NULL && val.size != 0u; }

SON8_EXTERN_CEND
