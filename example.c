//-----------------------------------------------------------------------------
// example.c -- A simple example of the brick block-allocator in action.
// Copyright (c) Philip Conrad 5/13/2013 @ 12:14 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "brick.h"


//---------------------------------------------------------
// EXAMPLE FUNCTIONS:


//---------------------------------------------------------
// UTILITY FUNCTIONS:


//
int main(int argc, char** argv) {
    //local variables:
    brickContext bc;
    char* refs[128];
    uint32 id1;
    uint32 id2;

    //allocate our intial block of memory:
    void* memref = malloc(128*64);

    //initialize brick context:
    brickInit(&bc, refs, (char*)memref, 128, 64);
    
    //allocate some blocks and stuff strings into them:
    id1 = brickMalloc(&bc, 5);
    strncpy(refs[id1], "hola!", 5);
    
    id2 = brickMalloc(&bc, 23);
    strncpy(refs[id2], "This is a nice program.", 23);

    //"free" the allocations made with brick:
    //note: these "frees" can be done out of order without breaking anything.
    brickFree(&bc, id2);
    brickFree(&bc, id1);

    //free our original block of memory:
    free(memref);

    return 0;
}


















