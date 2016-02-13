CC = g++
CFLAGS = -Wall -Werror -ansi -pedantic

all: src/rshell.o src/main.o
	mkdir -p bin
	$(CC) rshell.o main.o -o bin/rshell
src/rshell.o: src/rshell.cpp
	$(CC) $(CFLAGS) -c src/rshell.cpp
src/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c src/main.cpp
