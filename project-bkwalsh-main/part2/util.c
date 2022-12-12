/* 
 * CS 152, Spring 2022
 *
 * Utility functions
 */
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "util.h"

// Include to quiet the compiler warnings.
extern char *strdup(const char *);

/* ck_malloc: allocate s bytes of space and return a pointer to it.
 * An error message with be printed and the program will exit
 * if malloc fails.
 *
 * num_bytes: the number of bytes to allocate
 * fn_name: the name of the function making the call
 *
 * Returns: pointer to the space allocated
 */ 
void *ck_malloc(unsigned long s, char *fn_name) {
    void *tmp = malloc(s);
    if (tmp == NULL) {
        fprintf(stderr, "%s: ran out of space\n", fn_name);
        exit(1);
    }

    return tmp;
}

/* ck_free: free the specified space.
 *
 * space: a pointer to the space to be freed (must not be NULL)
 */ 

void ck_free(void *space) {
    assert(space != NULL);
    free(space);
}

/* ck_strdup: Duplicate a string. An error message with be printed 
 *   and the program will exit if strdup fails.
 *
 * str: the string to duplicate
 * fn_name: the name of the function making the call
 *
 * Returns: pointer to duplicate
 */ 
char *ck_strdup(char *str, char *fn_name) {
    char *dup = strdup(str);
    if (dup == NULL) {
        fprintf(stderr, "%s: ran out of space\n", fn_name);
        exit(1);
    }
    return dup;
}


/* ck_realloc: reallocate from current size to num_bytes bytes
 * An error message with be printed and the program will exit
 * if realloc fails.
 *
 * ptr: the space to be reallocted.
 * num_bytes: the number of bytes to allocate
 * fn_name: the name of the function making the call
 *
 * Returns: pointer to the space allocated
 */ 
void *ck_realloc(void *ptr, unsigned long num_bytes, char *fn_name) {
    void *tmp = realloc(ptr, num_bytes);
    if (tmp == NULL) {
        fprintf(stderr, "%s: ran out of space\n", fn_name);
        exit(1);
    }

    return tmp;
}

