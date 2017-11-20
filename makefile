all: main.c
	gcc -o work13 main.c

run: all
	./work13
