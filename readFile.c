#include "assembler.h"


void readFile(int argc, char** argv){
	int i,j; /*declare index variables*/
	char **fileNames; /*declare pointer to a pointer to a char storing vector of file names*/
	FILE *fd; /*declare the file pointer*/
	char *filename; /*declare pointer to a char storing filename*/
	char *tmpPtr; /*declare temporary pointer for allocating memory*/

	/*for loop iterating over all arguments from command line*/
	for(i=ARGCOUNTERSTART; i<argc; i++){


		/*allocate memory to store filename*/
		tmpPtr = (char*)calloc(MAXFILENAMELEN, sizeof(char));
		if(!tmpPtr)
		{
			printf("\nError! memory not allocated.");
			exit(0);
		}
		filename = tmpPtr; 



		fileNames = argv; /*assign argument vector to fileNames variable*/

		if(!(fd = fopen(fileNames[i], "r"))){
			/*fopen to initialize file pointer fd and if the file cannot be opened print error message, free filename, and continue to next file argument*/
			printf("Cannot open file \"%s\"\n",fileNames[i]);
			free(filename);
			continue;
		}
		if(!((fileNames[i][strlen(fileNames[i])-FILEEXTENSIONPOS1] == '.') && (fileNames[i][strlen(fileNames[i])-FILEEXTENSIONPOS2] == 'a') && (fileNames[i][strlen(fileNames[i])-FILEEXTENSIONPOS3] == 's'))){
			/*if file entered from the command line arguments does not have a .as ending print error message, free filename and continue to next file argument*/
			printf("File \"%s\" must be assembly file (.as) \n",fileNames[i]);
			free(filename);
			continue;
		}

		/*store file name from command line argument in filename array not including extension of .as*/
		for(j=STARTINDEX; j<strlen(fileNames[i]); j++){
			if(fileNames[i][j] == '.'){
				filename[j] = '\0';
				break;
			}	
			filename[j] = fileNames[i][j];
		}
		
		
		handleFileContents(fd,filename); /*enter handleFileContents function with file pointer fd and filename string as arguments to analyze contents of file*/

		fclose(fd); /*fclose of file pointer once analysis of file is complete*/

		free(filename); /*free the string containing the name of the file just analyzed*/
	}
}
