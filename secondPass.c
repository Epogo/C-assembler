#include "assembler.h"

static char *directives[]={".db",".dw", ".dh", ".asciz"};

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL};

void secondPass(LINE_FIELDS_T* linesHead, TABLE_NODE_T* tableHead, int ICF, int DCF, char *filename){

	LINE_FIELDS_T* currentLine;
	TABLE_NODE_T* tableTmp;
	int i,directiveFlag,endWhileFlag,lastLineFlag,labelDetected,step;

	directiveFlag = FLAGOFF;
	endWhileFlag = FLAGOFF;
	lastLineFlag = FLAGOFF;
	labelDetected = FLAGOFF;

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
					printf("\n Error of unrecognized label should be printed \n");
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
				step = 8;
				break;
			case 8:
				if(lastLineFlag == FLAGON){
					step = 9;
				}
				else if(lastLineFlag == FLAGOFF){
					step = 1;
					currentLine = currentLine->next;
				}
				break;
			case 9:
				step = 10;
				break;
			case 10:
				createOutputFiles(tableHead,filename);
				endWhileFlag = FLAGON;
				break;
		}
		if(endWhileFlag == FLAGON){
			break;
		}

	}
				
}

void createOutputFiles(TABLE_NODE_T* tableHead, char *filename){
	TABLE_NODE_T* tableTmp;
	FILE *fptrEntry,*fptrExtern;
	int entryFlag,externFlag;
	char *filenameEntry;
	char *filenameExtern;
	char *tmpPtr;

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
			fprintf(fptrEntry,"%s %u\n",tableTmp->symbol,tableTmp->value);
			/*printf("%s %u\n",tableTmp->symbol,tableTmp->value);*/
		}
		tableTmp = tableTmp->next;
	}
	if(entryFlag == FLAGON){
		fclose(fptrEntry);
	}

	tableTmp = tableHead;
	while(1){
		if(tableTmp == NULL){
			break;
		}
		if(tableTmp->attribute[0] == EXTERNAL){
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
			fprintf(fptrExtern,"%s %u\n",tableTmp->symbol,tableTmp->value);
			/*printf("%s %u\n",tableTmp->symbol,tableTmp->value);*/
		}
		tableTmp = tableTmp->next;
	}
	if(externFlag == FLAGON){
		fclose(fptrExtern);
	}

}
