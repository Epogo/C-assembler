#include "assembler.h"

static char *directives[]={".db",".dw", ".dh", ".asciz"};

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL};


void freeSymbolAddNewStruct(SYMBOL_ADD_STRUCT_T *headStructPtr){
    SYMBOL_ADD_STRUCT_T *temp;
    SYMBOL_ADD_STRUCT_T *current;

    current=headStructPtr;
    /*While the linked list is not null-continue to delete nodes from the linked-list*/
    while(1){
        temp=current;

        current=current->next;
	
        free(temp);
	if (current==NULL){
            break;
	}
    }
}


void secondPass(LINE_FIELDS_T* linesHead, TABLE_NODE_T* tableHead, int ICF, int DCF, char *filename, MEMIM* memImHead,int symbolTableInitFlag){

	LINE_FIELDS_T* currentLine;
	TABLE_NODE_T* tableTmp;
	int i,directiveFlag,endWhileFlag,lastLineFlag,labelDetected,step,externalFlag,firstEntryFlag,errorFlag;
    	SYMBOL_ADD_STRUCT_T *structPtr;
    	SYMBOL_ADD_STRUCT_T *headStructPtr;
    	SYMBOL_ADD_STRUCT_T *currentStructPtr;
	/*static int errorFlag = FLAGOFF;*/
	errorFlag = FLAGOFF;
	directiveFlag = FLAGOFF;
	endWhileFlag = FLAGOFF;
	lastLineFlag = FLAGOFF;
	labelDetected = FLAGOFF;
	externalFlag = FLAGOFF;

	firstEntryFlag = FLAGON;

	step = 1;

	currentLine = linesHead;
	while(1){
		switch(step){
			case 1:
				if(currentLine->next == NULL){
					lastLineFlag = FLAGON;
				}
				step = 2;
				break;
			case 2:
				step = 3;
				break;
			case 3:
				step = 4;
				break;
			case 4:
				for(i=0;i<NUMDIRECTIVES;i++){
					if(!strcmp(directives[i], currentLine->comOrDir)){
						directiveFlag = FLAGON;
						
					}
				}
				if(directiveFlag == FLAGON && strcmp(".entry", currentLine->comOrDir)){
					if(lastLineFlag == FLAGON){
						step = 9;
					}
					else if(lastLineFlag == FLAGOFF){
						step = 1;
						currentLine = currentLine->next;
					}
				}
				else{
					step = 5;
				}
				directiveFlag = FLAGOFF;
				break;
			case 5:
				if(!strcmp(".entry", currentLine->comOrDir)){
					step = 6;
				}
				else{
					step = 7;
				}
				
				break;
			case 6:
				tableTmp = tableHead;
				while(1){
					if(tableTmp->next == NULL){
						if(!strcmp(currentLine->values,tableTmp->symbol)){
							tableTmp->attribute[1] = ENTRY;
							labelDetected = FLAGON;
						}
						break;
					}
					else{
						if(!strcmp(currentLine->values,tableTmp->symbol)){
							tableTmp->attribute[1] = ENTRY;
							labelDetected = FLAGON;
						}
						tableTmp = tableTmp->next;
					}
				}
				if(labelDetected == FLAGOFF){
					errorMsg(24,currentLine->lineNumber,currentLine->values);
					errorFlag = FLAGON;
				}
				labelDetected = FLAGOFF;
				if(lastLineFlag == FLAGON){
					step = 9;
				}
				else if(lastLineFlag == FLAGOFF){
					step = 1;
					currentLine = currentLine->next;
				}
				break;
			case 7:
				/*if(lastLineFlag == FLAGON){
					symbolAdd(memImHead,tableHead);
				}*/

				if(firstEntryFlag == FLAGON){
					structPtr = symbolAddNew(memImHead,tableHead,currentLine->lineNumber,firstEntryFlag);
					firstEntryFlag = FLAGOFF;
					headStructPtr = structPtr;
					headStructPtr->next = NULL;
					currentStructPtr = headStructPtr;
				}
				else{
					structPtr = symbolAddNew(memImHead,tableHead,currentLine->lineNumber,firstEntryFlag);
					currentStructPtr->next = structPtr;
					currentStructPtr = currentStructPtr->next;
					currentStructPtr->next = NULL;
				}


				/*structPtr = symbolAddNew(memImHead,tableHead,currentLine->lineNumber);*/
				
				step = 8;
				break;
			case 8:
				if(structPtr->externalFlag == FLAGON){
					externalFlag = FLAGON;
					/*if(externalFlag == FLAGOFF){
						externalHead = structPtr;
						externalCurrent = externalHead;
						externalCurrent->next = NULL;
						externalFlag = FLAGON;
					}
					else if(externalFlag == FLAGON){
						externalCurrent->next = structPtr;
						externalCurrent = externalCurrent->next;
						externalCurrent->next = NULL;	
					}*/
				}
				if(lastLineFlag == FLAGON){
					step = 9;
				}
				else if(lastLineFlag == FLAGOFF){
					step = 1;
					currentLine = currentLine->next;
				}
				break;
			case 9:
				if(errorFlag == FLAGON){
					endWhileFlag = FLAGON;
				}
				step = 10;
				break;
			case 10:
				/*printList(memImHead);*/
				createOutputFiles(memImHead,tableHead,filename,headStructPtr,ICF,DCF,errorFlag,symbolTableInitFlag,externalFlag);

				freeSymbolAddNewStruct(headStructPtr);

				endWhileFlag = FLAGON;
				break;
		}
		if(endWhileFlag == FLAGON){
			break;
		}

	}
				
}

void createOutputFiles(MEMIM* memImHead, TABLE_NODE_T* tableHead, char *filename,SYMBOL_ADD_STRUCT_T *headStructPtr, int ICF, int DCF,int errorFlag,int symbolTableInitFlag,int externalFlag){
	TABLE_NODE_T* tableTmp;
	SYMBOL_ADD_STRUCT_T* tmpStructPtr;
	FILE *fptrEntry,*fptrExtern,*fptrObject;
	int entryFlag,externFlag;
	char *filenameEntry;
	char *filenameExtern;
	char *filenameObject;
	char *tmpPtr;
	int ICFminus100;

	ICFminus100 = ICF - 100;
	

	if(errorFlag == FLAGOFF){
		tmpPtr = (char*)calloc(100, sizeof(char));
		if(!tmpPtr)
		{
			printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
			exit(0);
		}
		filenameEntry = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

		entryFlag = FLAGOFF;

		tmpPtr = (char*)calloc(100, sizeof(char));
		if(!tmpPtr)
		{
			printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
			exit(0);
		}
		filenameExtern = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

		entryFlag = FLAGOFF;
		externFlag = FLAGOFF;

		if(symbolTableInitFlag == FLAGON){
			tableTmp = tableHead;
			while(1){
				if(tableTmp == NULL){
					break;
				}
				if(tableTmp->attribute[1] == ENTRY){
					if(entryFlag == FLAGOFF){
						strcat(filenameEntry,filename);
						strcat(filenameEntry,".ent");
						fptrEntry = fopen(filenameEntry,"w");
						if(fptrEntry == NULL){
							printf("Error... Unable to write to file");
							exit(0);
						}
						entryFlag = FLAGON;
					}
					fprintf(fptrEntry,"%s 0%u\n",tableTmp->symbol,tableTmp->value);
					/*printf("%s %u\n",tableTmp->symbol,tableTmp->value);*/
				}
				tableTmp = tableTmp->next;
			}

			if(entryFlag == FLAGON){
				fclose(fptrEntry);
			}
		}

		/*if(externalFlag == FLAGON){

			externalTmp = externalHead;
			while(1){
				if(externalTmp == NULL){
					break;
				}

				if(externFlag == FLAGOFF){
					strcat(filenameExtern,filename);
					strcat(filenameExtern,".ext");
					fptrExtern = fopen(filenameExtern,"w");
					if(fptrExtern == NULL){
						printf("Error... Unable to write to file");
						exit(0);
					}

					externFlag = FLAGON;
				}
				fprintf(fptrExtern,"%s 0%u\n",externalTmp->label,externalTmp->address);

				externalTmp = externalTmp->next;
			}
			if(externFlag == FLAGON){
				fclose(fptrExtern);
			}
		}*/

		if(externalFlag == FLAGON){

			tmpStructPtr = headStructPtr;
			while(1){
				if(tmpStructPtr == NULL){
					break;
				}
				if(tmpStructPtr->externalFlag == FLAGON){
					if(externFlag == FLAGOFF){
						strcat(filenameExtern,filename);
						strcat(filenameExtern,".ext");
						fptrExtern = fopen(filenameExtern,"w");
						if(fptrExtern == NULL){
							printf("Error... Unable to write to file");
							exit(0);
						}

						externFlag = FLAGON;
					}
					fprintf(fptrExtern,"%s 0%u\n",tmpStructPtr->label,tmpStructPtr->address);
				}
				tmpStructPtr = tmpStructPtr->next;
			}
			if(externFlag == FLAGON){
				fclose(fptrExtern);
			}
		}
		free(filenameEntry);
		free(filenameExtern);
	}


	tmpPtr = (char*)calloc(100, sizeof(char));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	filenameObject = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	strcat(filenameObject,filename);
	strcat(filenameObject,".ob");

	fptrObject = fopen(filenameObject,"w");
	if(fptrObject == NULL){
		printf("Error... Unable to write to file");
		exit(0);
	}
	fprintf(fptrObject,"     %u %u\n",ICFminus100,DCF);

	printListToFile(memImHead,fptrObject);

	/*freeList(memImHead,fptrObject);*/

	fclose(fptrObject);

	if(errorFlag == FLAGON){
		remove(filenameObject);
	}

	free(filenameObject);

}
