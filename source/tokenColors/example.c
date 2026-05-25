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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main( int argc, char *argv[] ) {
    struct son8_c_string cstr = son8_c_string_new( argv[0] );
    printf( "tokenColors %s\n", cstr.data );

    son8_c_string_del( &cstr );
    assert( cstr.data == NULL );
    assert( cstr.size == 0 );

    return EXIT_SUCCESS;
}

struct son8_c_string
son8_c_string_new( char const *str ) {
    struct son8_c_string cstr;

    cstr.size = SON8_CSTRSIZE( str );
    cstr.data = (char *)malloc( cstr.size );

    return cstr;
}

void
son8_c_string_del( struct son8_c_string *cstr) {
    free( cstr->data );

    cstr->data = NULL;
    cstr->size = 0;
}
