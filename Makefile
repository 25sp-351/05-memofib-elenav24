all: memofib

OBJS = memofib.o
CC = clang
CFLAGS = -g -Wall

memofib: $(OBJS)
	clang -o memofib $(CFLAGS) $(OBJS)

memofib.o: memofib.c

clean:
	rm -f memofib $(OBJS)
