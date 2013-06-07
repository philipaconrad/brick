SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .h .c .o .lib .s
BRICK_SOURCES = types.h brick.h brick.c
BRICK_TEST_SOURCES = greatest.h

all: install

install:
	$(CC) -I. -I$(srcdir) $(CFLAGS) -g example.c $(BRICK_SOURCES) -o example

clean:
	rm example

test:
	$(CC) -I. -I$(srcdir) $(CFLAGS) -g test_brick_zero_write.c $(BRICK_SOURCES) $(BRICK_TEST_SOURCES) -o test_brick_zero_write -Wall
	mkdir test
	mv test_brick_zero_write test/test_brick_zero_write
	./test/test_brick_zero_write