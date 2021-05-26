#include "main.h"

enum status {PREFW,FIRSTWORD,POSTFW};

void firstPass(NODE_T *ptrNode){
	int index,firstCharacterFlag,firstWordIndex;
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
			index++;
			continue;
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
				break;
			case FIRSTWORD:
				/*printf("entered FIRSTWORD\n");*/
				ptrFirstWord[firstWordIndex] = current->inputChar[index];
				firstWordIndex++;
				break;
			case POSTFW:
				/*printf("entered POSTFW\n");*/
				printf("%s \n",ptrFirstWord);
				exit(0);
				break;
		}		

		index++;
	}


}

