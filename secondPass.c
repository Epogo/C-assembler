/*This file contains functions for the second pass algorithm and for outputting files. The file is entered once the secondPass function is called from within the firstPass, and the secondPass algorithm is executed, finally outputting files if no errors were detected.*/

#include "assembler.h" /*include assembler.h header file*/
#include "secondPass.h" /*include secondPass.h header file*/

static char *directives[]={".db",".dw", ".dh", ".asciz"}; /*array of directives*/

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL}; /*enum of symbol table attributes*/


/*The secondPass function receives linesHead of type pointer to LINE_FIELDS_T, tableHead of type pointer to TABLE_NODE_T, ICF of type int, DCF of type int, filename of type pointer to char, memImHead of type pointer to MEMIM, symbolTableInitFlag of type int, and does not return anything as it is a void function. The function iterates through the second pass algorithm after it is called from the firstPass function using linesHead to iterate through all the lines of the file, and tableHead to iterate through the symbol table, and if no errors were detected, the algorithm proceeds to complete the memory image, using the memImHead passed to the function from the firstPass and finally outputs the corresponding files (.ob,.ext.ent).*/
void secondPass(LINE_FIELDS_T* linesHead, TABLE_NODE_T* tableHead, int ICF, int DCF, char *filename, MEMIM* memImHead,int symbolTableInitFlag){

	enum steps {STEP1, STEP2, STEP3,  STEP4, STEP5, STEP6, STEP7, STEP8, STEP9, STEP10}; /*enum of all steps of state machine of second pass algorithm*/

	LINE_FIELDS_T* currentLine; /*pointer to current line of line fields linked list*/
	TABLE_NODE_T* tableTmp; /*temporary pointer to symbol table*/
	int i,directiveFlag,endWhileFlag,lastLineFlag,labelDetected,step,externalFlag,firstEntryFlag,errorFlag;
    	SYMBOL_ADD_STRUCT_T *structPtr; /*pointer to linked list to store nodes from symbolAddNew function*/
    	SYMBOL_ADD_STRUCT_T *headStructPtr; /*head of linked list to store nodes from symbolAddNew function*/
    	SYMBOL_ADD_STRUCT_T *currentStructPtr; /*pointer to current node of linked list to store nodes from symbolAddNew function*/
	errorFlag = FLAGOFF; /*set errorFlag to flag off*/
	directiveFlag = FLAGOFF; /*set errorFlag to flag off*/
	endWhileFlag = FLAGOFF; /*set endWhileFlag to flag off*/
	lastLineFlag = FLAGOFF; /*set lastLineFlag to flag off*/
	labelDetected = FLAGOFF; /*set labelDetected to flag off*/
	externalFlag = FLAGOFF; /*set externalFlag to flag off*/

	firstEntryFlag = FLAGON; /*set firstEntryFlag to on*/

	step = STEP1; /*set intitial step of state machine of second pass algorithm to step 1*/

	currentLine = linesHead; /*set current line to head of linked list of lines*/
	while(1){
		/*switch case for all steps of second pass algorithm*/
		switch(step){
			case STEP1:
				/*check if last line reached*/
				if(currentLine->next == NULL){
					lastLineFlag = FLAGON; /*set lastLineFlag to on*/
					step = STEP9; /*proceed to step 9*/
				}
				else{
					step = STEP2; /*proceed to step 2*/
				}
				break;
			case STEP2:
				step = STEP3; /*proceed to step 3*/
				break;
			case STEP3:
				step = STEP4; /*proceed to step 4*/
				break;
			case STEP4:
				/*check if current line contains directive*/
				for(i=STARTINDEX;i<NUMDIRECTIVES;i++){
					if(!strcmp(directives[i], currentLine->comOrDir)){
						directiveFlag = FLAGON; /*set directiveFlag to on*/
						
					}
				}
				if(!strcmp(".extern", currentLine->comOrDir)){
					directiveFlag = FLAGON; /*if current line contains ".extern" set directiveFlag to on*/
					
				}
				/*check if directive flag is on and current line does not contain ".entry"*/
				if(directiveFlag == FLAGON && strcmp(".entry", currentLine->comOrDir)){
					if(lastLineFlag == FLAGON){
						step = STEP9; /*if lastLineFlag is on proceed to step 9*/
					}
					else if(lastLineFlag == FLAGOFF){
						step = STEP1; /*if lastLineFlag is off return to step 1*/
						currentLine = currentLine->next; /*set current line to next line*/
					}
				}
				else{
					step = STEP5; /*proceed to step 5*/
				}
				directiveFlag = FLAGOFF;
				break;
			case STEP5:
				/*check if current line contains ".entry"*/
				if(!strcmp(".entry", currentLine->comOrDir)){
					step = STEP6; /*proceed to step 6*/
				}
				else{
					step = STEP7; /*proceed to step 7*/
				}
				
				break;
			case STEP6:
				tableTmp = tableHead; /*sets temporary pointer to symbol table to point to head*/
				/*iterates over all of symbol table*/
				while(tableTmp!=NULL){
					/*check if label of entry also defined as label of external*/
					if((!strcmp(tableTmp->symbol,currentLine->values)) && (tableTmp->attribute[ATTRIBUTE1] == EXTERNAL)){
						errorMsg(ERRORTYPE34,currentLine->lineNumber,currentLine->values,filename); /*print error message*/
						errorFlag = FLAGON; /*set error flag to on*/
						break;
					}
					tableTmp = tableTmp->next; /*iterate to next node in symbol table*/
				}

				tableTmp = tableHead;/*set temporary pointer to symbol table back to point to head*/
				while(1){
					/*iterates over all of symbol table*/
					if(tableTmp->next == NULL){
						/*look for current label in symbol table*/
						if(!strcmp(currentLine->values,tableTmp->symbol)){
							tableTmp->attribute[ATTRIBUTE2] = ENTRY; /*set second attribute to entry*/
							labelDetected = FLAGON; /*set labelDetected to on*/
						}
						break;
					}
					else{
						/*look for current label in symbol table*/
						if(!strcmp(currentLine->values,tableTmp->symbol)){
							tableTmp->attribute[ATTRIBUTE2] = ENTRY; /*set second attribute to entry*/
							labelDetected = FLAGON; /*set labelDetected to on*/
						}
						tableTmp = tableTmp->next; /*iterate to next node in symbol table*/
					}
				}
				/*check if labelDetected flag is off*/
				if(labelDetected == FLAGOFF){
					errorMsg(ERRORTYPE24,currentLine->lineNumber,currentLine->values,filename); /*print error message*/
					errorFlag = FLAGON; /*set error flag to on*/
				}
				labelDetected = FLAGOFF; /*set labelDetected flag to off*/
				if(lastLineFlag == FLAGON){
					step = STEP9; /*if last line proceed to step 9*/
				}
				else if(lastLineFlag == FLAGOFF){
					step = STEP1; /*if not last line, return to step 1*/
					currentLine = currentLine->next; /*set current line to next line*/
				}
				break;
			case STEP7:
				/*check if firstEntryFlag is on*/
				if(firstEntryFlag == FLAGON){
					structPtr = symbolAddNew(memImHead,tableHead,currentLine->lineNumber,firstEntryFlag,filename); /*complete memory image for current line*/
					firstEntryFlag = FLAGOFF; /*set firstEntryFlag to off*/
					headStructPtr = structPtr; /*set head of linked list to current node of struct returned from symbolAddNew*/
					headStructPtr->next = NULL; /*set next node to null*/
					currentStructPtr = headStructPtr; /*set current node to head*/
				}
				else{
					structPtr = symbolAddNew(memImHead,tableHead,currentLine->lineNumber,firstEntryFlag,filename); /*complete memory image for current line*/
					currentStructPtr->next = structPtr; /*set next node of current to point to struct returned from symbolAddNew*/
					currentStructPtr = currentStructPtr->next; /*set current node to next node*/
					currentStructPtr->next = NULL; /*set next node to NULL*/
				}

				/*check if errorFlag of struct returned from symbolAddNew is on*/
				if(structPtr->errorFlag == FLAGON){
					errorFlag = FLAGON; /*set errorFlag to on*/
					currentLine = currentLine->next; /*set current line to next line*/
					step = STEP1; /*return to step 1*/
				}
				else{
					step = STEP8; /*proceed to step 8*/
				}
				
							
				break;
			case STEP8:
				/*check if externalFlag is on from struct returned from symbolAddNew*/
				if(structPtr->externalFlag == FLAGON){
					externalFlag = FLAGON; /*set externalFlag to on*/
				}
				if(lastLineFlag == FLAGON){
					step = STEP9; /*if last line, proceed to step 9*/
				}
				else if(lastLineFlag == FLAGOFF){
					step = STEP1; /*if not last line, return to step 1*/
					currentLine = currentLine->next; /*set current line to next line*/
				}
				break;
			case STEP9:
				/*check if errorFlag is on*/
				if(errorFlag == FLAGON){
					endWhileFlag = FLAGON; /*end while loop of state machine of second pass of algorithm and do not output files*/
					if(firstEntryFlag == FLAGOFF){
						freeSymbolAddNewStruct(headStructPtr); /*if symbolAddNew entered, free all structs returned from function*/
					}
				}
				step = STEP10; /*proceed to step 10*/
				break;
			case STEP10:
				createOutputFiles(memImHead,tableHead,filename,headStructPtr,ICF,DCF,errorFlag,symbolTableInitFlag,externalFlag); /*call createOutputFiles function to output corresponding files*/

				if(firstEntryFlag == FLAGOFF){
					freeSymbolAddNewStruct(headStructPtr); /*if symbolAddNew entered, free all structs returned from function*/
				}

				endWhileFlag = FLAGON; /*end while loop of state machine of second pass algorithm*/
				break;
		}
		if(endWhileFlag == FLAGON){
			break; /*break out of while loop of state machine of second pass algorithm*/
		}

	}
				
}


/*The createOutputFiles function receives memImHead of type pointer to MEMIM, tableHead of type pointer to TABLE_NODE_T, filename of type pointer to char, external head of type pointer to SYMBOL_ADD_STRUCT_T, ICF of type int, DCF of type int, errorFlag of type int, symbolTableInitFlag of type int and externalFlag of type int, and does not return anything, but creates and writes to files if necessary. The function creates output files of type .ob,.ext, and .ent. To do this, the file receives as inputs the head of the memory image, memImHead, which is used to print to the object file, the head of the symbol table, tableHead, which is used to create the extern file, the input file name which is used to name the output files, externalHead which is used to create the extern files, ICF and DCF which are used to label the object file, errorFlag which alerts if files should be printed or not, symbolTableInitFlag which alerts if symbol table was initialized, and externalFlag, which alerts if externals were used and if an ext file should be created.*/
void createOutputFiles(MEMIM* memImHead, TABLE_NODE_T* tableHead, char *filename,SYMBOL_ADD_STRUCT_T *headStructPtr, int ICF, int DCF,int errorFlag,int symbolTableInitFlag,int externalFlag){
	TABLE_NODE_T* tableTmp; /*temporary pointer to symbol table*/
	SYMBOL_ADD_STRUCT_T* tmpStructPtr; /*temporary pointer to linked list of structs returned from symbolAddNew*/
	FILE *fptrEntry,*fptrExtern,*fptrObject; /*file pointers for output files*/
	int entryFlag,externFlag; /*flags to notify if entry or extern in file*/
	char *filenameEntry; /*pointer to char to store name of output entry file*/
	char *filenameExtern; /*pointer to char to store name of output extern file*/
	char *filenameObject; /*pointer to char to store name of output object file*/
	char *tmpPtr; /*temporary pointer for memory allocation*/
	int ICFminus100; /*int to store ICF minus initial IC*/

	ICFminus100 = ICF - ICINIT; /*Instruction counter final - initial instruction counter*/
	
	/*check if errorFlag is off*/
	if(errorFlag == FLAGOFF){
		tmpPtr = (char*)calloc(MAXFILENAMELEN, sizeof(char)); /*allocate memory to store name of output entry file*/
		if(!tmpPtr)
		{
			printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
			exit(0);
		}
		filenameEntry = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

		entryFlag = FLAGOFF; /*set entryFlag to off*/

		tmpPtr = (char*)calloc(MAXFILENAMELEN, sizeof(char)); /*allocate memory to store name of output extern file*/
		if(!tmpPtr)
		{
			printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
			exit(0);
		}
		filenameExtern = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

		entryFlag = FLAGOFF; /*set entryFlag to off*/
		externFlag = FLAGOFF; /*set externFlag to off*/

		/*check if symbol table initialized*/
		if(symbolTableInitFlag == FLAGON){
			tableTmp = tableHead; /*set temporary pointer to symbol table to head of symbol table*/
			while(1){
				/*iterate until end of symbol table*/
				if(tableTmp == NULL){
					break;
				}
				/*check if second attribute of current node in symbol table is entry*/
				if(tableTmp->attribute[ATTRIBUTE2] == ENTRY){
					/*check if entryFlag is off*/
					if(entryFlag == FLAGOFF){
						strcat(filenameEntry,filename); /*place filename in filenameEntry*/
						strcat(filenameEntry,".ent"); /*add .ent extension to filenameEntry*/
						fptrEntry = fopen(filenameEntry,"w"); /*open file for writing*/
						/*check if file successfully opened*/
						if(fptrEntry == NULL){
							printf("Error... Unable to write to file");
							exit(0);
						}
						entryFlag = FLAGON; /*set entryFlag to on*/
					}
					fprintf(fptrEntry,"%s 0%u\n",tableTmp->symbol,tableTmp->value); /*write to file*/
				}
				tableTmp = tableTmp->next; /*iterate to next node in symbol table*/
			}

			/*check if entryFlag is on*/
			if(entryFlag == FLAGON){
				fclose(fptrEntry); /*close file*/
			}
		}

		/*check if externFlag is on*/
		if(externalFlag == FLAGON){

			tmpStructPtr = headStructPtr; /*set temporary struct pointer to point to head of linked list of structs returned from symbolAddNew*/
			while(1){
				/*iterate over structs returned from symbolAddNew*/
				if(tmpStructPtr == NULL){
					break;
				}
				/*check if externalFlag of current struct is on*/
				if(tmpStructPtr->externalFlag == FLAGON){
					/*check if externFlag is off*/
					if(externFlag == FLAGOFF){
						strcat(filenameExtern,filename); /*place filename in filenameExtern*/
						strcat(filenameExtern,".ext"); /*add .ext extension to filenameExtern*/
						fptrExtern = fopen(filenameExtern,"w"); /*open file for writing*/
						/*if file not opened successfully*/
						if(fptrExtern == NULL){
							printf("Error... Unable to write to file");
							exit(0);
						}

						externFlag = FLAGON; /*set externFlag to on*/
					}
					fprintf(fptrExtern,"%s 0%u\n",tmpStructPtr->label,tmpStructPtr->address); /*write to file*/
				}
				tmpStructPtr = tmpStructPtr->next; /*iterate to next node of structs returned from symbolAddNew*/
			}
			/*check if externFlag is on*/
			if(externFlag == FLAGON){
				fclose(fptrExtern); /*close file*/
			}
		}
		free(filenameEntry); /*free filenameEntry storing name of entry file*/
		free(filenameExtern); /*free filenameExtern storing name of extern file*/
	}


	tmpPtr = (char*)calloc(MAXFILENAMELEN, sizeof(char)); /*allocate memory to store name of output object file*/
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	filenameObject = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	strcat(filenameObject,filename); /*store filename is filenameObject*/
	strcat(filenameObject,".ob"); /*add .ob extension to filenameObject*/

	fptrObject = fopen(filenameObject,"w"); /*open file for writing*/
	/*if file not opened successfully*/
	if(fptrObject == NULL){
		printf("Error... Unable to write to file");
		exit(0);
	}
	fprintf(fptrObject,"     %u %u\n",ICFminus100,DCF); /*write to file*/

	printListToFile(memImHead,fptrObject); /*call printListToFile function to write to file*/

	fclose(fptrObject); /*close file*/

	if(errorFlag == FLAGON){
		remove(filenameObject); /*if errorFlag is on remove file*/
	}

	free(filenameObject); /*free filenameObject storing name of object file*/

}

/*The freeSymbolAddNewStruct function receives headStructPtr of type SYMBOL_ADD_STRUCT_T* and returns nothing as it is a void function. The function iterates through the list of nodes returned from symbolAddNew and frees every node until the end of the linked list is reached.*/
void freeSymbolAddNewStruct(SYMBOL_ADD_STRUCT_T *headStructPtr){
    SYMBOL_ADD_STRUCT_T *temp; /*temporary pointer to SYMBOL_ADD_STRUCT_T*/
    SYMBOL_ADD_STRUCT_T *current; /*current pointer to SYMBOL_ADD_STRUCT_T*/

    current=headStructPtr; /*set current node to headStructPtr*/

    /*While the linked list is not null-continue to free nodes from the linked-list*/
    while(1){
	temp=current; /*set temp node to current node*/

	current=current->next; /*set current node to next node*/
	
	free(temp); /*free temp node*/
	if (current==NULL){
	    break; /*break from while loop once end of linked list reached*/
	}
    }
}
