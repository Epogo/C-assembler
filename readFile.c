#include "assembler.h"


void readFile(int argc, char** argv){
	int i,j;
	char **fileNames;
	FILE *fd; /*declare the file pointer*/
	char *filename;
	char *tmpPtr;

	
	for(i=1; i<argc; i++){



		tmpPtr = (char*)calloc(100, sizeof(char));
		if(!tmpPtr)
		{
			printf("\nError! memory not allocated.");
			exit(0);
		}
		filename = tmpPtr; 




		fileNames = argv; 
		/*fileNames[i][strlen(fileNames[i])-strlen(".as")] = '\0';*/
		/*printf("%s \n",fileNames[i]);*/

		if(!(fd = fopen(fileNames[i], "r"))){
			printf("Cannot open file \n"); /*if the file cannot be opened, print error message and exit program*/
			continue;
		}
		if(!((fileNames[i][strlen(fileNames[i])-3] == '.') && (fileNames[i][strlen(fileNames[i])-2] == 'a') && (fileNames[i][strlen(fileNames[i])-1] == 's'))){
			printf("File \"%s\" must be assembly file (.as) \n",fileNames[i]);
			continue;
		}

		for(j=0; j<strlen(fileNames[i]); j++){
			if(fileNames[i][j] == '.'){
				filename[j] = '\0';
				break;
			}	
			filename[j] = fileNames[i][j];
		}
		
		handleFileContents(fd,filename);

		fclose(fd);

		free(filename);
	}
}
