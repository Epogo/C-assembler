#include "assembler.h"

enum status {PREFIRSTWORD,FIRSTWORD,POSTFIRSTWORD,POSTCOMMAND,POSTDIRECTIVE,POSTLABEL,POSTEXTERN,POSTENTRY,COMMANDORDIRECTIVE,MYDATA,CODE};

/*static enum status state;*/

static char *directives[]={".db",".dw", ".dh", ".asciz"};

static char *commands[]={"add","sub", "and", "or", "nor", "move", "mvhi","mvlo", "addi", "subi", "andi", "ori","nori", "bne", "beq", "blt", "bgt","lb", "sb", "lw", "sw", "lh","sh", "jmp", "la", "call", "stop"};


int checkState(char *ptrInput);


FIELD_BUFFER_T* addToFieldBuffer(char *field,int firstFieldFlag){
	static FIELD_BUFFER_T *head;
	static FIELD_BUFFER_T *current;
	FIELD_BUFFER_T *ptrNode;
	FIELD_BUFFER_T *tmpPtr;
	/*static int firstFieldFlag;*/

	tmpPtr = (FIELD_BUFFER_T*)calloc(SINGLENODE, sizeof(FIELD_BUFFER_T));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/


	if(firstFieldFlag == FLAGON){
		/*printf("entered head condition\n");*/
		head = ptrNode;
	}
	else{
		current->next = ptrNode;
	}

	ptrNode->field = field;
	ptrNode->next = NULL;

	current = ptrNode;

	return head;

}

void freeFields(FIELD_BUFFER_T *head){
	FIELD_BUFFER_T *current;
	FIELD_BUFFER_T *tmp;
	current = head;
	while(1){
		tmp = current;
		if(tmp == NULL){
			break;
		}
		current = tmp->next;
		/*tmp->field = NULL;*/
		free(tmp->field);
		free(tmp);
	}
}


void manageContents(NODE_T *ptrNode, char *filename){
	int index,firstWordIndex,labelFlag,labelIndex,midLabel,CommandDirectiveIndex,dataIndex,codeIndex,errorDetected,i,breakFlag,firstFieldFlag,labelWithExtEntFlag;
	NODE_T *current;
	char *ptrFirstWord;
	char *ptrLabel;
	char *ptrCommandDirective;
	char *ptrData;
	char *ptrCode;
	char *ptrCodeChecked;
	char *ptrDataChecked;
	FIELD_BUFFER_T *headFields;

	enum status state;

	index = STARTINDEX; /*initialize index*/
	current = ptrNode; /*set current node to head*/
	labelFlag = FLAGOFF; /*set labelFlag to off symboling no label detected yet*/
	midLabel = FLAGOFF; /*set midLabel to off symboling not in a label declaration*/
	state = PREFIRSTWORD; /*set state of state machine to PREFIRSTWORD*/
	errorDetected = FLAGOFF; /*set errorDetected to off as no errors have been detected yet*/
	firstFieldFlag = FLAGON; /*set firstFieldFlag to on symboling the first field is yet to be detected*/
	labelWithExtEntFlag = FLAGOFF; /*set labelWithExtEntFlag to off as no label before extern/entry has been detected*/


	while(1){
		/*check if end of currrent line reached*/
		if(current->inputChar[index] == '\0'){
			/*check if current state of state machine is FIRSTWORD*/
			if(state == FIRSTWORD){
				ptrFirstWord[firstWordIndex] = '\0'; /*place null terminator to complete string*/
				/*check if stop command detected*/
				if(!strcmp("stop", ptrFirstWord)){
					firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send stop command to firstPass function*/
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					errorMsg(22,current->lineNumber,NULL,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
			}
			/*check if current state of state machine is POSTFIRSTWORD*/
			if(state == POSTFIRSTWORD){
				if(!strcmp("stop", ptrFirstWord)){
					firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					state = checkState(ptrFirstWord);
					if(state == -1){
						errorMsg(21,current->lineNumber,ptrFirstWord,filename);
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
						continue;
					}
					else{
						errorMsg(22,current->lineNumber,NULL,filename);
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
						continue;
					}
				}
			}
			
			/*check if current state of state machine is COMMANDORDIRECTIVE*/
			if(state == COMMANDORDIRECTIVE){
				ptrCommandDirective[CommandDirectiveIndex] = '\0'; /*place null terminator to complete string*/
				if(!strcmp("stop", ptrCommandDirective)){
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber);
				}
				else if(state != -1){
					errorMsg(22,current->lineNumber,ptrCommandDirective,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
				}
				else{
					errorMsg(20,current->lineNumber,ptrCommandDirective,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
				}
				headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
				firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
				headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/

				state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
				continue;
			}
			/*check if current state of state machine is CODE*/
			if(state == CODE){
				ptrCode[codeIndex] = '\0'; /*place null terminator to complete string*/
				if(labelFlag == FLAGON){
					ptrCodeChecked = checkCommand(ptrCode,ptrCommandDirective,current->lineNumber,filename);
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrCodeChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrCode,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
				else if(labelFlag == FLAGOFF){
					ptrCodeChecked = checkCommand(ptrCode,ptrFirstWord,current->lineNumber,filename);
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrCodeChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCode,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
			}
			/*check if current state of state machine is MYDATA*/
			if(state == MYDATA){
				ptrData[dataIndex] = '\0'; /*place null terminator to complete string*/
				if(labelFlag == FLAGON){
					ptrDataChecked = checkData(ptrData,ptrCommandDirective,current->lineNumber,filename);
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrDataChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
				else if(labelFlag == FLAGOFF){
					ptrDataChecked = checkData(ptrData,ptrFirstWord,current->lineNumber,filename);
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrDataChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/

				}
			}
			if(current->next == NULL){
				firstPass(NULL,NULL,NULL,LASTLINE,errorDetected,filename,current->lineNumber); /*end of file*/	
				freeFields(headFields);
				break;
			}
			else{
				state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
			}
		}
		if((index >= MAXLINELEN) && (current->inputChar[index] != '\n')){
			errorMsg(23,current->lineNumber,NULL,filename);
			errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
			firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
			if(state == FIRSTWORD || state == POSTFIRSTWORD){
				headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
				firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
			}
			else if(state == COMMANDORDIRECTIVE){
				headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
				firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
				headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
			}
			else if(state == CODE){
				if(labelFlag == FLAGON){
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrCode,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
				else{
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCode,firstFieldFlag); /*add field to field buffer in order to free later*/
				}

			}
			else if(state == MYDATA){
				if(labelFlag == FLAGON){
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
				else{
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/
				}

			}
			state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
			continue;
		}
		if(current->inputChar[index] == '\t' || current->inputChar[index] == ' '){
			/*check if current state of state machine is FIRSTWORD*/
			if(state == FIRSTWORD){
				state = POSTFIRSTWORD;
			}
			/*check if current state of state machine is COMMANDORDIRECTIVE*/
			if(state == COMMANDORDIRECTIVE){
				ptrCommandDirective[CommandDirectiveIndex] = '\0'; /*place null terminator to complete string*/
				state = checkState(ptrCommandDirective);
				if(state == -1){
					errorMsg(20,current->lineNumber,ptrCommandDirective,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				if(!strcmp("stop", ptrCommandDirective)){
					errorDetected = checkExtraneousChars(&current,&index);
					if(errorDetected == FLAGON){
						errorMsg(4,current->lineNumber,NULL,filename);
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);						
					}
					else if(errorDetected == FLAGOFF){
						firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				/*check if current state of state machine is POSTEXTERN or POSTENTRY*/
				if(state == POSTEXTERN || state == POSTENTRY){
					ptrLabel = calloc(MAXLINELEN,sizeof(char));
					labelIndex = 0;
					labelWithExtEntFlag = FLAGON;
					continue;
				}
			}
			/*check if current state of state machine is POSTEXTERN or POSTENTRY and in the middle of defining label (midLabel == FLAGON)*/
			else if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0'; /*place null terminator to complete string*/
				midLabel = FLAGOFF;
				if((checkState(ptrLabel) != -1) || !strcmp("dh", ptrLabel) || !strcmp("dw", ptrLabel) || !strcmp("db", ptrLabel) || !strcmp("asciz", ptrLabel)){
					errorMsg(3,current->lineNumber,ptrLabel,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
					if(labelWithExtEntFlag == FLAGON){
						headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
						labelWithExtEntFlag = FLAGOFF;
					}
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else if(strlen(ptrLabel) >= MAXLABELLEN){
					errorMsg(0,current->lineNumber,ptrLabel,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
					if(labelWithExtEntFlag == FLAGON){
						headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
						labelWithExtEntFlag = FLAGOFF;
					}
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else if(!((ptrLabel[0] >= 'a' && ptrLabel[0] <= 'z') || (ptrLabel[0] >= 'A' && ptrLabel[0] <= 'Z'))){
					errorMsg(1,current->lineNumber,ptrLabel,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
					if(labelWithExtEntFlag == FLAGON){
						headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
						labelWithExtEntFlag = FLAGOFF;
					}
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					breakFlag = FLAGOFF;
					for(i=1;ptrLabel[i] != '\0';i++){
						if(!((ptrLabel[i] >= 'a' && ptrLabel[i] <= 'z') || (ptrLabel[i] >= 'A' && ptrLabel[i] <= 'Z') || (ptrLabel[i] >= '0' && ptrLabel[i] <= '9'))){
							errorMsg(2,current->lineNumber,ptrLabel,filename);
							errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
							firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
							headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
							firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
							headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
							if(labelWithExtEntFlag == FLAGON){
								headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
								labelWithExtEntFlag = FLAGOFF;
							}
							state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
							breakFlag = FLAGON;
							break;
						}
					}
					if(breakFlag == FLAGON){
						continue;
					}
				}
				errorDetected = checkExtraneousChars(&current,&index);
				if(errorDetected == FLAGON){
					errorMsg(4,current->lineNumber,NULL,filename);
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);						
				}
				else if(errorDetected == FLAGOFF){
					if(labelWithExtEntFlag == FLAGON){
						firstPass(ptrFirstWord,ptrCommandDirective,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);
					}
				}
				headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
				firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
				headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
				if(labelWithExtEntFlag == FLAGON){
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					labelWithExtEntFlag = FLAGOFF;
				}
				state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
				continue;
			} 
			/*check if current state of state machine is CODE*/
			else if(state == CODE){
				ptrCode[codeIndex] = current->inputChar[index];
				codeIndex++;
				index++;
				continue;
			}
			/*check if current state of state machine is MYDATA*/
			else if(state == MYDATA){
				ptrData[dataIndex] = current->inputChar[index];
				dataIndex++;
				index++;
				continue;
			}
			else{
				index++;
				continue;
			}
		}
		if(current->inputChar[index] == ';' && state == PREFIRSTWORD){
			state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
			continue;
		}
		if(current->inputChar[index] == '\n'){
			/*check if current state of state machine is FIRSTWORD*/
			if(state == FIRSTWORD){
				ptrFirstWord[firstWordIndex] = '\0'; /*place null terminator to complete string*/
				if(!strcmp("stop", ptrFirstWord)){
					firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					errorMsg(22,current->lineNumber,NULL,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
			}
			/*check if current state of state machine is POSTFIRSTWORD*/
			if(state == POSTFIRSTWORD){
				if(!strcmp("stop", ptrFirstWord)){
					firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					state = checkState(ptrFirstWord);
					if(state == -1){
						errorMsg(21,current->lineNumber,ptrFirstWord,filename);
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
						continue;
					}
					else{
						errorMsg(22,current->lineNumber,NULL,filename);
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
						continue;
					}
				}
			}
			/*check if current state of state machine is CODE*/
			if(state == CODE){
				ptrCode[codeIndex] = '\0'; /*place null terminator to complete string*/
				if(labelFlag == FLAGON){
					ptrCodeChecked = checkCommand(ptrCode,ptrCommandDirective,current->lineNumber,filename);
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrCodeChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrCode,firstFieldFlag); /*add field to field buffer in order to free later*/

				}
				else if(labelFlag == FLAGOFF){
					ptrCodeChecked = checkCommand(ptrCode,ptrFirstWord,current->lineNumber,filename);
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrCodeChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCode,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
			}
			/*check if current state of state machine is POSTEXTERN or POSTENTRY and in the middle of defining label (midLabel == FLAGON)*/
			if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0'; /*place null terminator to complete string*/
				midLabel = FLAGOFF;
				if((checkState(ptrLabel) != -1) || !strcmp("dh", ptrLabel) || !strcmp("dw", ptrLabel) || !strcmp("db", ptrLabel) || !strcmp("asciz", ptrLabel)){
					errorMsg(3,current->lineNumber,ptrLabel,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
					if(labelWithExtEntFlag == FLAGON){
						headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
						labelWithExtEntFlag = FLAGOFF;
					}
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else if(strlen(ptrLabel) >= MAXLABELLEN){
					errorMsg(0,current->lineNumber,ptrLabel,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
					if(labelWithExtEntFlag == FLAGON){
						headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
						labelWithExtEntFlag = FLAGOFF;
					}
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else if(!((ptrLabel[0] >= 'a' && ptrLabel[0] <= 'z') || (ptrLabel[0] >= 'A' && ptrLabel[0] <= 'Z'))){
					errorMsg(1,current->lineNumber,ptrLabel,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
					if(labelWithExtEntFlag == FLAGON){
						headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
						labelWithExtEntFlag = FLAGOFF;
					}
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					breakFlag = FLAGOFF;
					for(i=1;ptrLabel[i] != '\0';i++){
						if(!((ptrLabel[i] >= 'a' && ptrLabel[i] <= 'z') || (ptrLabel[i] >= 'A' && ptrLabel[i] <= 'Z') || (ptrLabel[i] >= '0' && ptrLabel[i] <= '9'))){
							errorMsg(2,current->lineNumber,ptrLabel,filename);
							errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
							firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
							state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
							headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
							firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
							headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
							if(labelWithExtEntFlag == FLAGON){
								headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
								labelWithExtEntFlag = FLAGOFF;
							}
							breakFlag = FLAGON;
							break;
						}
					}
					if(breakFlag == FLAGON){
						continue;
					}
				}
				/*firstPass(ptrFirstWord,ptrLabel,NULL,labelFlag);*/
				if(labelWithExtEntFlag == FLAGON){
					firstPass(ptrFirstWord,ptrCommandDirective,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);
				}
				else{
					firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);
				}
				headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
				firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
				headFields = addToFieldBuffer(ptrLabel,firstFieldFlag); /*add field to field buffer in order to free later*/
				if(labelWithExtEntFlag == FLAGON){
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					labelWithExtEntFlag = FLAGOFF;
				}
			} 
			/*check if current state of state machine is MYDATA*/
			if(state == MYDATA){
				ptrData[dataIndex] = '\0'; /*place null terminator to complete string*/
				if(labelFlag == FLAGON){
					ptrDataChecked = checkData(ptrData,ptrCommandDirective,current->lineNumber,filename);
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrDataChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
				else if(labelFlag == FLAGOFF){
					ptrDataChecked = checkData(ptrData,ptrFirstWord,current->lineNumber,filename);
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrDataChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF;
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/

				}
			}
			/*check if current state of state machine is COMMANDORDIRECTIVE*/
			if(state == COMMANDORDIRECTIVE){
				ptrCommandDirective[CommandDirectiveIndex] = '\0'; /*place null terminator to complete string*/
				if(!strcmp("stop", ptrCommandDirective)){
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber);
				}
				else if(state != -1){
					errorMsg(22,current->lineNumber,ptrCommandDirective,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
				}
				else{
					errorMsg(20,current->lineNumber,ptrCommandDirective,filename);
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
				}
				headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
				firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
				headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
			}
			state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
			continue;
		}
		/*check if current state of state machine is FIRSTWORD and ':' character detected*/
		if(state == FIRSTWORD && current->inputChar[index] == ':'){
			state = POSTFIRSTWORD;
			labelFlag = FLAGON;
		}




		switch(state){
			case PREFIRSTWORD:
				/*printf("entered PREFIRSTWORD\n");*/
				ptrFirstWord = calloc(MAXLINELEN,sizeof(char));
				firstWordIndex = 0;
				ptrFirstWord[firstWordIndex] = current->inputChar[index];
				firstWordIndex++;
				state = FIRSTWORD;
				index++;
				break;
			case FIRSTWORD:
				/*printf("entered FIRSTWORD\n");*/
				ptrFirstWord[firstWordIndex] = current->inputChar[index];
				firstWordIndex++;
				index++;
				break;
			case POSTFIRSTWORD:
				/*printf("entered POSTFIRSTWORD\n");*/
				ptrFirstWord[firstWordIndex] = '\0'; /*place null terminator to complete string*/
				/*printf("%s\n",ptrFirstWord);
				exit(0);*/
				if(labelFlag == FLAGON){
					state = POSTLABEL;
					if((checkState(ptrFirstWord) != -1) || !strcmp("dh", ptrFirstWord) || !strcmp("dw", ptrFirstWord) || !strcmp("db", ptrFirstWord) || !strcmp("asciz", ptrFirstWord)){
						errorMsg(3,current->lineNumber,ptrFirstWord,filename);
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					}
					else if(strlen(ptrFirstWord) >= MAXLABELLEN){
						errorMsg(0,current->lineNumber,ptrFirstWord,filename);
						/*printf("Line %u: Unrecognized command or directive %s (or label with missing :)\n",current->lineNumber,ptrFirstWord);*/
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					}
					else if(!((ptrFirstWord[0] >= 'a' && ptrFirstWord[0] <= 'z') || (ptrFirstWord[0] >= 'A' && ptrFirstWord[0] <= 'Z'))){
						errorMsg(1,current->lineNumber,ptrFirstWord,filename);
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					}
					else{
						for(i=1;ptrFirstWord[i] != '\0';i++){
							if(!((ptrFirstWord[i] >= 'a' && ptrFirstWord[i] <= 'z') || (ptrFirstWord[i] >= 'A' && ptrFirstWord[i] <= 'Z') || (ptrFirstWord[i] >= '0' && ptrFirstWord[i] <= '9'))){
								errorMsg(2,current->lineNumber,ptrFirstWord,filename);
								errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
								firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
								headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
								firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
								state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
								break;
							}
						}
					}
				}
				else{
					if(!strcmp("stop", ptrFirstWord)){
						errorDetected = checkExtraneousChars(&current,&index);
						if(errorDetected == FLAGON){
							errorMsg(4,current->lineNumber,NULL,filename);
							firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);						
						}
						else if(errorDetected == FLAGOFF){
							firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber);
						}
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
						break;
					}
					else{
						state = checkState(ptrFirstWord);
						if(state == -1){
							errorMsg(21,current->lineNumber,ptrFirstWord,filename);
							errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
							firstPass(NULL,NULL,NULL,labelFlag,errorDetected,filename,current->lineNumber);
							headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
							firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
							state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
							break;
							/*exit(0);*/
						}
					}
				}
				if(state == POSTEXTERN){
					ptrLabel = calloc(MAXLINELEN,sizeof(char));
					labelIndex = 0;
				}
				else if(state == POSTENTRY){
					ptrLabel = calloc(MAXLINELEN,sizeof(char));
					labelIndex = 0;
				}
				else if(state == POSTLABEL){
					index++;
				}
				break;
			case POSTCOMMAND:
				ptrCode = calloc(MAXLINELEN,sizeof(char));
				codeIndex = 0;
				state = CODE;			
				break;
			case POSTDIRECTIVE:
				ptrData = calloc(MAXLINELEN,sizeof(char));
				dataIndex = 0;
				state = MYDATA;
				break;
			case POSTLABEL:
				ptrCommandDirective = calloc(MAXLINELEN,sizeof(char));
				CommandDirectiveIndex = 0;
				ptrCommandDirective[CommandDirectiveIndex] = current->inputChar[index];
				CommandDirectiveIndex++;
				state = COMMANDORDIRECTIVE;
				index++;

				break;
			case POSTEXTERN:
				midLabel = FLAGON;
				ptrLabel[labelIndex] = current->inputChar[index];
				labelIndex++;
				index++;
				break;
			case POSTENTRY:
				midLabel = FLAGON;
				ptrLabel[labelIndex] = current->inputChar[index];
				labelIndex++;
				index++;
				break;
			case COMMANDORDIRECTIVE:
				ptrCommandDirective[CommandDirectiveIndex] = current->inputChar[index];
				CommandDirectiveIndex++;
				index++;
				break;
			case MYDATA:
				ptrData[dataIndex] = current->inputChar[index];
				dataIndex++;
				index++;
				break;
			case CODE:
				ptrCode[codeIndex] = current->inputChar[index];
				codeIndex++;
				index++;
				break;
		}		
	}
}



int checkState(char *ptrInput){
	int i;
	enum status state;

	for(i=0;i<NUMDIRECTIVES;i++){
		if(!strcmp(directives[i], ptrInput)){
			state = POSTDIRECTIVE;
			return state;
		}
	}
	if(!strcmp(".extern", ptrInput)){
		state = POSTEXTERN;
		return state;
	}
	if(!strcmp(".entry", ptrInput)){
		state = POSTENTRY;
		return state;
	}
	if(!strcmp("stop", ptrInput)){
		state = POSTCOMMAND;
		return state;
	}
	for(i=0;i<NUMCOMMANDS;i++){
		if(!strcmp(commands[i], ptrInput)){
			state = POSTCOMMAND;
			return state;
		}
	}
	return -1;
}


void errorMsg(int error,int lineNumber,char *fieldName,char *fileName)
{
	switch (error)
	{
		case 0:
			printf("File \"%s.as\", Line %u: Label length of %s greater than maximum label length of 31\n",fileName,lineNumber,fieldName);
			break;
		case 1:
			printf("File \"%s.as\", Line %u: Label %s must start with alphabetic letter\n",fileName,lineNumber,fieldName);
			break;
		case 2:
			printf("File \"%s.as\", Line %u: Label %s must be composed of only alphabetic letters or numbers\n",fileName,lineNumber,fieldName);
			break;
		case 3:
			printf("File \"%s.as\", Line %u: Label %s must not have the same name as a saved Assembly word\n",fileName,lineNumber,fieldName);
			break;
		case 4:
			printf("File \"%s.as\", Line %u: Extraneous character(s) at end of line\n",fileName,lineNumber);
			break;
		case 5:
			printf("File \"%s.as\", Line %u: Expected $ sign\n",fileName,lineNumber);
			break;
		case 6:
			printf("File \"%s.as\", Line %u: Illegal consecutive commas\n",fileName,lineNumber);
			break;
		case 7:
			printf("File \"%s.as\", Line %u: Expected number after $ sign\n",fileName,lineNumber);
			break;
		case 8:
			printf("File \"%s.as\", Line %u: Unexpected comma\n",fileName,lineNumber);
			break;
		case 9:
			printf("File \"%s.as\", Line %u: Extraneous character(s) at end of line\n",fileName,lineNumber);
			break;
		case 10:
			printf("File \"%s.as\", Line %u: Unexpected character\n",fileName,lineNumber);
			break;
		case 11:
			printf("File \"%s.as\", Line %u: Missing comma\n",fileName,lineNumber);
			break;
		case 12:
			printf("File \"%s.as\", Line %u: Missing register(s)\n",fileName,lineNumber);
			break;
		case 13:
			printf("File \"%s.as\", Line %u: Missing label\n",fileName,lineNumber);
			break;
		case 14:
			printf("File \"%s.as\", Line %u: Expected register or (legal) label after jmp command\n",fileName,lineNumber);
			break;
		case 15:
			printf("File \"%s.as\", Line %u: Expected (legal) label after la or call command\n",fileName,lineNumber);
			break;
		case 16:
			printf("File \"%s.as\", Line %u: Expected quotation marks after .asciz directive\n",fileName,lineNumber);
			break;
		case 17:
			printf("File \"%s.as\", Line %u: Expected addtional quotation marks to close initial quotation marks\n",fileName,lineNumber);
			break;
		case 18:
			printf("File \"%s.as\", Line %u: Expected number or + or - after .db/.dw/.dh directive\n",fileName,lineNumber);
			break;
		case 19:
			printf("File \"%s.as\", Line %u: Expected number after +/- sign\n",fileName,lineNumber);
			break;
		case 20:
			printf("File \"%s.as\", Line %u: Unrecognized command or directive \"%s\"\n",fileName,lineNumber,fieldName);
			break;
		case 21:
			printf("File \"%s.as\", Line %u: Unrecognized command or directive \"%s\" (or label with missing :)\n",fileName,lineNumber,fieldName);
			break;
		case 22:
			printf("File \"%s.as\", Line %u: Line incomplete\n",fileName,lineNumber);
			break;
		case 23:
			printf("File \"%s.as\", Line %u: Line exceeded maximum length of 80 characters\n",fileName,lineNumber);
			break;
		case 24:
			printf("File \"%s.as\", Line %u: Label \"%s\" not found in symbol table\n",fileName,lineNumber,fieldName);
			break;
		default:
			printf("Default error message!\n");
			break;
	}
}
				
int newLine(int *errorDetected,NODE_T **current,int *labelFlag,int *index){
	*errorDetected = FLAGOFF;
	*current = (*current)->next;
	*labelFlag = FLAGOFF;
	*index = 0;
	return PREFIRSTWORD;
}


int checkExtraneousChars(NODE_T **current,int *index){
	while(((*current)->inputChar[*index] != '\0') && ((*current)->inputChar[*index] != '\n')){
		if((*current)->inputChar[*index] == '\t' || (*current)->inputChar[*index] == ' '){
			(*index)++;
		}
		else{
			return FLAGON;
		}
	}		
	return FLAGOFF;
}





