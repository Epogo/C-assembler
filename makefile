assembler: assToBin.o checkDirectives.o checkCommands.o secondPass.o symbolTable.o firstPass.o manageContents.o handleInput.o readFile.o assembler.o
	gcc -Wall -ansi -pedantic assembler.o readFile.o handleInput.o manageContents.o firstPass.o symbolTable.o secondPass.o checkCommands.o checkDirectives.o assToBin.o -o assembler -lm
assembler.o: assembler.c assembler.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
readFile.o: readFile.c assembler.h
	gcc -c -Wall -ansi -pedantic readFile.c -o readFile.o
handleInput.o: handleInput.c assembler.h
	gcc -c -Wall -ansi -pedantic handleInput.c -o handleInput.o
manageContents.o: manageContents.c assembler.h
	gcc -c -Wall -ansi -pedantic manageContents.c -o manageContents.o
firstPass.o: firstPass.c assembler.h
	gcc -c -Wall -ansi -pedantic firstPass.c -o firstPass.o
symbolTable.o: symbolTable.c assembler.h
	gcc -c -Wall -ansi -pedantic symbolTable.c -o symbolTable.o 
secondPass.o: secondPass.c assembler.h
	gcc -c -Wall -ansi -pedantic secondPass.c -o secondPass.o 
checkCommands.o: checkCommands.c assembler.h
	gcc -c -Wall -ansi -pedantic checkCommands.c -o checkCommands.o
checkDirectives.o: checkDirectives.c assembler.h
	gcc -c -Wall -ansi -pedantic checkDirectives.c -o checkDirectives.o
assToBin.o: assToBin.c assembler.h
	gcc -c -Wall -ansi -pedantic assToBin.c -o assToBin.o -lm
