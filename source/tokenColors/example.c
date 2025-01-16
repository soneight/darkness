#ifndef HEADER_GUARD
#define HEADER_GUARD

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct son8_c_string son8_c_string;
struct son8_c_string {
    char *data;
    size_t size;
};

son8_c_string son8_c_string_new(char const *data);
void son8_c_string_del(son8_c_string *self);

#ifdef __cplusplus
}
#endif

#endif//HEADER_GUARD

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    son8_c_string cstr = son8_c_string_new(argv[0]);

    printf("tokenColors %s\n", argv[0]);

    son8_c_string_del(&cstr);
    assert(cstr.data == NULL);
    assert(cstr.size == 0);

    return 0;
}

son8_c_string son8_c_string_new(char const *str)
{
    son8_c_string cstr = { NULL, strlen(str) + 1 };

    cstr.data = (char *)malloc(cstr.size);
    return cstr;
}
void son8_c_string_del(son8_c_string *cstr)
{
    free(cstr->data);
    cstr->data = NULL;
    cstr->size = 0;
}
