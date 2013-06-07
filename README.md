brick
=======

A small, fixed-size block allocator suitable for use as a heap allocator.
This allocator works well for fixed-memory environs, where all of the blocks in a block list can rest contiguously in memory.

### How it works

A `char*` array is mapped one-to-one with a contiguous chunk of memory, 
each pointer corresponding to a matching "block" of a uniform, predetermined size.

    ```
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
    ```

The result is essentially an overlay of malloc and free. See the [example][1] program for how this works in practice.


### API
 - `void   brickInit(brickContext* ctx, char** blockPtrList, char* memory, uint32 numBlocks, uint32 blockSize);`
 - `uint32 brickFindOpenRun(brickContext* ctx, uint32 length);`
 - `uint32 brickMalloc(brickContext* ctx, uint32 size);`
 - `void   brickFree(brickContext* ctx, uint32 key);`
 - `void   brickGC(brickContext* ctx);` **Warning:** Not implemented yet.


### Idioms
 - **Malloc Error Check:**
   To see if `brickMalloc()` failed, check to see if its return value is equivalent to BRICK_MALLOC_ERROR.

   *Example:*

    ```
    uint32 key = brickMalloc(&bc, 9001);
    if(key == BRICK_MALLOC_ERROR) {
        /* ... failure-handling code ... */
    }
    ```

 - **Accessing an allocated block:**
   To access memory blocks allocated with brick, just access the `char*` array by 
   the key provided by `brickMalloc()`. If 2 blocks were used in the allocation, 
   then the next index's pointer (at key+1) will point to the same location. See 

   *Example:*
   
    ```
    uint32 key = brickMalloc(&bc, 9001);
    char* src  = blocks[key];
    uint32 strLength = strlen(src);
    ```

 - **Find the length (in blocks) of an allocation:**
   When an allocation occurs, brick matches the pointers in the `char*` array 
   by index with the blocklist, and sets the matching `char*`s to all point to 
   the start of the allocation. This means that one can discover the number of blocks
   used in an allocation by looking at how many similar pointers are in the `char*` array.

   *Example:*

    ```
    brickContext* ctx;
    uint32 keyval;
    uint32 i = 0;
    
    //accumulate the number of matching pointers in `i`.
    for(; (i < ctx->numBlocks) && (ctx->blockptrlist[i] == keyval); i++) { continue; }

    //`i` now holds the number of blocks in the allocation.
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
