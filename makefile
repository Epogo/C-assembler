main: firstPass.o handleInput.o readFile.o main.o
	gcc -Wall -ansi -pedantic main.o readFile.o handleInput.o firstPass.o -o main
main.o: main.c main.h
	gcc -c -Wall -ansi -pedantic main.c -o main.o
readFile.o: readFile.c main.h
	gcc -c -Wall -ansi -pedantic readFile.c -o readFile.o
handleInput.o: handleInput.c main.h
	gcc -c -Wall -ansi -pedantic handleInput.c -o handleInput.o
firstPass.o: firstPass.c main.h
	gcc -c -Wall -ansi -pedantic firstPass.c -o firstPass.o
