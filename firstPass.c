#include "assembler.h"

static char *directives[]={".db",".dw", ".dh", ".asciz"}; /*array of assembly directives*/

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL}; /*enum of symbol table attributes*/

enum steps {STEP1, STEP2, STEP3,  STEP4, STEP5, STEP6, STEP7, STEP8, STEP9, STEP10, STEP11, STEP12, STEP13, STEP14, STEP15, STEP16, STEP17, STEP18, STEP19, STEP20, STEP21};

void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int errorDetected,char *filename,int lineNumber){
	static int IC,DC; /*instruction and data counter*/
	int i,directiveFlag,endWhileFlag,DCF,ICF;
	static TABLE_NODE_T* tableHead; /*pointer to head of symbol table*/
	static LINE_FIELDS_T* linesHead; /*pointer to head of lines linked list*/
	TABLE_NODE_T* tableTmp; /*temporary pointer to symbol table*/
	static int step = STEP1; /*set initial step of state machine of first pass algorithm to 1*/
	static MEMIM* headCom; /*pointer to head of memory image commands*/
	static MEMIM* headData; /*pointer to head of memory image data*/
	static char fileNamePrev[MAXFILENAMELEN]; /*stores name of previous filename analyzed*/
	MEMIM* memImHead; /*pointer to head of memory image*/
	MEMIM* node; /*pointer to node of memory image*/
	static int firstDataFlag = FLAGOFF; /*set firstDataFlag to off for alerting when first data detected*/
	static int firstComFlag = FLAGOFF; /*set firstComFlag to off for alerting when first command detected*/
	static int errorFlag = FLAGOFF; /*set errorFlag to off for alerting when error detcted*/
	static int firstLineFlag; /*declare firstLineFlag for alerting when first line detected*/
	static int firstSymbolFlag; /*declare firstSymbolFlag for alerting when first symbol (label) detected*/
	static int firstComNodeAddflag; /*declare firstComNodeAddflag for alerting when first command node added to memory image*/
	static int firstDataNodeAddflag; /*declare firstDataNodeAddflag for alerting when first data node added to memory image*/
	static int symbolTableInitFlag; /*declare symbolTableInitFlag for alerting when symbol table initialized*/
	static int lineFieldsInitFlag; /*declare lineFieldsInitFlag for alerting when line fields initialized*/
	int prevDetectLabelFlag = FLAGOFF; /*set prevDetectLabelFlag to off*/
	DATA* tempData; /*temp pointer to data linked list*/
	DATA* currentData; /*current pointer of data linked list*/
	directiveFlag = FLAGOFF; /*set directiveFlag to off*/
	endWhileFlag = FLAGOFF; /*set endWhileFlag to off*/
	firstLineFlag = FLAGOFF; /*set firstLineFlag to off*/

	/*when new file is analyzed reset all flags to initial condition and set step of state machine of first pass algoritm to 1*/
	if(strcmp(filename,fileNamePrev)){
		step = STEP1;
		firstDataFlag = FLAGOFF;
		firstComFlag = FLAGOFF;
		errorFlag = FLAGOFF;
		firstLineFlag = FLAGON;
		firstSymbolFlag = FLAGOFF;
		firstComNodeAddflag = FLAGOFF;
		firstDataNodeAddflag = FLAGOFF;
		symbolTableInitFlag = FLAGOFF;
		lineFieldsInitFlag = FLAGOFF;
	}
	strcpy(fileNamePrev,filename); /*set current filename to fileNamePrev*/

	/*if labelFlag is on set symbolTableInitFlag to on*/
	if((labelFlag == FLAGON)){
		symbolTableInitFlag = FLAGON;
	}

	while(1){
		/*switch case for all steps of first pass algorithm*/
		switch(step){
			case STEP1:
				DC = DCINIT; /*initialize data counter to zero*/
				IC = ICINIT; /*initialize instructions counter to 100*/
				step = STEP2;
				break;
			case STEP2:
				if(errorDetected == FLAGON){
					errorFlag = FLAGON;
					/*if(labelFlag != LASTLINE){
						endWhileFlag = FLAGON;
						break;
					}*/
				}
				if(errorFlag == FLAGOFF){
					linesHead = storeLineFields(ptrField1,ptrField2,ptrField3,labelFlag,lineNumber,firstLineFlag);
					lineFieldsInitFlag = FLAGON;
				}

				if(labelFlag == LASTLINE){
					step = STEP17;
				}
				else{
					step = STEP6;
				}
				break;
			case STEP3:
				break;
			case STEP4:
				break;
			case STEP5:
				break;
			case STEP6:
				if(strcmp(ptrField2,"-1")){
					for(i=STARTINDEX;i<NUMDIRECTIVES;i++){
						if(!strcmp(directives[i], ptrField2)){
							directiveFlag = FLAGON;
						
						}
					}
				}
				if(directiveFlag == FLAGON){
					step = STEP7;
					directiveFlag = FLAGOFF; /*check if line necessary*/
				}
				else if(directiveFlag == FLAGOFF){
					step = STEP9;
				}				
				break;
			case STEP7:
				if(labelFlag == FLAGON){
					if(firstSymbolFlag == FLAGOFF){
						tableHead = symbolTable(ptrField1,DC,MYDATA,EMPTY,FLAGON);
						firstSymbolFlag = FLAGON;
					}
					else{
						tableTmp = tableHead;
						while(tableTmp!=NULL){
        						if(!strcmp(tableTmp->symbol,ptrField1)){
								errorMsg(ERRORTYPE33,lineNumber,ptrField1,filename); /*print error message*/
								errorFlag = FLAGON;
								prevDetectLabelFlag = FLAGON;
								break;
							}
							tableTmp = tableTmp->next;
						}
						if(prevDetectLabelFlag == FLAGOFF){
							tableHead = symbolTable(ptrField1,DC,MYDATA,EMPTY,FLAGOFF);
						}
					}
				}
				step = STEP8;
				break;
			case STEP8:
				if(errorDetected == FLAGOFF){
					node = memAdd(ptrField1,ptrField2,ptrField3);
					/*check if register value in range*/
					if(node->errorFlag==ERRORFLAG1){
						errorMsg(ERRORTYPE27,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}
					/*check if immediate value in range*/
					else if(node->errorFlag==ERRORFLAG2){
						errorMsg(ERRORTYPE28,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}
					/*check if .db value in range*/
					else if(node->errorFlag==ERRORFLAG3){
						errorMsg(ERRORTYPE29,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}
					/*check if .dw value in range*/
					else if(node->errorFlag==ERRORFLAG4){
						errorMsg(ERRORTYPE30,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}
					/*check if .dh value in range*/
					else if(node->errorFlag==ERRORFLAG5){
						errorMsg(ERRORTYPE31,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}

					if(node->p!=NULL){
						currentData = node->p;
						while(1){
							tempData = currentData;
							currentData = currentData->next;
							free(tempData);
							if(currentData == NULL){
								break;
							}
						}
					}
					free(node);
				}

				if(errorFlag == FLAGOFF){
					if(firstDataFlag == FLAGOFF){
						headData = memAdd(ptrField1,ptrField2,ptrField3);
						node = headData;
						firstDataFlag = FLAGON;
					}
					else{
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
				step = STEP2;
				endWhileFlag = FLAGON;
				break;
			case STEP9:
				if(!strcmp(".extern", ptrField2) || !strcmp(".entry", ptrField2)){
					step = STEP10;
				}
				else{
					step = STEP12;
				}
				break;
			case STEP10:
				if(!strcmp(".entry", ptrField2)){
					step = STEP2;
					endWhileFlag = FLAGON;
				}
				else{
					step = STEP11;
				}
				break;
			case STEP11:

				if(!strcmp(".extern", ptrField2)){
					if(firstSymbolFlag == FLAGOFF){
						tableHead = symbolTable(ptrField3,EXTERNVALUE,EXTERNAL,EMPTY,FLAGON);
						firstSymbolFlag = FLAGON;
					}
					else{
						tableTmp = tableHead;
						while(tableTmp!=NULL){
        						if((!strcmp(tableTmp->symbol,ptrField3)) && (tableTmp->attribute[ATTRIBUTE1] != EXTERNAL)){
								errorMsg(ERRORTYPE32,lineNumber,ptrField3,filename); /*print error message*/
								errorFlag = FLAGON;
								prevDetectLabelFlag = FLAGON;
								break;
							}
							tableTmp = tableTmp->next;
						}
						if(prevDetectLabelFlag == FLAGOFF){
							tableHead = symbolTable(ptrField3,EXTERNVALUE,EXTERNAL,EMPTY,FLAGOFF);
						}
					}
				}

				step = STEP2;
				endWhileFlag = FLAGON;
				break;
			case STEP12:

				if(labelFlag == FLAGON){

					if(firstSymbolFlag == FLAGOFF){
						tableHead = symbolTable(ptrField1,IC,CODE,EMPTY,FLAGON);
						firstSymbolFlag = FLAGON;
					}
					else{
						tableTmp = tableHead;
						while(tableTmp!=NULL){
        						if(!strcmp(tableTmp->symbol,ptrField1)){
								errorMsg(ERRORTYPE33,lineNumber,ptrField1,filename); /*print error message*/
								errorFlag = FLAGON;
								prevDetectLabelFlag = FLAGON;
								break;
							}
							tableTmp = tableTmp->next;
						}
						if(prevDetectLabelFlag == FLAGOFF){
							tableHead = symbolTable(ptrField1,IC,CODE,EMPTY,FLAGOFF);
						}
					}
				}

				step = STEP13;
				break;
			case STEP13:
				step = STEP14;
				break;
			case STEP14:
				step = STEP15;
				break;
			case STEP15:
				if(errorDetected == FLAGOFF){
					node = memAdd(ptrField1,ptrField2,ptrField3);
					/*check if register value in range*/
					if(node->errorFlag==ERRORFLAG1){
						errorMsg(ERRORTYPE27,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}
					/*check if immediate value in range*/
					else if(node->errorFlag==ERRORFLAG2){
						errorMsg(ERRORTYPE28,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}
					/*check if .db value in range*/
					else if(node->errorFlag==ERRORFLAG3){
						errorMsg(ERRORTYPE29,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}
					/*check if .dw value in range*/
					else if(node->errorFlag==ERRORFLAG4){
						errorMsg(ERRORTYPE30,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}
					/*check if .dh value in range*/
					else if(node->errorFlag==ERRORFLAG5){
						errorMsg(ERRORTYPE31,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON;
					}

					if(node->p!=NULL){
						currentData = node->p;
						while(1){
							tempData = currentData;
							currentData = currentData->next;
							free(tempData);
							if(currentData == NULL){
								break;
							}
						}
					}
					free(node);
				}

				if(errorFlag == FLAGOFF){
					/*Add to "Tmunat Hazikaron"*/
					if(firstComFlag == FLAGOFF){
						headCom = memAdd(ptrField1,ptrField2,ptrField3);
						headCom->ic = IC;
						firstComFlag = FLAGON;
					}
					else{
				
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
				step = STEP16;
				break;
			case STEP16:
				IC = IC + ICINCREMENT;
				step = STEP2;
				endWhileFlag = FLAGON;
				break;
			case STEP17:
				if(errorFlag == FLAGON){
					endWhileFlag = FLAGON;

					if(lineFieldsInitFlag == FLAGON){
						freeLines(linesHead);
					}
					if(symbolTableInitFlag == FLAGON){
						freeTable(tableHead);
					}
					if(firstDataFlag == FLAGON){
						freeMemIm(headData);
					}
					if(firstComFlag == FLAGON){
						freeMemIm(headCom);
					}

				}
				else{

					if(firstComFlag == FLAGON){
						memImHead = headCom;
						if(firstDataFlag == FLAGON)
							concatNodes(headCom,headData);
					}
					else if(firstDataFlag == FLAGON){
						memImHead = headData;
					}
					else{
						memImHead = NULL;
					}

					step = STEP18;
				}
				break;
			case STEP18:
				DCF = DC;
				ICF = IC;
				step = STEP19;
				break;
			case STEP19:
				tableTmp = tableHead;
				if(symbolTableInitFlag == FLAGON){
					while(1){
						if(tableTmp->next == NULL){
							if(tableTmp->attribute[ATTRIBUTE1] == MYDATA){
								tableTmp->value = tableTmp->value + ICF;
							}
							break;
						}
						else{
							if(tableTmp->attribute[ATTRIBUTE1] == MYDATA){
								tableTmp->value = tableTmp->value + ICF;
							}
							tableTmp = tableTmp->next;
						}
					
					}
				}
				step = STEP20;
				break;
			case STEP20:
				step = STEP21;
				break;
			case STEP21:
				endWhileFlag = FLAGON;
				
				secondPass(linesHead,tableHead,ICF,DCF,filename,memImHead,symbolTableInitFlag);

				freeLines(linesHead);

				if(symbolTableInitFlag == FLAGON){
					freeTable(tableHead);
				}


				if(firstDataFlag == FLAGON && firstComFlag == FLAGON){
					freeMemIm(memImHead);
				}
				else if(firstDataFlag == FLAGON && firstComFlag == FLAGOFF){
					freeMemIm(headData);
				}
				else if(firstDataFlag == FLAGOFF && firstComFlag == FLAGON){
					freeMemIm(headCom);
				}
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
		firstSymbolFlag = FLAGON;
	}
	

	tmpPtr = (LINE_FIELDS_T*)calloc(1, sizeof(LINE_FIELDS_T));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrLineFields = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	if(firstSymbolFlag == FLAGOFF){
		current->next = ptrLineFields;
	}


	if(firstSymbolFlag == FLAGON){
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

    current=linesPtr;
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


/*void freeMemIm(MEMIM* node){
    MEMIM *temp;
    MEMIM *current;
    DATA *tempData;

    current=node;

    if(current!=NULL){
	    while(1){
		temp = current;
		if (temp->next==NULL){
		    free(temp);
		    break;
		}
		if(temp->p!=NULL){

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

		current = temp->next;

		free(temp);
	    }
    }
}*/






/*void freeMemIm(MEMIM* node){
    MEMIM *temp;
    MEMIM *current;
    DATA *tempData;

    current=node;
    while(1){
	temp = current;

	if(temp->p!=NULL){
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
	
	if (current==NULL)
		break;
    }
}*/

void freeMemIm(MEMIM* node){
    MEMIM *temp;
    MEMIM *current;
    DATA *tempData;
    DATA *currentData;

    current=node;
    while(1){
	temp = current;

	if(temp->p!=NULL){
		currentData = temp->p;
		while(1){
			tempData = currentData;
			currentData = currentData->next;
			free(tempData);
			if(currentData == NULL){
				break;
			}
		}
	}

	current = current->next;

        free(temp);
	
	if (current==NULL)
		break;
    }
}



void freeTable(TABLE_NODE_T* tablePtr){
    TABLE_NODE_T *temp;/*A temp node which will be deleted from the linked list*/
    TABLE_NODE_T *current;/*A temp node which will be deleted from the linked list*/

    current = tablePtr;
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







