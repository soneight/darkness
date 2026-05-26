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

struct son8_c_string {
    char *data;
    size_t size;
};

struct son8_c_string
son8_c_string_new( char const *str );
void
son8_c_string_del( struct son8_c_string *self );

SON8_EXTERN_CEND
#endif/*HEADER_H*/
/* source */
#include <GLFW/glfw3.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main( int argc, char *argv[] ) {
    GLFWwindow *window;
    struct son8_c_string cstr = son8_c_string_new( argv[0] );
    assert( cstr.data[cstr.size] == '\0' );
    printf( "tokenColors %s\n", cstr.data );
    if ( !glfwInit(  ) ) return EXIT_FAILURE;

    window = glfwCreateWindow( 640, 360, cstr.data, NULL, NULL );

    if ( !window ) goto terminate;

    glfwMakeContextCurrent( window );

    while ( !glfwWindowShouldClose( window ) ) {
        glClear( GL_COLOR_BUFFER_BIT );
        glClearColor( .1f, .4f, .4f, 0.f );
        glfwSwapBuffers( window );
        glfwPollEvents( );
    }

    son8_c_string_del( &cstr );
    assert( cstr.data == NULL );
    assert( cstr.size == 0 );

    return EXIT_SUCCESS;

terminate:
    glfwTerminate( );
    return EXIT_FAILURE;
}

struct son8_c_string
son8_c_string_new( char const *str ) {
    struct son8_c_string ret;

    ret.size = SON8_CSTRSIZE( str );
    ret.data = (char *)malloc( ret.size );

    if ( ret.data == NULL ) {
        ret.size = 1u;
        str = NullData;
    }

    memcpy( ret.data, str, ret.size );

    return ret;
}

void
son8_c_string_del( struct son8_c_string *cstr) {
    free( cstr->data );

    cstr->data = NULL;
    cstr->size = 0;
}
