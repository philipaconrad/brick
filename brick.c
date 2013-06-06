//-----------------------------------------------------------------------------
// brick.c -- Simple fixed-memory allocator that works by key-value 
//            accesses of a blocklist.
// Copyright (C) Philip Conrad 5/13/2013 @ 12:14 PM -- MIT License
//
//-----------------------------------------------------------------------------

#include "types.h"
#include "brick.h"


//---------------------------------------------------------
//UTILITY FUNCTIONS:

//Simple round up to nearest multiple function. Based off of Swedish currency rounding.
//swedeRoundUp :: uint32 -> uint32 -> uint32
uint32 swedeRoundUp(uint32 x, uint32 multiple) {
    uint32 i = 0;

    while(x > i) {
        i += multiple;
    }

    return i;
}


//---------------------------------------------------------
// FUNCTION IMPLEMENTATIONS:

//Returns an unsigned integer key into the list of allocation ptrs.
// usage: char* blockptrs[numberofblocksinlist]; uint32 key = blockalloc(size);
//on dealloc: charptr is zeroed out.
//on alloc: since numblocks == numblockptrs, we set all "dirty" blockptrs to point to the start of the region.
//search for open blocks by doing a linear search of the blockptrs array.


//Zeroes out initial memory of the pointer array, and sets the context's reference to the slab of memory.
//brickInit :: brickContext* -> [char*] -> char* -> uint32 -> uint32 -> Effect
void brickInit(brickContext* ctx, char** blockPtrList, char* memory, uint32 numBlocks, uint32 blockSize) {
    uint32 i = 0;
    ctx->blockptrlist = blockPtrList;
    ctx->memory = memory;
    ctx->numBlocks = numBlocks;
    ctx->blockSize = blockSize;

    for(; i < numBlocks; i++) {
        ctx->blockptrlist[i] = 0;
    }
}

//TODO: div by block size!!!!!

//Returns the starting index/key of the first fit for an allocation of length `length`.
//brickFindOpenRun :: brickContext* -> uint32 -> uint32
uint32 brickFindOpenRun(brickContext* ctx, uint32 length) {
    uint32 i = 0;
    uint32 currentRun = 0;

    for(; i < ctx->numBlocks; i++) {
        if(ctx->blockptrlist[i] == 0) {
            currentRun++;
            if(currentRun == length) {
                i -= (currentRun-1);
                break;
            }
            continue;
        }
        //Implicit else:
        currentRun = 0;
    }

    return i;
}


//Returns a key for later access into the index.
//blockMalloc :: brickContext -> uint32 -> Effect -> uint32
uint32 brickMalloc(brickContext* ctx, uint32 size) {
    uint32 key;
    uint32 i = 0;
    uint32 blocksNeeded = swedeRoundUp(size, ctx->blockSize);
    char* memRef = ctx->memory;
    
    blocksNeeded = blocksNeeded / ctx->blockSize;

    key = brickFindOpenRun(ctx, blocksNeeded);

    //0th index allocation case:
    if(!key) {
        //Check to see if index[0] is dirty:
        if(ctx->blockptrlist[0]) {
            //Error, this index is not clean!
            //printf("allocation error!");
            goto endpoint;
        }
        else {
            goto writeptrs;
        }
    }

writeptrs:
    for(i = key; i < key+blocksNeeded; i++) {
        ctx->blockptrlist[i] = &ctx->memory[key*ctx->blockSize]; //FINISH!!
    }

endpoint:
    return key;
}


//"Frees" memory by zeroing out the pointers in the pointer array.
//NOTE: if FREE_DEST_BLOCKS is set, then the blocks of memory will also be zeroed out.
//blockFree :: brickContext* -> uint32 -> Effect
void brickFree(brickContext* ctx, uint32 key) {
    uint32 i = key;
    char* keyval = ctx->blockptrlist[key];

    for(; i < ctx->numBlocks; i++) {
        if(ctx->blockptrlist[i] == keyval) {
            ctx->blockptrlist[i] = 0;
            continue;
        }
        break;
    }
}


//A full, stop-the-world compaction of the blocklist.
//CONCURRENCY NOTE: Needs to be wrapped in a mutex or critical section for safe use.
//brickGC :: brickContext* -> Effect
void brickGC(brickContext* ctx) {
}


//---------------------------------------------------------