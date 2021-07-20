#include "main.h"

enum status {PREFIRSTWORD,FIRSTWORD,POSTFIRSTWORD,POSTCOMMAND,POSTDIRECTIVE,POSTLABEL,POSTEXTERN,POSTENTRY,COMMANDORDIRECTIVE,DATA,CODE};

/*static enum status state;*/

static char *directives[]={".db",".dw", ".dh", ".asciz"};

static char *commands[]={"add","sub", "and", "or", "nor", "move", "mvhi","mvlo", "addi", "subi", "andi", "ori","nori", "bne", "beq", "blt", "bgt","lb", "sb", "lw", "sw", "lh","sh", "jmp", "la", "call", "stop"};


int checkState(char *ptrInput);


void manageContents(NODE_T *ptrNode){
	int index,firstWordIndex,labelFlag,labelIndex,midLabel,CommandDirectiveIndex,dataIndex,codeIndex,errorDetected,i;
	NODE_T *current;
	char *ptrFirstWord;
	char *ptrLabel;
	char *ptrCommandDirective;
	char *ptrData;
	char *ptrCode;
	char *ptrCodeChecked;
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
			if(state == CODE){
				ptrCode[codeIndex] = '\0';
				if(labelFlag == FLAGON){
					firstPass(ptrFirstWord,ptrCommandDirective,ptrCode,labelFlag,errorDetected);
				}
				else if(labelFlag == FLAGOFF){
					/*firstPass(ptrFirstWord,ptrCode,ptrTrash,labelFlag);*/
					firstPass(ptrTrash,ptrFirstWord,ptrCode,labelFlag,errorDetected);
				}
				/*printf("%s\n",ptrCode);
				exit(0);*/
			}
			if(state == DATA){
				ptrData[dataIndex] = '\0';
				if(labelFlag == FLAGON){
					firstPass(ptrFirstWord,ptrCommandDirective,ptrData,labelFlag,errorDetected);
				}
				else if(labelFlag == FLAGOFF){
					/*firstPass(ptrFirstWord,ptrData,ptrTrash,labelFlag);*/
					firstPass(ptrTrash,ptrFirstWord,ptrData,labelFlag,errorDetected);				
				}

				/*exit(0);*/
			}
			if(current->next == NULL){
				firstPass(ptrTrash,ptrTrash,ptrTrash,LASTLINE,errorDetected); /*end of file*/	
				break;
			}
			else{
				state = newLine(&errorDetected,&current,&labelFlag,&index);
			}
		}
		if(current->inputChar[index] == '\t' || current->inputChar[index] == ' '){
			if(state == FIRSTWORD){
				state = POSTFIRSTWORD;
			}
			if(state == COMMANDORDIRECTIVE){
				ptrCommandDirective[CommandDirectiveIndex] = '\0';
				state = checkState(ptrCommandDirective);
				if(!strcmp("stop", ptrCommandDirective)){
					errorDetected = checkExtraneousChars(&current,&index);
					if(errorDetected == FLAGON){
						errorMsg(4,current->lineNumber,NULL);
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);						
					}
					else if(errorDetected == FLAGOFF){
						firstPass(ptrFirstWord,ptrCommandDirective,ptrTrash,labelFlag,errorDetected);
					}
					state = newLine(&errorDetected,&current,&labelFlag,&index);
				}
			}
			else if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0';
				midLabel = FLAGOFF;
				errorDetected = checkExtraneousChars(&current,&index);
				if(errorDetected == FLAGON){
					errorMsg(4,current->lineNumber,NULL);
					firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);						
				}
				else if(errorDetected == FLAGOFF){
					firstPass(ptrTrash,ptrFirstWord,ptrLabel,labelFlag,errorDetected);
				}
				/*firstPass(ptrTrash,ptrFirstWord,ptrLabel,labelFlag,errorDetected);*/
				state = newLine(&errorDetected,&current,&labelFlag,&index);
				continue;
			} 
			else if(state == CODE){
				ptrCode[codeIndex] = current->inputChar[index];
				codeIndex++;
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
			if(state == CODE){
				ptrCode[codeIndex] = '\0';
				if(labelFlag == FLAGON){
					ptrCodeChecked = checkCommand(ptrCode,ptrCommandDirective,current->lineNumber);
					/*if(ptrCodeChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected);
					}*/
					firstPass(ptrFirstWord,ptrCommandDirective,ptrCode,labelFlag,errorDetected);
				}
				else if(labelFlag == FLAGOFF){
					ptrCodeChecked = checkCommand(ptrCode,ptrFirstWord,current->lineNumber);
					/*if(ptrCodeChecked == NULL){
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected);
					}*/
					firstPass(ptrTrash,ptrFirstWord,ptrCode,labelFlag,errorDetected);
				}
				/*if(ptrCodeChecked == NULL)
					printf("\nNull mf\n");
				else
					printf("\n|%s|\n",ptrCodeChecked);*/
			}
			if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0';
				midLabel = FLAGOFF;
				/*firstPass(ptrFirstWord,ptrLabel,ptrTrash,labelFlag);*/
				firstPass(ptrTrash,ptrFirstWord,ptrLabel,labelFlag,errorDetected);
			} 
			if(state == DATA){
				ptrData[dataIndex] = '\0';
				if(labelFlag == FLAGON){
					firstPass(ptrFirstWord,ptrCommandDirective,ptrData,labelFlag,errorDetected);
				}
				else if(labelFlag == FLAGOFF){
					/*firstPass(ptrFirstWord,ptrData,ptrTrash,labelFlag);*/
					firstPass(ptrTrash,ptrFirstWord,ptrData,labelFlag,errorDetected);
				}
			}
			if(state == COMMANDORDIRECTIVE){
				ptrCommandDirective[CommandDirectiveIndex] = '\0';
				if(!strcmp("stop", ptrCommandDirective)){
					firstPass(ptrFirstWord,ptrCommandDirective,ptrTrash,labelFlag,errorDetected);
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
					if(checkState(ptrFirstWord) != -1){
						errorMsg(3,current->lineNumber,ptrFirstWord);
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);
						state = newLine(&errorDetected,&current,&labelFlag,&index);
					}
					else if(strlen(ptrFirstWord) >= MAXLABELLEN){
						errorMsg(0,current->lineNumber,ptrFirstWord);
						/*printf("Line %u: Unrecognized command or directive %s (or label with missing :)\n",current->lineNumber,ptrFirstWord);*/
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);
						state = newLine(&errorDetected,&current,&labelFlag,&index);
					}
					else if(!((ptrFirstWord[0] >= 'a' && ptrFirstWord[0] <= 'z') || (ptrFirstWord[0] >= 'A' && ptrFirstWord[0] <= 'Z'))){
						errorMsg(1,current->lineNumber,ptrFirstWord);
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);
						state = newLine(&errorDetected,&current,&labelFlag,&index);
					}
					else{
						for(i=1;ptrFirstWord[i] != '\0';i++){
							if(!((ptrFirstWord[i] >= 'a' && ptrFirstWord[i] <= 'z') || (ptrFirstWord[i] >= 'A' && ptrFirstWord[i] <= 'Z') || (ptrFirstWord[i] >= '0' && ptrFirstWord[i] <= '9'))){
								errorMsg(2,current->lineNumber,ptrFirstWord);
								errorDetected = FLAGON;
								firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);
								state = newLine(&errorDetected,&current,&labelFlag,&index);
								break;
							}
						}
					}
				}
				else{
					state = checkState(ptrFirstWord);
					if(state == -1){
						printf("Line %u: Unrecognized command or directive %s (or label with missing :)\n",current->lineNumber,ptrFirstWord);
						errorDetected = FLAGON;
						firstPass(ptrTrash,ptrTrash,ptrTrash,labelFlag,errorDetected);
						state = newLine(&errorDetected,&current,&labelFlag,&index);
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
				state = DATA;
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
			case DATA:
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





