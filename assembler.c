/*This project has been made as the final project (MAMAN14) of the course "Systems Programming Laboratory".
Authors: David Muskal and Evgeni Pogoster.
DATE: 20.08.21
The purpose of this project is to implement an "Assembler" which is used as an interpreter only.
The assembler is able to handle unlimited amount of assembly files (.as files) and each file can contain unlimited
amount of lines.
Each line within the file is limited to 80 chars. 
Each file name is assumed to be limited to 100 chars.
The assembler will interpret each legal file (a file without any input errors).
For files which will have errors inside them-those files won't be interpreted, but all the 
errors which have been detected will be printed to the screen.

The main file of the assembler is the file "assembler.c", this file is responsible handling the file names which have been
entered through the command line.
The main function within the assembler file is calling to the function "readFile" which will read each .as file.
Each file content from each file which has been read is passed to the function handleFileContents in order to handle
all the content of the file which has been read.
The function "handleFileContents" is responsible to divide each file to a linked list of lines.
Than-the head of the lines linked-list is passed to the function "manageContents".
This function is checking each line in terms of finding any input errors.
If a line is a "command type" line-it will be checked by using the file "checkCommands.c",
else, if a line is a "data type" line-it will be checked by using the file "checkDirectives.c".
If the input is without errors-Than the content is passed to the "firstPass" function.
This function is responsible for the first-pass process, or in another words-this function
is responsible for the interpretation of each line to a binary code.
The "firstPass" function is using 3 important functions from the file "assToBin", those functions are:
"addNode","memAdd","concatNodes".
The memAdd function is used for the interpretation process of each line (from "assembly language" to binary),
The addNode function is used for adding each interpretated line (or, in other word-"Memory image node")
to the memory  Image linked-list.
At this stage there are two kinds of memory image linked-lists: A linked list for the commands and a linked-list
for the data. 
Each new memory image node is added to the suitable linked-list.
The function "concatNodes" is responsible for the unification of those two linked-lists.
The data linked-list is added to the end of the commands linked-list, and by that-a unified memory image linked
list has been formed.
The memory image which has been built by using the "firstPass" function might be incomplete.
If a given command is a branching command or a "J-type" command, One of the arguments might be a label.
Some Labels values might be unknown during the first-pass, so all the labels values are stored, during the first-pass,
in a symbol table by using the function "symbolAddNew" (from the assToBin file).
Than, The second-pass is launched from the firstPass file (by using the "secondPass.c" file which contains the
secondPass function).
The second pass is passing over the memory image and looks for uninterpreted labels.
Each uninterpreted label will be replaced with a symbol value (or a distance between symbol's value and the
instructions counter). 
If a given label couldn't be found at the symbol table-an error will be raised.
When the second-pass process has been done (without any errors)-few output files will be generated (for each input file).
For each legal .as file a .obj file will be created.
The .obj file will contain the final instruction counter value, final data counter value and the interpreted 
assembly lines (represented as hexadecimal chars).
If a given file had any .entry labels, .ent file will be created (This file will contain the .entry label and its instruction
count).
 If a given file had any .extern labels, .ext file will be created (This file will contain the .extern label and its instruction
count).
If one or more files had raised some errors-those errors will be printed to screen (with the relevant file name
and with an indication of the line number which has raised the error).
For files which had lines with errors inside them-any output files won't be generated.*/

#include "assembler.h"

/*The main function receives argc of type int and argv of type pointer to pointer to char and returns an int that is simply zero. The inputs are the argument counter and argument vector received from the command line. If the argument counter is 1 an error message is printed of not enough inputs and exits the program and otherwise the argument counter and argument vector is passed to the readFile function for further analysis.*/
int main(int argc, char** argv){
	if(argc == SINGLEARG){
		/*if no files are added as arguments in the command line, then print out message to user of not enough inputs and exit program*/
		printf("Not enough inputs \n");
		exit(0);
	}
	else{
		/*if arguments were added from the command line, enter readFile function with argument counter and argument vector as the function arguments*/
		readFile(argc,argv);
	}
	return 0;
}
