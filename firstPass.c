#include "main.h"

enum status {PREFW,FIRSTWORD,POSTFW};

static char *directives[]={".db",".dw", ".dh", ".asciz"};

static char *commands[]={"add","sub", "and", "or", "nor", "move", "mvhi","mvlo", "addi", "subi", "andi", "ori","nori", "bne", "beq", "blt", "bgt","lb", "sb", "lw", "sw", "lh","sh", "jmp", "la", "call", "stop"};


void firstPass(NODE_T *ptrNode){
	int index,firstCharacterFlag,firstWordIndex,labelFlag,i;
	NODE_T *current;
	char *ptrFirstWord;

	enum status state;

	index = 0;
	current = ptrNode;
	firstCharacterFlag = FLAGOFF;
	state = PREFW;
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
				firstCharacterFlag = FLAGOFF;
				index = 0;

			}
		}
		if(current->inputChar[index] == '\t' || current->inputChar[index] == ' '){
			/*printf("entered white space\n");*/
			if(state == FIRSTWORD){
				state = POSTFW;
			}
			else{
				index++;
				continue;
			}
		}
		if((current->inputChar[index] == ';' && firstCharacterFlag == FLAGOFF) || current->inputChar[index] == '\n'){
			/*printf("entered comment\n");*/
			current = current->next;
			firstCharacterFlag = FLAGOFF;
			index = 0;
			continue;
		}
		if(state == FIRSTWORD && current->inputChar[index] == ':'){
			state = POSTFW;
			labelFlag = FLAGON;
		}
		firstCharacterFlag = FLAGON;
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
				printf("%s \n",ptrFirstWord);
				for(i=0;i<NUMDIRECTIVES;i++){
					if(!strcmp(directives[i], ptrFirstWord)){
						printf("Directive detected \n");
					}
				}
				if(!strcmp(".extern", ptrFirstWord)){
					printf("Extern detected \n");
				}
				if(!strcmp(".entry", ptrFirstWord)){
					printf("Entry detected \n");
				}
				for(i=0;i<NUMCOMMANDS;i++){
					if(!strcmp(commands[i], ptrFirstWord)){
						printf("Command detected \n");
					}
				}

				free(ptrFirstWord);
				state = PREFW;
				current = current->next;
				firstCharacterFlag = FLAGOFF;
				index = 0;

				break;
		}		

	}


}

