#include "main.h"

static char *directives[]={".db",".dw", ".dh", ".asciz"};

/*static char *commands[]={"add","sub", "and", "or", "nor", "move", "mvhi","mvlo", "addi", "subi", "andi", "ori","nori", "bne", "beq", "blt", "bgt","lb", "sb", "lw", "sw", "lh","sh", "jmp", "la", "call", "stop"};*/

enum Attributes {EMPTY,CODE,DATA,ENTRY,EXTERNAL};

void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int errorDetected){
	static int IC,DC,step,errorFlag;
	int i,directiveFlag,endWhileFlag,DCF,ICF;
	static TABLE_NODE_T* tableHead;
	static LINE_FIELDS_T* linesHead;
	/*enum Attributes Attribute;*/
	step = 1;
	errorFlag = FLAGOFF;
	directiveFlag = FLAGOFF;
	endWhileFlag = FLAGOFF;
	/*printf("%s, %s, %s\n",ptrField1,ptrField2,ptrField3);*/

	

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
				linesHead = storeLineFields(ptrField1,ptrField2,ptrField3,labelFlag);
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
				if(labelFlag == FLAGON){
					tableHead = symbolTable(ptrField1,DC,DATA,EMPTY);
				}
				step = 8;
				break;
			case 8:
				/*Add to "Tmunat Hazikaron"*/
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
				tableHead = symbolTable(ptrField3,0,EXTERNAL,EMPTY);
				step = 2;
				endWhileFlag = FLAGON;
				break;
			case 12:
				if(labelFlag == FLAGON){
					tableHead = symbolTable(ptrField1,IC,CODE,EMPTY);
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
				}
				else{
					step = 18;
				}
				break;
			case 18:
				DCF = DC;
				ICF = IC;
				step = 19;
				break;
			case 19:
				step = 20;
				break;
			case 20:
				step = 21;
				break;
			case 21:
				endWhileFlag = FLAGON;
				/*begin second pass*/
				break;
	
				
		}
		if(endWhileFlag == FLAGON){
			break;
		}

	}
			
}


LINE_FIELDS_T* storeLineFields(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag){
	LINE_FIELDS_T *ptrLineFields; /*initialize pointer to TABLE_NODE_T, variable ptrTableNode*/
	LINE_FIELDS_T *tmpPtr; /*initialize pointer to tmpPtr of type TABLE_NODE_T*/
	static LINE_FIELDS_T *current;
	static LINE_FIELDS_T *head;
	static int firstSymbolFlag = 1;
	

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

	strcpy(ptrLineFields->label,ptrField1);
	strcpy(ptrLineFields->comOrDir,ptrField2);
	strcpy(ptrLineFields->values,ptrField3);
	ptrLineFields->labelFlag = labelFlag;
	ptrLineFields->next = NULL;

	current = ptrLineFields;
	
	firstSymbolFlag = 0;

	return head;

}

