/* gcc C90 pedantic-errors free example */
/* header */
#ifndef HEADER_H
#define HEADER_H
/* macros */
#define SON8_CSTRSIZE( str ) ( strlen( str ) + 1 )
/* -- extern c */
#ifdef __cplusplus
#define SON8_EXTERN_CBEG extern "C" {
#define SON8_EXTERN_CEND }
#else
#define SON8_EXTERN_CBEG
#define SON8_EXTERN_CEND
#endif/* extern c */
/* includes */
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
/* fixed integer types */
/* char */
#if CHAR_BIT != 8u && UCHAR_MAX != 0xFFu
#error "1 byte integer is not supported or exotic architecture"
#else
#define SON8INT0 signed char
#define SON8UNT0 unsigned char
#define SON8CHAR unsigned char
#endif
/* short */
#if USHRT_MAX == 0xFFFFu
#define SON8INT1 signed short
#define SON8UNT1 unsigned short
#elif UINT_MAX == 0xFFFFu
#define SON8INT1 signed int
#define SON8UNT1 unsigned int
#else
#error "2 byte integer is not supported or TODO platform specific integer definition"
#endif
/* int */
#if UINT_MAX == 0xFFFFFFFFu
#define SON8INT2 signed int
#define SON8UNT2 unsigned int
#else
#error "4 byte integer is not supported or TODO platform specific integer definition"
#endif
/* long */
#if ULONG_MAX <= 0xFFFFFFFFul
#error "8 byte integer is not supported or TODO platform specific integer definition"
#else
#define SON8INT3 signed long
#define SON8UNT3 unsigned long
#endif

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

static Son8CCStr NullData = "\0";
/* NOTE: size includes null terminated character
 * - size 0 : error happened (malloc failed?), points to NullData
 * - size 1 : just empty string, points to NullData
 * - size 2+: heap allocated string
 */
struct Son8String {
    char *data;
    size_t size;
};

typedef struct Son8String *Son8StringPtr;
typedef struct Son8String const *Son8StringRef;
typedef struct Son8String Son8StringVal; /* copy */


#define SON8TEXT_SMALL_SIZE 16
struct Son8Text {
    union {
        struct {
            Son8Int0 buf_[SON8TEXT_SMALL_SIZE];
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

Son8Size    son8text_create( Son8TextPtr outPtr, Son8TextVal inVal );
Son8TextVal son8text_delete( Son8TextVal text );
Son8TextVal son8text_empty( void );
Son8Size    son8text_copy( Son8TextPtr outPtr, Son8TextVal inVal );

Son8StringVal
son8string_empty( );
Son8StringVal
son8string_new( Son8CCStr str );
Son8StringVal
son8string_del( Son8StringVal val );
char
son8string_last( Son8StringVal val );
Son8Bool
son8string_valid( Son8StringVal val );
Son8StringVal
son8string_copy( Son8StringVal val );

SON8_EXTERN_CEND
#endif/*HEADER_H*/
/* source */
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

int
main( int argc, char *argv[] ) {
    /* declarations, not mix with code */
    GLFWwindow *window;
    Son8StringVal name;
    size_t error;
    /* code */
    /* checking arguments */
    error = Error_Argc;
    if ( argc > 1 ) goto error_argc_;
    /* initializing GLFW */
    error = Error_Init;
    if ( !glfwInit(  ) ) goto error_init_;
    /* creating window */
    name = son8string_new( argv[0] );
    assert( son8string_last( name ) == '\0' );
    window = glfwCreateWindow( 640, 360, name.data, NULL, NULL );
    error = Error_Window;
    if ( !window ) goto error_window_;

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

    error = Error_None;
    /* cleaning */
error_window_:
    son8string_del( name );
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

Son8StringVal
son8string_empty( ) {
    Son8StringVal empty;
    empty.data =  (char *)NullData;
    empty.size = 1u;
    return empty;
}

Son8StringVal
son8string_error( ) {
    Son8StringVal error;
    error.data =  (char *)NullData;
    error.size = 0u;
    return error;
}

Son8StringVal
son8string_new( Son8CCStr cstr ) {
    Son8StringVal result;

    if ( cstr == NULL || cstr[0] == '\0' ) return son8string_empty( );

    result.size = SON8_CSTRSIZE( cstr );
    result.data = (char *)malloc( result.size );

    if ( result.data == NULL ) return son8string_error( );

    memcpy( result.data, cstr, result.size );
    return result;
}

Son8StringVal
son8string_del( Son8StringVal val ) {
    if ( val.size >= 2 ) free( val.data );
    return son8string_empty( );
}

Son8StringVal
son8string_copy( Son8StringVal val ) {
    Son8StringVal copy;

    if ( val.size <= 1u ) return val;

    copy.size = val.size;
    copy.data = (char *)malloc( copy.size );

    if ( copy.data == NULL ) return son8string_error( );

    memcpy( copy.data, val.data, val.size );
    return copy;
}

Son8Bool
son8string_valid( Son8StringVal val )
{ return val.data != NULL && val.size != 0u; }

char
son8string_last( Son8StringVal val ) {
    assert( son8string_valid( val ) );

    return val.data[val.size - 1u];
}

SON8_EXTERN_CEND
