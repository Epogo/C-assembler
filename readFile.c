/*This file is used to iterate over all arguments from the command line and open all the files entered as file arguments. The file prints errors, for example, of files that do not exist or files with incorrect extensions. The file calls other functions from other files once each file is successfully opened to further analyze the contents of the file.*/

#include "assembler.h" /*include assembler.h header file*/
#include "readFile.h" /*include readFile.h header file*/

/*The readFile function receives argc of type int and argv of type pointer to pointer to char as inputs, and is a void function so does not return anything. The argc is the argument counter and is used in a loop to iterate over all arguments entered from the command line, and argv is the argument vector which is used to store the name of each file and attempt to open the file. The function prints out an error if the file does not exist or the extension is not ".as". If the file is sucessfully opened the file pointer is passed to the handleFileContents function for further analysis is closed once the analysis is finished.*/
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

		if(!((fileNames[i][strlen(fileNames[i])-FILEEXTENSIONPOS1] == '.') && (fileNames[i][strlen(fileNames[i])-FILEEXTENSIONPOS2] == 'a') && (fileNames[i][strlen(fileNames[i])-FILEEXTENSIONPOS3] == 's'))){
			/*if file entered from the command line arguments does not have a .as ending print error message, free filename and continue to next file argument*/
			printf("File \"%s\" must be assembly file (.as) \n",fileNames[i]);
			free(filename);
			continue;
		}
		if(!(fd = fopen(fileNames[i], "r"))){
			/*fopen to initialize file pointer fd and if the file cannot be opened print error message, free filename, and continue to next file argument*/
			printf("Cannot open file \"%s\"\n",fileNames[i]);
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
