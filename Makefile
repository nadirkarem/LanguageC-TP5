# Makefile

CC = gcc
CFLAGS = -Wall -Wextra

all: Image 

Image: image.c image.h
	$(CC) $(CFLAGS) -w image.c -o Image

clean:
	rm -f Image
	rm -f lena_c*.pgm
