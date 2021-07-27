#include "main.h"

static char *directives[]={".db",".dw", ".dh", ".asciz"};

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL};

void secondPass(LINE_FIELDS_T* linesHead, TABLE_NODE_T* tableHead, int ICF, int DCF){

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
					printf("Error");
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
				endWhileFlag = FLAGON;
				break;
		}
		if(endWhileFlag == FLAGON){
			break;
		}

	}
				
}
