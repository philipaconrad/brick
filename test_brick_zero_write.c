//-----------------------------------------------------------------------------
// test_brick_zero_write.c -- Test for the zero-write option.
// Copyright (C) Philip Conrad 5/13/2013 @ 12:14 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "brick.h"
#include "greatest.h"


//---------------------------------------------------------
// MACRO DEFINITION CHECK

#ifndef BRICK_ZERO_WRITE_DEST_BLOCKS
#error BRICK_ZERO_WRITE_DEST_BLOCKS must be defined for the zero-write test suite.
#endif


//---------------------------------------------------------
// TESTS

TEST test_brick_zero_write() {
    brickContext bc;
    char* zeroString;
    char* refs[24];
    uint32 id1;
    uint32 id2;

    //allocate our intial block of memory:
    void* memref = malloc(24*64);

    //allocate an equally-sized block of zeroed-out memory:
    zeroString = (char*)malloc(24*64);
    memset(zeroString, '\0', 24*64);

    //initialize brick context:
    brickInit(&bc, refs, (char*)memref, 24, 64);

    //allocate memory and load strings into the allocated chunks:
    id1 = brickMalloc(&bc, 91);
    strncpy(refs[id1], "Hola! Welcome to Test-Land, where we run tests, all day long! This is a multi-block string.", 91);
    
    id2 = brickMalloc(&bc, 23);
    strncpy(refs[id2], "This is a nice program.", 23);

    //"free" allocated memory, performing a zero-write behind the scenes:
    brickFree(&bc, id1);
    brickFree(&bc, id2);

    //perform our check:
    ASSERTm("Zero-write failed.", memcmp(zeroString, memref, 24*64) == 0);

    //free our original block of memory:
    free(memref);

    PASS();
}


//---------------------------------------------------------
// SUITE

SUITE(suite) {
    RUN_TEST(test_brick_zero_write);
}


//---------------------------------------------------------
// MAIN

/* Add all the definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line arguments, initialization. */
    RUN_SUITE(suite);
    GREATEST_MAIN_END();        /* display results */
}
