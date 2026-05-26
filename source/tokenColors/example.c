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
#include <stddef.h>
#include <stdint.h>

SON8_EXTERN_CBEG

static char const *NullData = "\0";

struct Son8String {
    char *data;
    size_t size;
};

typedef struct Son8String *Son8StringPtr;

struct Son8String
son8string_new( char const *str );
void
son8string_del( Son8StringPtr self );

SON8_EXTERN_CEND
#endif/*HEADER_H*/
/* source */
#include <GLFW/glfw3.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Error {
    Error_None,
    Error_Argc,
    Error_Init,
    Error_Window,
    Error_Last_
};

static char const *Text_Error[Error_Last_] = {
    "none",
    "arguments count",
    "glwf init",
    "window create",
};

int
main( int argc, char *argv[] ) {
    /* declarations, not mix with code */
    GLFWwindow *window;
    struct Son8String name;
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
    assert( name.data[name.size] == '\0' );
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

error_window_:
    son8string_del( &name );
    assert( name.data == NULL );
    assert( name.size == 0 );
error_init_:
    glfwTerminate( );
error_argc_:
    assert( error < Error_Last_ );
    if ( error != Error_None ) {
        puts( "error: " );
        puts( Text_Error[error] );
        putchar( '\n' );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

struct Son8String
son8string_new( char const *cstr ) {
    struct Son8String result;

    result.size = SON8_CSTRSIZE( cstr );
    result.data = (char *)malloc( result.size );

    if ( result.data == NULL ) {
        result.size = 1u;
        cstr = NullData;
    }

    memcpy( result.data, cstr, result.size );

    return result;
}

void
son8string_del( Son8StringPtr strPtr ) {
    free( strPtr->data );

    strPtr->data = NULL;
    strPtr->size = 0;
}
