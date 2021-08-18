#include "assembler.h"



int main(int argc, char** argv){
	if(argc == 1){
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
