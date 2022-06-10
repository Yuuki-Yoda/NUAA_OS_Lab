main: main.o math.o
	gcc -o main main.o math.o

main.o:main.c
	gcc -c main.c

math.o:math.c
	gcc -c math.c

clean:
	rm -f *.o
