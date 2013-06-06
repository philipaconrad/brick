SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .h .c .o .lib .s
BRICK_SOURCES = types.h brick.h brick.c

all: install

install:
	$(CC) -I. -I$(srcdir) $(CFLAGS) -g example.c $(BRICK_SOURCES) -o brick.exe

clean:
	rm brick.exe
