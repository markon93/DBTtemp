CC = gcc
CFLAGS = -g -std=c99 -Wall

default: auto

auto: auto.o
	$(CC) $(CFLAGS) -o auto auto.o

all:
	auto.o

clean:
	rm auto *.o *~
