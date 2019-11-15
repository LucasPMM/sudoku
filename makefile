all: src/main.o src/core.o
	gcc -O3 src/main.o src/core.o -lm -o tp3
	make clean
	clear

core.o: src/core.c includes/core.h
	gcc -O3 -c src/core.c

main.o: src/main.c
	gcc -O3 -c src/main.c

clean:
	rm src/*.o