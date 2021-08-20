/*This file contains the firstPass function which contains the firstPass algorithm. The firstPass function is called from within the manageContents function and continues to analyze the input file line by line and begins to create the symbol table and memory image, and checks for errors. The secondPass function is called if no errors were detected in the current file*/

#include "assembler.h" /*include assembler.h header file*/
#include "firstPass.h" /*include firstPass.h header file*/

static char *directives[]={".db",".dw", ".dh", ".asciz"}; /*array of assembly directives*/

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL}; /*enum of symbol table attributes*/


/*The firstPass function receives ptrField1 of type pointer to char, ptrField2 of type pointer to char, ptrField3 of type pointer to char, labelFlag of type int, errorDetected of type int, filename of type pointer to char, lineNumber of type int, and returns nothing as it is a void function. The function is called from manageContents for every line of the input file, with ptrField1 containing an optional pointer, ptrField2 containing a command or directive, ptrField3 containing the corresponding values, labelFlag symboling if a label was defined, errorDetected symboling if error was detected, filename with the name of the current file, and lineNumber with the current line number. The function begins to build the symbol table and memory image and calls the secondPass function if no errors were detected.*/
void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int errorDetected,char *filename,int lineNumber){

	enum steps {STEP1, STEP2, STEP3,  STEP4, STEP5, STEP6, STEP7, STEP8, STEP9, STEP10, STEP11, STEP12, STEP13, STEP14, STEP15, STEP16, STEP17, STEP18, STEP19, STEP20, STEP21}; /*enum of all steps of state machine of first pass algorithm*/

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
					errorFlag = FLAGON; /*turn on errorFlag if errorDetected is on*/
				}
				if(errorFlag == FLAGOFF){
					/*if errorFlag is off store the current line in linked list via the storeLineFields function and turn on lineFieldsInitFlag*/
					linesHead = storeLineFields(ptrField1,ptrField2,ptrField3,labelFlag,lineNumber,firstLineFlag);
					lineFieldsInitFlag = FLAGON;
				}

				if(labelFlag == LASTLINE){
					step = STEP17; /*if last line detected proceed to step 17*/
				}
				else{
					step = STEP6; /*if not the last line proceed to step 6*/
				}
				break;
			case STEP3:
				break; /*Step 3 from example first pass algorithm is not necessary in this program but kept in state machine to maintain same structure*/
			case STEP4:
				break; /*Step 4 from example first pass algorithm is not necessary in this program but kept in state machine to maintain same structure*/
			case STEP5:
				break; /*Step 5 from example first pass algorithm is not necessary in this program but kept in state machine to maintain same structure*/
			case STEP6:
				/*checks if the line has a second field*/
				if(strcmp(ptrField2,"-1")){
					/*checks if the line's second field is a directive*/
					for(i=STARTINDEX;i<NUMDIRECTIVES;i++){
						if(!strcmp(directives[i], ptrField2)){
							directiveFlag = FLAGON; /*turns on directive flag*/
						
						}
					}
				}
				/*checks if directive was detected*/
				if(directiveFlag == FLAGON){
					step = STEP7; /*if directive detected proceed to step 7*/
					directiveFlag = FLAGOFF; /*check if line necessary*/
				}
				else if(directiveFlag == FLAGOFF){
					step = STEP9; /*if directive not detected proceed to step 9*/
				}				
				break;
			case STEP7:
				/*checks if label detected*/
				if(labelFlag == FLAGON){
					/*checks if label is first one of file*/
					if(firstSymbolFlag == FLAGOFF){
						tableHead = symbolTable(ptrField1,DC,MYDATA,EMPTY,FLAGON); /*adds label to head of symbol table linked list*/
						firstSymbolFlag = FLAGON; /*sets firstSymbolFlag to on*/
					}
					else{
						tableTmp = tableHead; /*sets temporary table pointer to head of linked list*/
						while(tableTmp!=NULL){
							/*checks if label already defined previously*/
        						if(!strcmp(tableTmp->symbol,ptrField1)){
								errorMsg(ERRORTYPE33,lineNumber,ptrField1,filename); /*print error message*/
								errorFlag = FLAGON; /*sets errorFlag to on*/
								prevDetectLabelFlag = FLAGON; /*sets prevDetectLabelFlag to on*/
								break;
							}
							tableTmp = tableTmp->next; /*increments to next node in symbol table*/
						}
						if(prevDetectLabelFlag == FLAGOFF){
							tableHead = symbolTable(ptrField1,DC,MYDATA,EMPTY,FLAGOFF); /*if label not in linked list already add to symbol table*/
						}
					}
				}
				step = STEP8;
				break;
			case STEP8:
				/*check for error detected*/
				if(errorDetected == FLAGOFF){
					node = memAdd(ptrField1,ptrField2,ptrField3); /*check if all registers,immediates,etc. are in valid range*/
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

					/*iterates over all nodes of linked list and frees them*/
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

				/*checks if errorFlag is set to off*/
				if(errorFlag == FLAGOFF){
					/*checks if first data not detected*/
					if(firstDataFlag == FLAGOFF){
						headData = memAdd(ptrField1,ptrField2,ptrField3); /*adds first node of data of memory image*/
						node = headData; /*sets node to head of data memory image*/
						firstDataFlag = FLAGON; /*sets firstDataFlag to on*/
					}
					else{
						/*checks if after first node added of data memory image*/
						if(firstDataNodeAddflag == FLAGOFF){
							node = memAdd(ptrField1,ptrField2,ptrField3); /*creates another node of data memory image*/
							addNode(headCom,headData,node,FLAGOFF,firstComNodeAddflag); /*adds another node to data memory image*/
							firstDataNodeAddflag = FLAGON; /*sets firstDataNodeAddFlag to on*/
						}
						else{
							node = memAdd(ptrField1,ptrField2,ptrField3); /*creates another node of data memory image*/
							addNode(headCom,headData,node,FLAGON,firstComNodeAddflag); /*adds another node to data memory image*/
						}
					}

					DC = DC + node->localDc; /*adds data counter of current node to total data counter*/
				}
				step = STEP2; /*return to step 2*/
				endWhileFlag = FLAGON; /*end while of state machine to proceed to next line of file*/
				break;
			case STEP9:
				/*checks if second field of current line is entry or extern*/
				if(!strcmp(".extern", ptrField2) || !strcmp(".entry", ptrField2)){
					step = STEP10; /*proceed to step 10*/
				}
				else{
					step = STEP12; /*proceed to step 12*/
				}
				break;
			case STEP10:
				/*checks if second field of current line is entry*/
				if(!strcmp(".entry", ptrField2)){
					step = STEP2; /*return to step 2*/
					endWhileFlag = FLAGON; /*end while of state machine to proceed to next line of file*/
				}
				else{
					step = STEP11; /*proceed to step 11*/
				}
				break;
			case STEP11:
				/*checks if second field of current line is extern*/
				if(!strcmp(".extern", ptrField2)){
					/*checks if label is first one of file*/
					if(firstSymbolFlag == FLAGOFF){
						tableHead = symbolTable(ptrField3,EXTERNVALUE,EXTERNAL,EMPTY,FLAGON); /*adds label to head of symbol table linked list*/
						firstSymbolFlag = FLAGON; /*sets firstSymbolFlag to on*/
					}
					else{
						tableTmp = tableHead; /*sets temporary table pointer to head of linked list*/
						while(tableTmp!=NULL){
							/*checks if label already previously define not as external*/
        						if((!strcmp(tableTmp->symbol,ptrField3)) && (tableTmp->attribute[ATTRIBUTE1] != EXTERNAL)){
								errorMsg(ERRORTYPE32,lineNumber,ptrField3,filename); /*print error message*/
								errorFlag = FLAGON; /*set error flag to on*/
								prevDetectLabelFlag = FLAGON; /*set prevDetectLabelFlag to on*/
								break;
							}
							tableTmp = tableTmp->next; /*iterate to next node in symbol table*/
						}
						if(prevDetectLabelFlag == FLAGOFF){
							tableHead = symbolTable(ptrField3,EXTERNVALUE,EXTERNAL,EMPTY,FLAGOFF); /*if label not previously detected as external, add to symbol table*/
						}
					}
				}

				step = STEP2; /*return to step 2*/
				endWhileFlag = FLAGON; /*end while of state machine to proceed to next line of file*/
				break;
			case STEP12:
				/*checks if label detected*/
				if(labelFlag == FLAGON){
					/*checks if label is first one of file*/
					if(firstSymbolFlag == FLAGOFF){
						tableHead = symbolTable(ptrField1,IC,CODE,EMPTY,FLAGON);/*adds label to head of symbol table linked list*/
						firstSymbolFlag = FLAGON; /*sets firstSymbolFlag to on*/
					}
					else{
						tableTmp = tableHead; /*sets temporary table pointer to head of linked list*/
						while(tableTmp!=NULL){
							/*checks if label already previously*/
        						if(!strcmp(tableTmp->symbol,ptrField1)){
								errorMsg(ERRORTYPE33,lineNumber,ptrField1,filename); /*print error message*/
								errorFlag = FLAGON; /*set error flag to on*/
								prevDetectLabelFlag = FLAGON; /*set prevDetectLabelFlag to on*/
								break;
							}
							tableTmp = tableTmp->next; /*iterate to next node in symbol table*/
						}
						if(prevDetectLabelFlag == FLAGOFF){
							tableHead = symbolTable(ptrField1,IC,CODE,EMPTY,FLAGOFF); /*if label not previously detected, add to symbol table*/
						}
					}
				}

				step = STEP13; /*proceed to step 13*/
				break;
			case STEP13:
				step = STEP14; /*Proceed to step 14*/
				break;
			case STEP14:
				step = STEP15; /*Proceed to step 15*/
				break;
			case STEP15:
				/*if error not detected*/
				if(errorDetected == FLAGOFF){
					node = memAdd(ptrField1,ptrField2,ptrField3); /*check if all registers,immediates,etc. are in valid range*/
					/*check if register value in range*/
					if(node->errorFlag==ERRORFLAG1){
						errorMsg(ERRORTYPE27,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON; /*sets error flag to on*/
					}
					/*check if immediate value in range*/
					else if(node->errorFlag==ERRORFLAG2){
						errorMsg(ERRORTYPE28,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON; /*sets error flag to on*/
					}
					/*check if .db value in range*/
					else if(node->errorFlag==ERRORFLAG3){
						errorMsg(ERRORTYPE29,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON; /*sets error flag to on*/
					}
					/*check if .dw value in range*/
					else if(node->errorFlag==ERRORFLAG4){
						errorMsg(ERRORTYPE30,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON; /*sets error flag to on*/
					}
					/*check if .dh value in range*/
					else if(node->errorFlag==ERRORFLAG5){
						errorMsg(ERRORTYPE31,lineNumber,NULL,filename); /*print error message*/
						errorFlag = FLAGON; /*sets error flag to on*/
					}

					/*iterates over all nodes of linked list and frees them*/
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

				/*if error flag is off*/
				if(errorFlag == FLAGOFF){
					/*if first command not detected yet*/
					if(firstComFlag == FLAGOFF){
						headCom = memAdd(ptrField1,ptrField2,ptrField3); /*add to memory image head of commands*/
						headCom->ic = IC; /*set the current instruction counter to node*/
						firstComFlag = FLAGON; /*set firstComFlag to on*/
					}
					else{
						/*checks if first com node added*/
						if(firstComNodeAddflag == FLAGOFF){
							node = memAdd(ptrField1,ptrField2,ptrField3); /*creates com node of memory image*/
							addNode(headCom,headData,node,firstDataNodeAddflag,FLAGOFF); /*adds node to memory image*/
							firstComNodeAddflag = FLAGON; /*sets firstComNodeAddflag to on*/
						}
						else{
							node = memAdd(ptrField1,ptrField2,ptrField3); /*creates com node of memory image*/
							addNode(headCom,headData,node,firstDataNodeAddflag,FLAGON); /*adds node to memory image*/
						}
					}
				}
				step = STEP16; /*proceed to step 16*/
				break;
			case STEP16:
				IC = IC + ICINCREMENT; /*add instruction counter increment of 4 to total instruction counter*/
				step = STEP2; /*return to step 2*/
				endWhileFlag = FLAGON; /*end while of state machine to proceed to next line of file*/
				break;
			case STEP17:
				/*checks if error flag is on*/
				if(errorFlag == FLAGON){
					endWhileFlag = FLAGON; /*ends while of state machine and does not perform second pass*/

					if(lineFieldsInitFlag == FLAGON){
						freeLines(linesHead); /*frees all stored lines from file if any stored*/
					}
					if(symbolTableInitFlag == FLAGON){
						freeTable(tableHead); /*frees all nodes from symbol table if any stored*/
					}
					if(firstDataFlag == FLAGON){
						freeMemIm(headData); /*frees all data from memory image if any stored*/
					}
					if(firstComFlag == FLAGON){
						freeMemIm(headCom); /*frees all com from memory image if any stored*/
					}
				}
				else{
					if(firstComFlag == FLAGON){
						memImHead = headCom; /*sets memory image head to head of commands if command was detected*/
						if(firstDataFlag == FLAGON)
							concatNodes(headCom,headData); /*concatenates the head of com and data if both detected*/
					}
					else if(firstDataFlag == FLAGON){
						memImHead = headData; /*if only data detected set memory image head to head of data*/
					}
					else{
						memImHead = NULL; /*if com and data both not detected set memory image head to NULL*/
					}

					step = STEP18; /*proceed to step 18*/
				}
				break;
			case STEP18:
				DCF = DC; /*set final data counter to current data counter*/
				ICF = IC; /*set final instruction counter to current instruction counter*/
				step = STEP19; /*proceed to step 19*/
				break;
			case STEP19:
				tableTmp = tableHead; /*set temporary pointer to symbol table to head of table*/
				/*check if symbol table initialized*/
				if(symbolTableInitFlag == FLAGON){
					while(1){
						/*loop while end of symbol table not reached*/
						if(tableTmp->next == NULL){
							/*add ICF to data labels */
							if(tableTmp->attribute[ATTRIBUTE1] == MYDATA){
								tableTmp->value = tableTmp->value + ICF;
							}
							break;
						}
						else{
							/*add ICF to data labels */
							if(tableTmp->attribute[ATTRIBUTE1] == MYDATA){
								tableTmp->value = tableTmp->value + ICF;
							}
							tableTmp = tableTmp->next; /*iterate to next node of symbol table*/
						}
					
					}
				}
				step = STEP20; /*proceed to step 20*/
				break;
			case STEP20:
				step = STEP21; /*proceed to step 21*/
				break;
			case STEP21:
				endWhileFlag = FLAGON; /*end while of state machine to proceed to next file*/
				
				secondPass(linesHead,tableHead,ICF,DCF,filename,memImHead,symbolTableInitFlag); /*perform second pass on file*/

				freeLines(linesHead); /*free all line stored from current file*/

				if(symbolTableInitFlag == FLAGON){
					freeTable(tableHead); /*if symbol table initialized all nodes of symbol table*/
				}


				if(firstDataFlag == FLAGON && firstComFlag == FLAGON){
					freeMemIm(memImHead); /*if data and com initialized, free all nodes of memory image*/
				}
				else if(firstDataFlag == FLAGON && firstComFlag == FLAGOFF){
					freeMemIm(headData); /*if only data initialized, free all data nodes of memory image*/
				}
				else if(firstDataFlag == FLAGOFF && firstComFlag == FLAGON){
					freeMemIm(headCom); /*if only com initialized, free all com nodes of memory image*/
				}
		}
		if(endWhileFlag == FLAGON){
			break; /*break out of while loop of state machine of first pass algorithm to next line of file (or to end of file)*/
		}

	}
			
}

/*The storeLineFields function receives ptrField1 of type pointer to char, ptrField2 of type pointer to char, and ptrField3 of type pointer to char, labelFlag of type int, lineNumber of type int, firstLineFlag of type int, and returns the head of the line fields of type pointer to LINE_FIELDS_T. The function uses the firstLineFlag to create the head of the linked list when the flag is on and adds nodes when the flag is off. In each line field the three fields from ptrField1, ptrField2, and ptrField3 are stored along with the labelFlag, and lineNumber, so all the lines that were iterated through in the first pass can then be iterated through again in the second pass and then freed.*/
LINE_FIELDS_T* storeLineFields(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int lineNumber,int firstLineFlag){
	LINE_FIELDS_T *ptrLineFields; /*pointer to line fields*/
	LINE_FIELDS_T *tmpPtr; /*temporary pointer to line fields*/
	static LINE_FIELDS_T *current; /*current pointer to line fields*/
	static LINE_FIELDS_T *head; /*head of line fields*/
	static int firstSymbolFlag; /*declares firstSymbolFlag*/

	if(firstLineFlag == FLAGON){
		firstSymbolFlag = FLAGON; /*sets firstSymbolFlag to on if firstLineFlag is on*/
	}
	
	/*allocates memory to store line field*/
	tmpPtr = (LINE_FIELDS_T*)calloc(SINGLENODE, sizeof(LINE_FIELDS_T));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrLineFields = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	if(firstSymbolFlag == FLAGOFF){
		current->next = ptrLineFields; /*if firstSymbolFlag is off set next node to ptrLineFields*/
	}

	if(firstSymbolFlag == FLAGON){
		head = ptrLineFields; /*if firstSymbolFlag is on set head to ptrLineFields*/
	}

	if(ptrField1 != NULL)
		strcpy(ptrLineFields->label,ptrField1); /*if first field of the line is not NULL place it in the label field of the struct*/
	if(ptrField2 != NULL)
		strcpy(ptrLineFields->comOrDir,ptrField2); /*if second field of the line is not NULL place it in the comOrDir field of the struct*/
	if(ptrField3 != NULL)
		strcpy(ptrLineFields->values,ptrField3); /*if third field of the line is not NULL place it in the values field of the struct*/
	ptrLineFields->labelFlag = labelFlag; /*place the current labelFlag in the labelFlag field of the struct*/
	ptrLineFields->lineNumber = lineNumber; /*place the current line number in the lineNumber field of the struct*/
	ptrLineFields->next = NULL; /*set the pointer to the next node of the struct to NULL*/
	current = ptrLineFields; /*set current node to ptrLineFields*/
	
	firstSymbolFlag = FLAGOFF; /*set firstSymbolFlag to off*/

	return head; /*return head of line fields*/

}


/*The freeLines function receives linesPtr of type LINE_FIELDS_T* and returns nothing as it is a void function. The function iterates through the line fields and frees every node from the line fields until the end of the line fields is reached.*/
void freeLines(LINE_FIELDS_T* linesPtr){
    LINE_FIELDS_T *temp; /*temporary pointer to LINE_FIELDS_T*/
    LINE_FIELDS_T *current; /*current pointer to LINE_FIELDS_T*/

    current=linesPtr; /*sets current pointer to linesPtr*/
    /*While the linked list is not null-continue to free nodes from the linked-list*/
    while(1){
        temp=current; /*set temp to current*/

        current=current->next; /*set current to next node*/
	
        free(temp); /*free temp node*/
	if (current==NULL){
            break; /*break from while if end of linked list reached*/
	}
    }
}


/*The freeMemIm function receives node of type MEMIM* and returns nothing as it is a void function. The function iterates through the memory image and frees every node from the memory image until the end of the memory image is reached. For each node the function also frees the linked list within the p field of the struct.*/
void freeMemIm(MEMIM* node){
    MEMIM *temp; /*temporary pointer to MEMIM*/
    MEMIM *current; /*current pointer to MEMIM*/
    DATA *tempData; /*temporary pointer to DATA*/
    DATA *currentData; /*current pointer to DATA*/

    current=node; /*sets current pointer to node*/
    /*While the linked list is not null-continue to free nodes from the linked-list*/
    while(1){
	temp = current; /*set temp to current*/
	
	/*check if p field of temp is not null*/
	if(temp->p!=NULL){
		currentData = temp->p; /*set currentData to temp->p*/
    		/*While the linked list is not null-continue to free nodes from the linked-list*/
		while(1){
			tempData = currentData; /*set tempData to currentData*/
			currentData = currentData->next; /*set currentData to next node*/
			free(tempData); /*free tempData*/
			if(currentData == NULL){
				break; /*break from while if end of linked list reached*/
			}
		}
	}

	current = current->next; /*set current node to next node*/

        free(temp); /*free temp node*/
	
	if (current==NULL)
		break; /*break from while if end of linked list reached*/
    }
}


/*The freeTable function receives tablePtr of type TABLE_NODE_T* and returns nothing as it is a void function. The function iterates through the symbol table and frees every node from the table until the end of the table is reached*/
void freeTable(TABLE_NODE_T* tablePtr){
    TABLE_NODE_T *temp;/*A temp node which will be deleted from the linked list*/
    TABLE_NODE_T *current;/*A temp node which will be deleted from the linked list*/

    current = tablePtr; /*set current node to tablePtr*/
    /*While the linked list is not null-continue to free nodes from the linked-list*/
    while(1){
        temp=current; /*set temp to current*/

        current=current->next; /*set current node to next node*/
	
        free(temp); /*free temp node*/
	if (current==NULL){
            break; /*break from while if end of linked list reached*/
	}
    }
}







