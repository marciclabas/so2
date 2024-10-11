CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -g

main: main.o list.o
	$(CC) $(CFLAGS) -o main main.o list.o

list.o: list.c list.h
main.o: main.c list.h

clean:
	rm -f main main.o list.o