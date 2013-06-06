block-alloc
=======

A small fixed-size block allocator suitable for use as a heap allocator.

### Usage Pattern
This allocator works well for fixed-memory environs, where all of the blocks in a block list can rest contiguously in memory.


### How it works
The system allocates an N^2 number of blocks (default is 64), each block being of a size specified at compile time (default is 64 bytes).
A bitmap of size N/8 bytes (where N is the number of blocks in the list) allows quick usage queries on the block list.
Each allocation is of the form `blockalloc(&bitmap, allocSize);`


### API:
 - `void* blockalloc(blockContext* ctx, int allocSize);`
   Ex: `blockalloc(&ctx, 56);`
 - `blockfree(blockContext* ctx, void* startblock, numBlocks);`


### Malloc/Free - legacy API:
 - `void* legacy_blockalloc(int allocSize);` Works just like `malloc()`.
 - `void legacy_blockfree(void* blockptr);` Works just like `free()`.
 - REQUIRED FOR SETUP: `void* legacy_init();` Sets up an alloction envrion for the legacy API.

### Documentation



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

   [1]: http://philipaconrad.github.io/mini-vm/
