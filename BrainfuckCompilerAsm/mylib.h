#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef HEX_WIDTH
#  define HEX_WIDTH 8
#endif

// always reads as binary
void*
readfile(
    const char *fname,
    size_t *psize
    );

void
writefile(
    const char *fname,
    void *data,
    size_t datasize
    );

// binary:
//   0 - read as text
//   1 - read as data
void*
readfile_ex(
    void *dst,
    size_t dstsize,
    const char *fname,
    size_t offset, 
    size_t *psize,
    int binary
    );

// table[256] filled with
// zeroes
// int *(table[256])
// int (*table)[256]
void
histogram(
    void *src,
    size_t count,
    int *table
    );

void
printhex(
    void *src,
    size_t size
    );
