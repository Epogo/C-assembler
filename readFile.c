#include "main.h"


void readFile(int argc, char** argv){
	int i;
	char **fileNames;
	FILE *fd; /*declare the file pointer*/
	
	for(i=1; i<argc; i++){
		fileNames = argv; 
		/*fileNames[i][strlen(fileNames[i])-strlen(".as")] = '\0';*/
		/*printf("%s \n",fileNames[i]);*/

		if(!(fd = fopen(fileNames[i], "r"))){
			printf("Cannot open file \n"); /*if the file cannot be opened, print error message and exit program*/
			continue;
		}
		handleFileContents(fd);
	}
}
