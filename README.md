brick
=======

brick is a fixed-memory block allocator with a simple interface.

It is *strictly* an interface (brick performs no mallocs or frees under the hood).

Its purpose is to make structured access to a chunk of memory easier.


### How it works

A `char*` array is (implicitly) mapped one-to-one with a contiguous chunk of memory, 
each pointer corresponding to a matching "block" in the chunk of memory. These blocks 
are of a uniform size, chosen at compile-time.


    char*    allocated memory
    array
               +-------+
    +---+      | +---+ |
    | 0 |  ->  | | 0 | |
    +---+      | +---+ |
    | 1 |  ->  | | 1 | |
    +---+      | +---+ |
    | 2 |  ->  | | 2 | |
    +---+      | +---+ |
               +-------+


Thanks to the implicit mapping, mallocs occur when one or more of the `char*` pointers is directed to point at 
a block in the preallocated chunk of memory. To "free" a block, its corresponding pointer is simply set to 0.

When multiple blocks are allocated together, the corresponding `char*` pointers are all set to point to the same 
address: the start of the allocation. The result is that the programmer can discover the precise size of an 
allocation by observing the number of equivalent pointers in a row.

See the [example][1] program for how this all works in practice.

See [Idioms](#idioms) for handy usage tips and examples.


### API
 - `void   brickInit(brickContext* ctx, char** blockPtrList, char* memory, uint32 numBlocks, uint32 blockSize);`
 - `uint32 brickFindOpenRun(brickContext* ctx, uint32 length);`
 - `uint32 brickMalloc(brickContext* ctx, uint32 size);`
 - `void   brickFree(brickContext* ctx, uint32 key);`
 - `void   brickGC(brickContext* ctx);` **Warning:** Not implemented yet.


### Idioms
 - **Malloc Error Check:**
   To see if `brickMalloc()` failed, check to see if its return value is equivalent to *BRICK_MALLOC_ERROR* 
   (which is currently a convenient alias for the constant *0xFFFFFFFF*).

   *Example:*

    ```
    //setup:
    brickContext* ctx;
    uint32 key;
    
    //attempt an allocation, and get a key to it:
    key = brickMalloc(&bc, 9001);
    
    //check for an allocation failure:
    if(key == BRICK_MALLOC_ERROR) {
        /* ... failure-handling code ... */
    }
    ```

 - **Accessing an allocated block:**
   To access memory blocks allocated with brick, just access the `char*` array with the unsigned integer key 
   provided by `brickMalloc()`.

   *Example:*
   
    ```
    //setup:
    brickContext* ctx;
    char* blocks[128]; //pointed at by ctx->blockptrlist.
    uint32 key;
    uint32 strLength;
    
    //allocate however many blocks necessary to contain at least 9001 bytes:
    key = brickMalloc(&bc, 9001);
    
    //stuff a small string into our allocated blocks:
    strncpy(blocks[key], "This is a nice program.", 23);
    
    //find the length of the string we stored:
    strLength = strlen(blocks[key]);
    ```

 - **Finding the length (in blocks) of an allocation:**
   One can discover the number of blocks used in an allocation by looking at how 
   many similar pointers are in the `char*` array.

   *Example:*

    ```
    //setup:
    brickContext* ctx;
    char* blocks[128]; //pointed at by ctx->blockptrlist.
    uint32 keyval;
    uint32 i;
    
    //accumulate the number of matching pointers in `i`.
    for(i = 0; (i < ctx->numBlocks) && (ctx->blockptrlist[i] == keyval); i++) { continue; }

    //`i` now holds the number of blocks in the allocation.
    printf("%d blocks used.\n", i);
    ```

 - **Out-of-order frees:**
   Freeing blocks out of order is safe since brick has no concept of nested/scoped memory allocation.

   *Example:*

    ```
    //setup:
    brickContext* ctx;
    uint32 id1;
    uint32 id2;
    uint32 id3;
    
    //allocate some blocks:
    id1 = brickMalloc(&bc, 5);
    id2 = brickMalloc(&bc, 23);
    id3 = brickMalloc(&bc, 781);

    //"free" those allocations:
    //note: these "frees" can be done out of order without breaking anything.
    brickFree(&bc, id2);
    brickFree(&bc, id3);
    brickFree(&bc, id1);
    ```


### Build
 - **\*nix-like:**
   - `$ make install` builds an example program that can be run with `$ ./example`.
   - `$ make test` builds and runs the test suite.
 - **Windows:**
   - `brick.vcxproj` is an MSVC 2010 project file that builds the example program.
     Just double click on it to generate a solution.
   - `test_brick_zero_write.vcxproj` is an MSVC 2010 project file that builds the lone test in the current test suite.
     Just double click on it to generate a solution.


### MIT License
Copyright (c) 2013 Philip Conrad.
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

   [1]: https://github.com/philipaconrad/brick/blob/master/example.c
