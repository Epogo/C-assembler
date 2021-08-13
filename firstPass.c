#include "assembler.h"

static char *directives[]={".db",".dw", ".dh", ".asciz"};

/*static char *commands[]={"add","sub", "and", "or", "nor", "move", "mvhi","mvlo", "addi", "subi", "andi", "ori","nori", "bne", "beq", "blt", "bgt","lb", "sb", "lw", "sw", "lh","sh", "jmp", "la", "call", "stop"};*/

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL};


void freeHeadData(MEMIM* node){
    MEMIM *temp;
    DATA *tempData;

    temp=node;
    while(1){
	temp = node;
        if (temp==NULL){
            break;
	}
	if(temp->p!=NULL){
		while(temp->p!=NULL){
			tempData = temp->p;
			temp->p = temp->p->next;
			free(tempData);
		}
	}
	node = node->next;
    }
}

void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int errorDetected,char *filename,int lineNumber){
	static int IC,DC;
	int i,directiveFlag,endWhileFlag,DCF,ICF;
	static TABLE_NODE_T* tableHead;
	static LINE_FIELDS_T* linesHead;
	TABLE_NODE_T* tableTmp;
	/*enum Attributes Attribute;*/
	static int step = 1;
	static MEMIM* headCom;
	static MEMIM* headData;
	static char fileNamePrev[100];
	MEMIM* memImHead;
	MEMIM* node;
	static int firstDataFlag = FLAGOFF;
	static int firstComFlag = FLAGOFF;
	static int errorFlag = FLAGOFF;
	static int firstLineFlag;
	static int firstSymbolFlag;
	static int firstComNodeAddflag;
	static int firstDataNodeAddflag;
	static int symbolTableInitFlag;
	directiveFlag = FLAGOFF;
	endWhileFlag = FLAGOFF;
	firstLineFlag = FLAGOFF;

	/*printf("%s, %s, %s\n",ptrField1,ptrField2,ptrField3);*/

	if(strcmp(filename,fileNamePrev)){
		step = 1;
		firstDataFlag = FLAGOFF;
		firstComFlag = FLAGOFF;
		errorFlag = FLAGOFF;
		firstLineFlag = FLAGON;
		firstSymbolFlag = FLAGOFF;
		firstComNodeAddflag = FLAGOFF;
		firstDataNodeAddflag = FLAGOFF;
		symbolTableInitFlag = FLAGOFF;
	}
	strcpy(fileNamePrev,filename);

	/*printf("Filename: %s\n",filename);*/

	if(labelFlag == FLAGON){
		symbolTableInitFlag = FLAGON;
	}

	while(1){
		switch(step){
			case 1:
				DC = 0;
				IC = 100;
				step = 2;
				break;
			case 2:
				if(errorDetected == FLAGON){
					errorFlag = FLAGON;
					if(labelFlag != LASTLINE){
						endWhileFlag = FLAGON;
						break;
					}
				}
				if(errorFlag == FLAGOFF){
					linesHead = storeLineFields(ptrField1,ptrField2,ptrField3,labelFlag,lineNumber,firstLineFlag);
				}

				if(labelFlag == LASTLINE){
					step = 17;
				}
				else{
					step = 6;
				}
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				for(i=0;i<NUMDIRECTIVES;i++){
					if(!strcmp(directives[i], ptrField2)){
						directiveFlag = FLAGON;
						
					}
				}
				if(directiveFlag == FLAGON){
					step = 7;
					directiveFlag = FLAGOFF; /*check if line necessary*/
				}
				else if(directiveFlag == FLAGOFF){
					step = 9;
				}				
				break;
			case 7:
				if(errorFlag == FLAGOFF){
					if(labelFlag == FLAGON){
						if(firstSymbolFlag == FLAGOFF){
							tableHead = symbolTable(ptrField1,DC,MYDATA,EMPTY,FLAGON);
							firstSymbolFlag = FLAGON;
						}
						else{
							tableHead = symbolTable(ptrField1,DC,MYDATA,EMPTY,FLAGOFF);
						}
					}
				}
				step = 8;
				break;
			case 8:
				if(errorFlag == FLAGOFF){
					/*Add to "Tmunat Hazikaron"*/
					if(firstDataFlag == FLAGOFF){
						headData = memAdd(ptrField1,ptrField2,ptrField3);
						node = headData;
						firstDataFlag = FLAGON;
					}
					else{
						/*if(firstComFlag == FLAGOFF){
							headCom = NULL;
						}*/
						/*node = memAdd(ptrField1,ptrField2,ptrField3);
						addNode(headCom,headData,node);*/

						/*if(firstComFlag == FLAGOFF){
							headCom = NULL;
						}*/

						if(firstDataNodeAddflag == FLAGOFF){
							node = memAdd(ptrField1,ptrField2,ptrField3);
							addNode(headCom,headData,node,FLAGOFF,firstComNodeAddflag);
							firstDataNodeAddflag = FLAGON;
						}
						else{
							node = memAdd(ptrField1,ptrField2,ptrField3);
							addNode(headCom,headData,node,FLAGON,firstComNodeAddflag);
						}
					}

					DC = DC + node->localDc;
				}
				step = 2;
				endWhileFlag = FLAGON;
				break;
			case 9:
				if(!strcmp(".extern", ptrField2) || !strcmp(".entry", ptrField2)){
					step = 10;
				}
				else{
					step = 12;
				}
				break;
			case 10:
				if(!strcmp(".entry", ptrField2)){
					step = 2;
					endWhileFlag = FLAGON;
				}
				else{
					step = 11;
				}
				break;
			case 11:
				if(errorFlag == FLAGOFF){
					if(!strcmp(".extern", ptrField2)){
						/*tableHead = symbolTable(ptrField3,0,EXTERNAL,EMPTY);*/
						if(firstSymbolFlag == FLAGOFF){
							tableHead = symbolTable(ptrField3,0,EXTERNAL,EMPTY,FLAGON);
							firstSymbolFlag = FLAGON;
						}
						else{
							tableHead = symbolTable(ptrField3,0,EXTERNAL,EMPTY,FLAGOFF);
						}
					}
				}
				step = 2;
				endWhileFlag = FLAGON;
				break;
			case 12:
				if(errorFlag == FLAGOFF){
					if(labelFlag == FLAGON){
						/*tableHead = symbolTable(ptrField1,IC,CODE,EMPTY);*/

						if(firstSymbolFlag == FLAGOFF){
							tableHead = symbolTable(ptrField1,IC,CODE,EMPTY,FLAGON);
							firstSymbolFlag = FLAGON;
						}
						else{
							tableHead = symbolTable(ptrField1,IC,CODE,EMPTY,FLAGOFF);
						}
					}
				}
				step = 13;
				break;
			case 13:
				/*for(i=0;i<NUMCOMMANDS;i++){
					if(!strcmp(commands[i], ptrField2)){
						commandFlag = FLAGON;
					}
				}
				if(commandFlag == FLAGOFF){
					errorFlag = FLAGON;
				}*/
				step = 14;
				break;
			case 14:
				step = 15;
				break;
			case 15:
				if(errorFlag == FLAGOFF){
					/*Add to "Tmunat Hazikaron"*/
					if(firstComFlag == FLAGOFF){
						headCom = memAdd(ptrField1,ptrField2,ptrField3);
						firstComFlag = FLAGON;
					}
					else{
						/*if(firstDataFlag == FLAGOFF){
							headData = NULL;
						}*/

					/*printf("Start-Filename: %s, Field2: %s\n",filename,ptrField2);
						addNode(headCom,headData,memAdd(ptrField1,ptrField2,ptrField3));
					printf("End-Filename: %s, Field2: %s\n",filename,ptrField2);*/


						/*if(firstDataFlag == FLAGOFF){
							headData = NULL;
						}*/

				
						if(firstComNodeAddflag == FLAGOFF){
							node = memAdd(ptrField1,ptrField2,ptrField3);
							addNode(headCom,headData,node,firstDataNodeAddflag,FLAGOFF);
							firstComNodeAddflag = FLAGON;
						}
						else{
							node = memAdd(ptrField1,ptrField2,ptrField3);
							addNode(headCom,headData,node,firstDataNodeAddflag,FLAGON);

						}
					}
				}
				step = 16;
				break;
			case 16:
				IC = IC + 4;
				step = 2;
				endWhileFlag = FLAGON;
				break;
			case 17:
				if(errorFlag == FLAGON){
					endWhileFlag = FLAGON;
					/*freeLines(linesHead);
					freeTable(tableHead);
					freeMemIm(memImHead);*/
				}
				else{
					if(headCom != NULL){
						memImHead = headCom;
						concatNodes(headCom,headData);
					}
					else if(headData != NULL){
						memImHead = headData;
					}
					else{
						memImHead = NULL;
					}
					step = 18;
				}
				break;
			case 18:
				DCF = DC;
				ICF = IC;
				step = 19;
				break;
			case 19:
				tableTmp = tableHead;
				if(symbolTableInitFlag == FLAGON){
					while(1){
						/*printf("\nSymbol: %s, Value: %d, Attribute 1: %d\n",tableTmp->symbol,tableTmp->value,tableTmp->attribute[0]);*/
						if(tableTmp->next == NULL){
							if(tableTmp->attribute[0] == MYDATA){
								tableTmp->value = tableTmp->value + ICF;
							}
							break;
						}
						else{
							if(tableTmp->attribute[0] == MYDATA){
								tableTmp->value = tableTmp->value + ICF;
							}
							tableTmp = tableTmp->next;
						}
					
					}
				}
				step = 20;
				break;
			case 20:
				step = 21;
				break;
			case 21:
				endWhileFlag = FLAGON;

				secondPass(linesHead,tableHead,ICF,DCF,filename,memImHead,symbolTableInitFlag);

				/*printTable(linesHead);*/
				/*freeLines(linesHead);*/
				if(symbolTableInitFlag == FLAGON){
					freeTable(tableHead);
				}
				/*freeHeadData(headData);*/

				freeMemIm(memImHead);

	
				
		}
		if(endWhileFlag == FLAGON){
			break;
		}

	}
			
}


LINE_FIELDS_T* storeLineFields(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int lineNumber,int firstLineFlag){
	LINE_FIELDS_T *ptrLineFields;
	LINE_FIELDS_T *tmpPtr;
	static LINE_FIELDS_T *current;
	static LINE_FIELDS_T *head;
	/*static int firstSymbolFlag = 1;*/
	static int firstSymbolFlag;
	if(firstLineFlag == FLAGON){
		firstSymbolFlag = 1;
	}
	

	tmpPtr = (LINE_FIELDS_T*)calloc(1, sizeof(LINE_FIELDS_T));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrLineFields = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	if(firstSymbolFlag == 0){
		current->next = ptrLineFields;
	}


	if(firstSymbolFlag == 1){
		head = ptrLineFields;
	}

	if(ptrField1 != NULL)
		strcpy(ptrLineFields->label,ptrField1);
	if(ptrField2 != NULL)
		strcpy(ptrLineFields->comOrDir,ptrField2);
	if(ptrField3 != NULL)
		strcpy(ptrLineFields->values,ptrField3);
	ptrLineFields->labelFlag = labelFlag;
	ptrLineFields->lineNumber = lineNumber;
	ptrLineFields->next = NULL;
	current = ptrLineFields;
	
	firstSymbolFlag = 0;

	return head;

}

void freeLines(LINE_FIELDS_T* linesPtr){
    LINE_FIELDS_T *temp;
    LINE_FIELDS_T *current;

    temp=linesPtr;
    while(1){
        if (temp==NULL){
            break;
	}
	current = temp->next;
        free(temp);
        temp=current;
    }
}

/*void freeMemIm(MEMIM* memImHead){
    MEMIM *temp;
    MEMIM *current;

    temp=memImHead;
    while(1){
        if (temp==NULL){
            break;
	}
	current = temp->next;
	if(current!=NULL){
        	free(temp);
	}
        temp=current;
    }
}*/


void freeMemIm(MEMIM* node){
    MEMIM *temp;
    MEMIM *current;
    DATA *tempData;

    current=node;
    while(1){
	temp = current;
        if (temp->next==NULL){
            break;
	}
	if(temp->p!=NULL){
		/*while(temp->p->next!=NULL){
			tempData = temp->p;
			temp->p = temp->p->next;
			free(tempData);
		}*/
		while(1){
			tempData = temp->p;
			if(temp->p->next==NULL){
				free(tempData);
				break;
			}
			temp->p = temp->p->next;
			free(tempData);
		}
	}

	current = current->next;

        free(temp);
    }
}


/*void freeLines(LINE_FIELDS_T* linesPtr){
    LINE_FIELDS_T *temp;

    while(1){
        temp=linesPtr;
        if (temp==NULL){
            break;
	}
        linesPtr=linesPtr->next;
        free(temp);
	temp = NULL;
    }
}*/

void freeTable(TABLE_NODE_T* tablePtr){
    TABLE_NODE_T *temp;/*A temp node which will be deleted from the linked list*/

    /*While the linked list is not null-continue to delete nodes from the linked-list*/
    while(1){
        temp=tablePtr;
        if (temp==NULL){
            break;
	}
        tablePtr=tablePtr->next;
        free(temp);
	temp = NULL;
    }
}
