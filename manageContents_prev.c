#include "assembler.h"

enum status {PREFIRSTWORD,FIRSTWORD,POSTFIRSTWORD,POSTCOMMAND,POSTDIRECTIVE,POSTLABEL,POSTEXTERN,POSTENTRY,COMMANDORDIRECTIVE,MYDATA,CODE};

/*static enum status state;*/

static char *directives[]={".db",".dw", ".dh", ".asciz"};

static char *commands[]={"add","sub", "and", "or", "nor", "move", "mvhi","mvlo", "addi", "subi", "andi", "ori","nori", "bne", "beq", "blt", "bgt","lb", "sb", "lw", "sw", "lh","sh", "jmp", "la", "call", "stop"};


int checkState(char *ptrInput);


void manageContents(NODE_T *ptrNode, char *filename){
	int index,firstWordIndex,labelFlag,labelIndex,midLabel,CommandDirectiveIndex,dataIndex,codeIndex,errorDetected,i,breakFlag;
	NODE_T *current;
	char *ptrFirstWord;
	char *ptrLabel;
	char *ptrCommandDirective;
	char *ptrData;
	char *ptrCode;
	char *ptrCodeChecked;
	char *ptrDataChecked;
	char ptrTrash[1];

	enum status state;

	index = 0;
	ptrTrash[0] = '\0';
	current = ptrNode;
	labelFlag = FLAGOFF;
	midLabel = FLAGOFF;
	state = PREFIRSTWORD;
	errorDetected = FLAGOFF;

	while(1){
		if(current->inputChar[index] == '\0'){
			if(state == FIRSTWORD){
				errorMsg(22,current->lineNumber,NULL);
				errorDetected = FLAGON;
				firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
				state = newLine(&errorDetected,&current,&labelFlag,&index);
				continue;
			}
			if(state == POSTFIRSTWORD){
				state = checkState(ptrFirstWord);
				if(state == -1){
					errorMsg(21,current->lineNumber,ptrFirstWord);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					continue;
				}
				else{
					errorMsg(22,current->lineNumber,NULL);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					continue;
				}
			}
			
			if(state == COMMANDORDIRECTIVE){
				ptrCommandDirective[CommandDirectiveIndex] = '\0';
				if(!strcmp("stop", ptrCommandDirective)){
					firstPass(ptrFirstWord,ptrCommandDirective,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					/*free(ptrFirstWord);*/
					free(ptrCommandDirective);
				}
				else if(state != -1){
					errorMsg(22,current->lineNumber,ptrCommandDirective);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
				}
				else{
					errorMsg(20,current->lineNumber,ptrCommandDirective);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
				}

				state = newLine(&errorDetected,&current,&labelFlag,&index);
				continue;
			}
			if(state == CODE){
				ptrCode[codeIndex] = '\0';
				if(labelFlag == FLAGON){
					ptrCodeChecked = checkCommand(ptrCode,ptrCommandDirective,current->lineNumber);
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);
						/*free(ptrFirstWord);*/
						free(ptrCommandDirective);
						free(ptrCodeChecked);
						free(ptrCode);
					}
					/*firstPass(ptrFirstWord,ptrCommandDirective,ptrCode,labelFlag,errorDetected,filename,current->lineNumber);*/
				}
				else if(labelFlag == FLAGOFF){
					ptrCodeChecked = checkCommand(ptrCode,ptrFirstWord,current->lineNumber);
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(ptrTrash,ptrFirstWord,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);
						/*free(ptrFirstWord);*/
						free(ptrCodeChecked);
						free(ptrCode);
					}
					/*firstPass(ptrTrash,ptrFirstWord,ptrCode,labelFlag,errorDetected,filename,current->lineNumber);*/
				}
			}
			if(state == MYDATA){
				ptrData[dataIndex] = '\0';
				if(labelFlag == FLAGON){
					ptrDataChecked = checkData(ptrData,ptrCommandDirective,current->lineNumber);
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);
						/*free(ptrFirstWord);*/
						free(ptrCommandDirective);
						free(ptrDataChecked);
						free(ptrData);
					}
				}
				else if(labelFlag == FLAGOFF){
					ptrDataChecked = checkData(ptrData,ptrFirstWord,current->lineNumber);
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					}
					else{
						firstPass(ptrTrash,ptrFirstWord,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);
						/*free(ptrFirstWord);*/
						free(ptrDataChecked);
						free(ptrData);
					}

				}
			}
			if(current->next == NULL){
				firstPass(ptrTrash,ptrTrash,ptrTrash,LASTLINE,errorDetected,filename,current->lineNumber); /*end of file*/	
				break;
			}
			else{
				state = newLine(&errorDetected,&current,&labelFlag,&index);
			}
		}
		if((index >= MAXLINELEN) && (current->inputChar[index] != '\n')){
			errorMsg(23,current->lineNumber,ptrCommandDirective);
			errorDetected = FLAGON;
			firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
			state = newLine(&errorDetected,&current,&labelFlag,&index);
		}
		if(current->inputChar[index] == '\t' || current->inputChar[index] == ' '){
			if(state == FIRSTWORD){
				state = POSTFIRSTWORD;
			}
			if(state == COMMANDORDIRECTIVE){
				ptrCommandDirective[CommandDirectiveIndex] = '\0';
				state = checkState(ptrCommandDirective);
				if(state == -1){
					errorMsg(20,current->lineNumber,ptrCommandDirective);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					free(ptrCommandDirective);
					continue;
				}
				if(!strcmp("stop", ptrCommandDirective)){
					errorDetected = checkExtraneousChars(&current,&index);
					if(errorDetected == FLAGON){
						errorMsg(4,current->lineNumber,NULL);
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);						
					}
					else if(errorDetected == FLAGOFF){
						firstPass(ptrFirstWord,ptrCommandDirective,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						/*free(ptrFirstWord);*/
					}
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					free(ptrCommandDirective);
					continue;
				}
			}
			else if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0';
				midLabel = FLAGOFF;
				if((checkState(ptrLabel) != -1) || !strcmp("dh", ptrLabel) || !strcmp("dw", ptrLabel) || !strcmp("db", ptrLabel) || !strcmp("asciz", ptrLabel)){
					errorMsg(3,current->lineNumber,ptrLabel);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					/*free(ptrLabel);*/
					continue;
				}
				else if(strlen(ptrLabel) >= MAXLABELLEN){
					errorMsg(0,current->lineNumber,ptrLabel);
					/*printf("Line %u: Unrecognized command or directive %s (or label with missing :)\n",current->lineNumber,ptrFirstWord);*/
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					/*free(ptrLabel);*/
					continue;
				}
				else if(!((ptrLabel[0] >= 'a' && ptrLabel[0] <= 'z') || (ptrLabel[0] >= 'A' && ptrLabel[0] <= 'Z'))){
					errorMsg(1,current->lineNumber,ptrLabel);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					/*free(ptrLabel);*/
					continue;
				}
				else{
					breakFlag = FLAGOFF;
					for(i=1;ptrLabel[i] != '\0';i++){
						if(!((ptrLabel[i] >= 'a' && ptrLabel[i] <= 'z') || (ptrLabel[i] >= 'A' && ptrLabel[i] <= 'Z') || (ptrLabel[i] >= '0' && ptrLabel[i] <= '9'))){
							errorMsg(2,current->lineNumber,ptrLabel);
							errorDetected = FLAGON;
							firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
							state = newLine(&errorDetected,&current,&labelFlag,&index);
							breakFlag = FLAGON;
							/*free(ptrLabel);*/
							break;
						}
					}
					if(breakFlag == FLAGON){
						continue;
					}
				}
				errorDetected = checkExtraneousChars(&current,&index);
				if(errorDetected == FLAGON){
					errorMsg(4,current->lineNumber,NULL);
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);						
				}
				else if(errorDetected == FLAGOFF){
					firstPass(ptrTrash,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);
				}
				/*firstPass(ptrTrash,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);*/
				state = newLine(&errorDetected,&current,&labelFlag,&index);
				/*free(ptrFirstWord);*/
				/*free(ptrLabel);*/
				continue;
			} 
			else if(state == CODE){
				ptrCode[codeIndex] = current->inputChar[index];
				codeIndex++;
				index++;
				continue;
			}
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
			state = newLine(&errorDetected,&current,&labelFlag,&index);
			continue;
		}
		if(current->inputChar[index] == '\n'){
			if(state == FIRSTWORD){
				errorMsg(22,current->lineNumber,NULL);
				errorDetected = FLAGON;
				firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
				state = newLine(&errorDetected,&current,&labelFlag,&index);
				continue;
			}
			if(state == POSTFIRSTWORD){
				state = checkState(ptrFirstWord);
				if(state == -1){
					errorMsg(21,current->lineNumber,ptrFirstWord);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					continue;
				}
				else{
					errorMsg(22,current->lineNumber,NULL);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					continue;
				}
			}
			if(state == CODE){
				ptrCode[codeIndex] = '\0';
				if(labelFlag == FLAGON){
					ptrCodeChecked = checkCommand(ptrCode,ptrCommandDirective,current->lineNumber);
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						free(ptrCode);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);
						/*free(ptrFirstWord);*/
						free(ptrCommandDirective);
						free(ptrCodeChecked);
						free(ptrCode);
					}
					/*firstPass(ptrFirstWord,ptrCommandDirective,ptrCode,labelFlag,errorDetected,filename,current->lineNumber);*/
				}
				else if(labelFlag == FLAGOFF){
					ptrCodeChecked = checkCommand(ptrCode,ptrFirstWord,current->lineNumber);
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						free(ptrCode);
					}
					else{
						firstPass(ptrTrash,ptrFirstWord,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);
						/*free(ptrFirstWord);*/
						free(ptrCodeChecked);
						free(ptrCode);
					}
					/*firstPass(ptrTrash,ptrFirstWord,ptrCode,labelFlag,errorDetected,filename,current->lineNumber);*/
				}
			}
			if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0';
				midLabel = FLAGOFF;
				if((checkState(ptrLabel) != -1) || !strcmp("dh", ptrLabel) || !strcmp("dw", ptrLabel) || !strcmp("db", ptrLabel) || !strcmp("asciz", ptrLabel)){
					errorMsg(3,current->lineNumber,ptrLabel);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					/*free(ptrLabel);*/
					continue;
				}
				else if(strlen(ptrLabel) >= MAXLABELLEN){
					errorMsg(0,current->lineNumber,ptrLabel);
					/*printf("Line %u: Unrecognized command or directive %s (or label with missing :)\n",current->lineNumber,ptrFirstWord);*/
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					/*free(ptrLabel);*/
					continue;
				}
				else if(!((ptrLabel[0] >= 'a' && ptrLabel[0] <= 'z') || (ptrLabel[0] >= 'A' && ptrLabel[0] <= 'Z'))){
					errorMsg(1,current->lineNumber,ptrLabel);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					state = newLine(&errorDetected,&current,&labelFlag,&index);
					/*free(ptrLabel);*/
					continue;
				}
				else{
					breakFlag = FLAGOFF;
					for(i=1;ptrLabel[i] != '\0';i++){
						if(!((ptrLabel[i] >= 'a' && ptrLabel[i] <= 'z') || (ptrLabel[i] >= 'A' && ptrLabel[i] <= 'Z') || (ptrLabel[i] >= '0' && ptrLabel[i] <= '9'))){
							errorMsg(2,current->lineNumber,ptrLabel);
							errorDetected = FLAGON;
							firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
							state = newLine(&errorDetected,&current,&labelFlag,&index);
							breakFlag = FLAGON;
							break;
						}
					}
					if(breakFlag == FLAGON){
						continue;
					}
				}
				/*firstPass(ptrFirstWord,ptrLabel,ptrTrash,labelFlag);*/
				firstPass(ptrTrash,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);
				/*free(ptrFirstWord);*/
				/*free(ptrLabel);*/
			} 
			if(state == MYDATA){
				ptrData[dataIndex] = '\0';
				if(labelFlag == FLAGON){
					ptrDataChecked = checkData(ptrData,ptrCommandDirective,current->lineNumber);
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						free(ptrCommandDirective);
						free(ptrDataChecked);
						free(ptrData);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);
						free(ptrCommandDirective);
						free(ptrDataChecked);
						free(ptrData);
					}
				}
				else if(labelFlag == FLAGOFF){
					ptrDataChecked = checkData(ptrData,ptrFirstWord,current->lineNumber);
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						free(ptrData);
					}
					else{
						firstPass(ptrTrash,ptrFirstWord,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);
						/*free(ptrFirstWord);*/
						free(ptrDataChecked);
						free(ptrData);
					}

				}
			}
			if(state == COMMANDORDIRECTIVE){
				ptrCommandDirective[CommandDirectiveIndex] = '\0';
				if(!strcmp("stop", ptrCommandDirective)){
					firstPass(ptrFirstWord,ptrCommandDirective,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					/*free(ptrFirstWord);*/
					free(ptrCommandDirective);
				}
				else if(state != -1){
					errorMsg(22,current->lineNumber,ptrCommandDirective);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					free(ptrCommandDirective);
				}
				else{
					errorMsg(20,current->lineNumber,ptrCommandDirective);
					errorDetected = FLAGON;
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
					free(ptrCommandDirective);
				}
			}
			state = newLine(&errorDetected,&current,&labelFlag,&index);
			continue;
		}
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
				ptrFirstWord[firstWordIndex] = '\0';
				/*printf("%s\n",ptrFirstWord);
				exit(0);*/
				if(labelFlag == FLAGON){
					state = POSTLABEL;
					if((checkState(ptrFirstWord) != -1) || !strcmp("dh", ptrFirstWord) || !strcmp("dw", ptrFirstWord) || !strcmp("db", ptrFirstWord) || !strcmp("asciz", ptrFirstWord)){
						errorMsg(3,current->lineNumber,ptrFirstWord);
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						state = newLine(&errorDetected,&current,&labelFlag,&index);
						/*free(ptrFirstWord);*/
					}
					else if(strlen(ptrFirstWord) >= MAXLABELLEN){
						errorMsg(0,current->lineNumber,ptrFirstWord);
						/*printf("Line %u: Unrecognized command or directive %s (or label with missing :)\n",current->lineNumber,ptrFirstWord);*/
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						state = newLine(&errorDetected,&current,&labelFlag,&index);
						/*free(ptrFirstWord);*/
					}
					else if(!((ptrFirstWord[0] >= 'a' && ptrFirstWord[0] <= 'z') || (ptrFirstWord[0] >= 'A' && ptrFirstWord[0] <= 'Z'))){
						errorMsg(1,current->lineNumber,ptrFirstWord);
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						state = newLine(&errorDetected,&current,&labelFlag,&index);
						/*free(ptrFirstWord);*/
					}
					else{
						for(i=1;ptrFirstWord[i] != '\0';i++){
							if(!((ptrFirstWord[i] >= 'a' && ptrFirstWord[i] <= 'z') || (ptrFirstWord[i] >= 'A' && ptrFirstWord[i] <= 'Z') || (ptrFirstWord[i] >= '0' && ptrFirstWord[i] <= '9'))){
								errorMsg(2,current->lineNumber,ptrFirstWord);
								errorDetected = FLAGON;
								firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
								state = newLine(&errorDetected,&current,&labelFlag,&index);
								/*free(ptrFirstWord);*/
								break;
							}
						}
					}
				}
				else{
					state = checkState(ptrFirstWord);
					if(state == -1){
						errorMsg(21,current->lineNumber,ptrFirstWord);
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected,filename,current->lineNumber);
						state = newLine(&errorDetected,&current,&labelFlag,&index);
						/*free(ptrFirstWord);*/
						break;
						/*exit(0);*/
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


void errorMsg(int error,int lineNumber,char *fieldName)
{
	switch (error)
	{
		case 0:
			printf("Line %u: Label length of %s greater than maximum label length of 31\n",lineNumber,fieldName);
			break;
		case 1:
			printf("Line %u: Label %s must start with alphabetic letter\n",lineNumber,fieldName);
			break;
		case 2:
			printf("Line %u: Label %s must be composed of only alphabetic letters or numbers\n",lineNumber,fieldName);
			break;
		case 3:
			printf("Line %u: Label %s must not have the same name as a saved Assembly word\n",lineNumber,fieldName);
			break;
		case 4:
			printf("Line %u: Extraneous character(s) at end of line\n",lineNumber);
			break;
		case 5:
			printf("Line %u: Expected $ sign\n",lineNumber);
			break;
		case 6:
			printf("Line %u: Illegal consecutive commas\n",lineNumber);
			break;
		case 7:
			printf("Line %u: Expected number after $ sign\n",lineNumber);
			break;
		case 8:
			printf("Line %u: Unexpected comma\n",lineNumber);
			break;
		case 9:
			printf("Line %u: Extraneous character(s) at end of line\n",lineNumber);
			break;
		case 10:
			printf("Line %u: Unexpected character\n",lineNumber);
			break;
		case 11:
			printf("Line %u: Missing comma\n",lineNumber);
			break;
		case 12:
			printf("Line %u: Missing register(s)\n",lineNumber);
			break;
		case 13:
			printf("Line %u: Missing label\n",lineNumber);
			break;
		case 14:
			printf("Line %u: Expected register or (legal) label after jmp command\n",lineNumber);
			break;
		case 15:
			printf("Line %u: Expected (legal) label after la or call command\n",lineNumber);
			break;
		case 16:
			printf("Line %u: Expected quotation marks after .asciz directive\n",lineNumber);
			break;
		case 17:
			printf("Line %u: Expected addtional quotation marks to close initial quotation marks\n",lineNumber);
			break;
		case 18:
			printf("Line %u: Expected number or + or - after .db/.dw/.dh directive\n",lineNumber);
			break;
		case 19:
			printf("Line %u: Expected number after +/- sign\n",lineNumber);
			break;
		case 20:
			printf("Line %u: Unrecognized command or directive \"%s\"\n",lineNumber,fieldName);
			break;
		case 21:
			printf("Line %u: Unrecognized command or directive \"%s\" (or label with missing :)\n",lineNumber,fieldName);
			break;
		case 22:
			printf("Line %u: Line incomplete\n",lineNumber);
			break;
		case 23:
			printf("Line %u: Line exceeded maximum length of 80 characters\n",lineNumber);
			break;
		case 24:
			printf("Line %u: Label \"%s\" not found in symbol table\n",lineNumber,fieldName);
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





