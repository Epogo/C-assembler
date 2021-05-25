#include "main.h"



int main(int argc, char** argv){
	if(argc == 1){
		printf("Not enough inputs \n");
		exit(0);
	}
	else{
		readFile(argc,argv);
	}
	return 0;
}

/*add comment*/
