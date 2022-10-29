CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g #-fsanitize=address,undefined

all: hashmap run

hashmap: multitype.c hashmap.c
	$(CC) $(CFLAGS) -o hashmap.exe hashmap.c

run:
	./hashmap.exe

clean:
	rm -f *.o *.exe
