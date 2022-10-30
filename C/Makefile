CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g -fsanitize=address,undefined

all: hashmap run

hashmap: src/*.h src/*.c
	$(CC) $(CFLAGS) -o hashmap.exe src/hashmap.c

run:
	./hashmap.exe

clean:
	rm -f *.o *.exe
