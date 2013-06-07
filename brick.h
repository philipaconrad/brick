//-----------------------------------------------------------------------------
// brick.h -- The basic definitions for the allocator.
// Copyright (c) Philip Conrad 5/13/2013 @ 12:14 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include "types.h"

#ifndef BRICK_H_
#define BRICK_H_


//---------------------------------------------------------
// MACRO DEFINITIONS:

//Since 0 is a valid index, we pick its opposite: all bits set to 1.
//Were this valued a signed 32-bit integer, it would be -1.
#define BRICK_ALLOC_ERROR 0xFFFFFFFF

//If BRICK_ZERO_WRITE_DEST_BLOCKS is defined, then the underlying memory will be 
//zeroed out on brickFree calls. This is a suggested safety feature.
//#define BRICK_ZERO_WRITE_DEST_BLOCKS 1


//---------------------------------------------------------
// DATA STRUCTURES & TYPEDEFS:

typedef struct brickContext {
    char** blockptrlist;
    char* memory;
    uint32 numBlocks;
    uint32 blockSize;
} brickContext;


//---------------------------------------------------------
// FUNCTIONS:

//Zeroes out initial memory of the pointer array, and sets the context's reference to the slab of memory.
//brickInit :: brickContext* -> [char*] -> char* -> uint32 -> uint32 -> Effect
void brickInit(brickContext* ctx, char** blockPtrList, char* memory, uint32 numBlocks, uint32 blockSize);

//Returns the starting index/key of the first fit for an allocation of length `length`.
//Returns 0 on failure, 1+ on success. (thus, our indexes start at 1, much like in Lua.)
//brickFindOpenRun :: brickContext* -> uint32 -> uint32
uint32 brickFindOpenRun(brickContext* ctx, uint32 length);

//Returns a key for later access into the index.
//Returns BRICK_ALLOC_ERROR on failure.
//blockMalloc :: brickContext -> uint32 -> Effect -> uint32
uint32 brickMalloc(brickContext* ctx, uint32 size);

//"Frees" memory by zeroing out the pointers in the pointer array.
//NOTE: if BRICK_ZERO_WRITE_DEST_BLOCKS is set, then the blocks of memory will also be zeroed out.
//blockFree :: brickContext* -> uint32 -> Effect
void brickFree(brickContext* ctx, uint32 key);

//A full, stop-the-world compaction of the blocklist.
//CONCURRENCY NOTE: Needs to be wrapped in a mutex or critical section for safe use.
//brickGC :: brickContext* -> Effect
void brickGC(brickContext* ctx);


//---------------------------------------------------------
#endif //ifndef BRICK_H_