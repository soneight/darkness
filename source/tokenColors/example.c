/* header */
#ifndef HEADER_H
#define HEADER_H

#define SON8_STRSIZE( str ) ( strlen( str ) + 1 )

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct son8_c_string {
    char *data;
    size_t size;
};

struct son8_c_string
son8_c_string_new( char const *data );
void
son8_c_string_del( struct son8_c_string *self );

#ifdef __cplusplus
}
#endif

#endif/*HEADER_H*/
/* source */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char *argv[] )
{
    struct son8_c_string cstr = son8_c_string_new( argv[0] );
    printf( "tokenColors %s\n", cstr.data );

    son8_c_string_del( &cstr );
    assert( cstr.data == NULL );
    assert( cstr.size == 0 );

    return EXIT_SUCCESS;
}

struct son8_c_string son8_c_string_new( char const *str )
{
    struct son8_c_string cstr;

    cstr.size = SON8_STRSIZE( str );
    cstr.data = (char *)malloc( cstr.size );

    return cstr;
}

void son8_c_string_del( struct son8_c_string *cstr)
{
    free( cstr->data );

    cstr->data = NULL;
    cstr->size = 0;
}
