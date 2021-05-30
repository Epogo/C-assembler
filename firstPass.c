#include "main.h"

enum status {PREFW,FIRSTWORD,POSTFW,POSTCOMMAND,POSTDIRECTIVE,POSTLABEL,POSTEXTERN,POSTENTRY};

static char *directives[]={".db",".dw", ".dh", ".asciz"};

static char *commands[]={"add","sub", "and", "or", "nor", "move", "mvhi","mvlo", "addi", "subi", "andi", "ori","nori", "bne", "beq", "blt", "bgt","lb", "sb", "lw", "sw", "lh","sh", "jmp", "la", "call", "stop"};


void firstPass(NODE_T *ptrNode){
	int index,firstWordIndex,labelFlag,i,labelIndex,midLabel,DC,IC;
	NODE_T *current;
	char *ptrFirstWord;
	char *ptrLabel;

	enum status state;

	index = 0;
	current = ptrNode;
	labelFlag = FLAGOFF;
	midLabel = FLAGOFF;
	state = PREFW;

	IC = 100;
	DC = 0;
	while(1){

		if(current->inputChar[index] == '\0'){
			if(current->next == NULL){
				/*printf("entered Null condition");*/
				break;
			}
			else{
				/*printf("entered 0 condition \n");*/
				/*switch(state)*/
				current = current->next;
				state = PREFW;
				labelFlag = FLAGOFF;
				index = 0;

			}
		}
		if(current->inputChar[index] == '\t' || current->inputChar[index] == ' '){
			/*printf("entered white space\n");*/
			if(state == FIRSTWORD){
				state = POSTFW;
			}
			else if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0';
				midLabel = FLAGOFF;
				printf("%s \n",ptrFirstWord);
				printf("%s \n",ptrLabel);
				exit(0);
			} 
			else{
				index++;
				continue;
			}
		}
		if(current->inputChar[index] == ';' && state == PREFW){
			/*printf("entered comment\n");*/
			current = current->next;
			labelFlag = FLAGOFF;
			index = 0;
			continue;
		}
		if(current->inputChar[index] == '\n'){
			/*printf("entered comment\n");*/
			if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0';
				midLabel = FLAGOFF;
				/*printf("%s \n",ptrFirstWord);
				printf("%s \n",ptrLabel);
				exit(0);*/
			} 
			current = current->next;
			labelFlag = FLAGOFF;
			index = 0;
			continue;
		}
		if(state == FIRSTWORD && current->inputChar[index] == ':'){
			state = POSTFW;
			labelFlag = FLAGON;
		}

		switch(state){
			case PREFW:
				/*printf("entered PREFW\n");*/
				ptrFirstWord = calloc(MAXLABELLEN,sizeof(char));
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
			case POSTFW:
				/*printf("entered POSTFW\n");*/
				ptrFirstWord[firstWordIndex] = '\0';
				/*printf("%s \n",ptrFirstWord);*/
				if(labelFlag == FLAGON){
					state = POSTLABEL;
					index++;
					break;
				}
				for(i=0;i<NUMDIRECTIVES;i++){
					if(!strcmp(directives[i], ptrFirstWord)){
						state = POSTDIRECTIVE;
						index++;
						break;
					}
				}
				if(!strcmp(".extern", ptrFirstWord)){
					state = POSTEXTERN;
					ptrLabel = calloc(MAXLABELLEN,sizeof(char));
					index++;
					break;
				}
				if(!strcmp(".entry", ptrFirstWord)){
					state = POSTENTRY;
					ptrLabel = calloc(MAXLABELLEN,sizeof(char));
					labelIndex = 0;
					index++;
					break;
				}
				for(i=0;i<NUMCOMMANDS;i++){
					if(!strcmp(commands[i], ptrFirstWord)){
						state = POSTCOMMAND;
						index++;
						break;
					}
				}
				break;
			case POSTCOMMAND:
				break;
			case POSTDIRECTIVE:
				break;
			case POSTLABEL:
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
			
		}		

	}


}

