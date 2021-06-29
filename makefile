main: symbolTable.o firstPass.o manageContents.o handleInput.o readFile.o main.o
	gcc -Wall -ansi -pedantic main.o readFile.o handleInput.o manageContents.o firstPass.o symbolTable.o -o main
main.o: main.c main.h
	gcc -c -Wall -ansi -pedantic main.c -o main.o
readFile.o: readFile.c main.h
	gcc -c -Wall -ansi -pedantic readFile.c -o readFile.o
handleInput.o: handleInput.c main.h
	gcc -c -Wall -ansi -pedantic handleInput.c -o handleInput.o
manageContents.o: manageContents.c main.h
	gcc -c -Wall -ansi -pedantic manageContents.c -o manageContents.o
firstPass.o: firstPass.c main.h
	gcc -c -Wall -ansi -pedantic firstPass.c -o firstPass.o
symbolTable.o: symbolTable.c main.h
	gcc -c -Wall -ansi -pedantic symbolTable.c -o symbolTable.o 
