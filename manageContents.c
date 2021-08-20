/*This file contains the manageContents function which analyzes the input file line by line and checks for errors and disects each line into three fields, which include an optional label, command or directive, and corresponding values. Once each line is checked for errors (and printed if necessary, from errorMsg function), the fields from each line are passed into the firstPass function for further analysis.*/

#include "assembler.h" /*include assembler.h header file*/
#include "manageContents.h" /*include manageContents.h header file*/

enum status {PREFIRSTWORD,FIRSTWORD,POSTFIRSTWORD,POSTCOMMAND,POSTDIRECTIVE,POSTLABEL,POSTEXTERN,POSTENTRY,COMMANDORDIRECTIVE,MYDATA,CODE}; /*states of state machine in manageContents*/

static char *directives[]={".db",".dw", ".dh", ".asciz"}; /*array of assembly directives*/

static char *commands[]={"add","sub", "and", "or", "nor", "move", "mvhi","mvlo", "addi", "subi", "andi", "ori","nori", "bne", "beq", "blt", "bgt","lb", "sb", "lw", "sw", "lh","sh", "jmp", "la", "call", "stop"}; /*array of assembly commands*/


/*The manageContents function receives ptrNode of type pointer to NODE_T, and filename of type pointer to char. The function iterates over the lines from the input file using ptrNode, which points to the head of the linked list of lines, and analyzes each line for errors and divides the lines into three fields, which include an optional label, command or directive, and corresponding values. These fields are passed into the firstPass function for further analysis. The manageContents function does not return anything as it is a void function but prints out error messages accordingly with the filename from the input and the line number from the ptrNode struct.*/
void manageContents(NODE_T *ptrNode, char *filename){
	int index,firstWordIndex,labelFlag,labelIndex,midLabel,CommandDirectiveIndex,dataIndex,codeIndex,errorDetected,i,breakFlag,firstFieldFlag,labelWithExtEntFlag;
	NODE_T *current;
	char *ptrFirstWord; /*pointer to char to store first field (word) of line*/
	char *ptrLabel; /*pointer to char to store label*/
	char *ptrCommandDirective; /*pointer to char to store command or directive*/
	char *ptrData; /*pointer to char to store data*/
	char *ptrCode; /*pointer to char to store code*/
	char *ptrCodeChecked; /*pointer to char to store checked code*/
	char *ptrDataChecked; /*pointer to char to store checked data*/
	FIELD_BUFFER_T *headFields; /*pointer to head of field table containing all fields from all lines of file*/

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
					errorMsg(ERRORTYPE22,current->lineNumber,NULL,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
			}
			/*check if current state of state machine is POSTFIRSTWORD*/
			if(state == POSTFIRSTWORD){
				if(!strcmp("stop", ptrFirstWord)){
					firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send stop command to firstPass function*/
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					state = checkState(ptrFirstWord); /*check state of state machine*/
					if(state == UNKNOWNSTATE){
						errorMsg(ERRORTYPE21,current->lineNumber,ptrFirstWord,filename); /*print error message*/
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
						continue;
					}
					else{
						errorMsg(ERRORTYPE22,current->lineNumber,NULL,filename); /*print error message*/
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
				}
				else if(state != UNKNOWNSTATE){
					errorMsg(ERRORTYPE22,current->lineNumber,ptrCommandDirective,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
				}
				else{
					errorMsg(ERRORTYPE20,current->lineNumber,ptrCommandDirective,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
						headFields = addToFieldBuffer(ptrCodeChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrCode,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
				/*check if label detected*/
				else if(labelFlag == FLAGOFF){
					ptrCodeChecked = checkCommand(ptrCode,ptrFirstWord,current->lineNumber,filename); /*check to see if values after command are formatted correctly*/
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,ptrFirstWord,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
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
				 /*check to see if label detected*/
				if(labelFlag == FLAGON){
					ptrDataChecked = checkData(ptrData,ptrCommandDirective,current->lineNumber,filename); /*check to see if values after directive are formatted correctly*/
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(ptrFirstWord,ptrCommandDirective,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
						headFields = addToFieldBuffer(ptrDataChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
				else if(labelFlag == FLAGOFF){
					ptrDataChecked = checkData(ptrData,ptrFirstWord,current->lineNumber,filename); /*check to see if values after directive are formatted correctly*/
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,ptrFirstWord,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
						headFields = addToFieldBuffer(ptrDataChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/

				}
			}
			/*check to see if last line of file reached*/
			if(current->next == NULL){
				firstPass(NULL,NULL,NULL,LASTLINE,errorDetected,filename,current->lineNumber); /*send firstPass end of file flag*/	
				freeFields(headFields); /*free all fields from all lines that were stored from addToFieldBuffer*/
				break;
			}
			else{
				state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
			}
		}

		/*check to see if maximum line length exceeded*/
		if((index >= MAXLINELEN) && (current->inputChar[index] != '\n')){
			errorMsg(ERRORTYPE23,current->lineNumber,NULL,filename); /*print error message*/
			errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
			firstPass(ptrFirstWord,"-1",NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
			/*check if current state of state machine is FIRSTWORD or POSTFIRSTWORD*/
			if(state == FIRSTWORD || state == POSTFIRSTWORD){
				headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
				firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
			}
			/*check if current state of state machine is COMMANDORDIRECTIVE*/
			else if(state == COMMANDORDIRECTIVE){
				headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
				firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
				headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
			}
			/*check if current state of state machine is CODE*/
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
			/*check if current state of state machine is MYDATA*/
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
				if(state == UNKNOWNSTATE){
					errorMsg(ERRORTYPE20,current->lineNumber,ptrCommandDirective,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				/*check if command is stop*/
				if(!strcmp("stop", ptrCommandDirective)){
					errorDetected = checkExtraneousChars(&current,&index);
					if(errorDetected == FLAGON){
						errorMsg(ERRORTYPE4,current->lineNumber,NULL,filename); /*print error message*/
						firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/						
					}
					else if(errorDetected == FLAGOFF){
						firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				/*check if current state of state machine is POSTEXTERN or POSTENTRY*/
				if(state == POSTEXTERN || state == POSTENTRY){
					ptrLabel = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store label after extern or entry*/
					labelIndex = STARTINDEX;
					labelWithExtEntFlag = FLAGON;
					continue;
				}
			}
			/*check if current state of state machine is POSTEXTERN or POSTENTRY and in the middle of defining label (midLabel == FLAGON)*/
			else if((state == POSTEXTERN || state == POSTENTRY) && (midLabel == FLAGON)){
				ptrLabel[labelIndex] = '\0'; /*place null terminator to complete string*/
				midLabel = FLAGOFF;
				/*check to see if label has same name as saved assembly word*/
				if((checkState(ptrLabel) != UNKNOWNSTATE) || !strcmp("dh", ptrLabel) || !strcmp("dw", ptrLabel) || !strcmp("db", ptrLabel) || !strcmp("asciz", ptrLabel)){
					errorMsg(ERRORTYPE3,current->lineNumber,ptrLabel,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
				/*check to see if label exceeds maximum label length*/
				else if(strlen(ptrLabel) >= MAXLABELLEN){
					errorMsg(ERRORTYPE0,current->lineNumber,ptrLabel,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
				/*check to see if label starts with allowable character*/
				else if(!((ptrLabel[0] >= 'a' && ptrLabel[0] <= 'z') || (ptrLabel[0] >= 'A' && ptrLabel[0] <= 'Z'))){
					errorMsg(ERRORTYPE1,current->lineNumber,ptrLabel,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
					/*check to see if label has all valid characters*/
					for(i=1;ptrLabel[i] != '\0';i++){
						if(!((ptrLabel[i] >= 'a' && ptrLabel[i] <= 'z') || (ptrLabel[i] >= 'A' && ptrLabel[i] <= 'Z') || (ptrLabel[i] >= '0' && ptrLabel[i] <= '9'))){
							errorMsg(ERRORTYPE2,current->lineNumber,ptrLabel,filename); /*print error message*/
							errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
							firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
				/*check for extraneous chars at end of line*/
				errorDetected = checkExtraneousChars(&current,&index);
				if(errorDetected == FLAGON){
					errorMsg(ERRORTYPE4,current->lineNumber,NULL,filename); /*print error message*/
					if(labelWithExtEntFlag == FLAGON){
						firstPass(ptrFirstWord,ptrCommandDirective,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/	
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/	
					}					
				}
				else if(errorDetected == FLAGOFF){
					if(labelWithExtEntFlag == FLAGON){
						firstPass(ptrFirstWord,ptrCommandDirective,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
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
				ptrCode[codeIndex] = current->inputChar[index]; /*store current character in code field string*/
				codeIndex++;
				index++;
				continue;
			}
			/*check if current state of state machine is MYDATA*/
			else if(state == MYDATA){
				ptrData[dataIndex] = current->inputChar[index]; /*store current character in data field string*/
				dataIndex++;
				index++;
				continue;
			}
			else{
				index++;
				continue;
			}
		}
		/*check to see if current line is a comment line*/
		if(current->inputChar[index] == ';' && state == PREFIRSTWORD){
			state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
			continue;
		}
		/*check to see if new line character entered*/
		if(current->inputChar[index] == '\n'){
			/*check if current state of state machine is FIRSTWORD*/
			if(state == FIRSTWORD){
				ptrFirstWord[firstWordIndex] = '\0'; /*place null terminator to complete string*/
				if(!strcmp("stop", ptrFirstWord)){
					firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					errorMsg(ERRORTYPE22,current->lineNumber,NULL,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(ptrFirstWord,"-1",NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
			}
			/*check if current state of state machine is POSTFIRSTWORD*/
			if(state == POSTFIRSTWORD){
				if(!strcmp("stop", ptrFirstWord)){
					firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					continue;
				}
				else{
					state = checkState(ptrFirstWord);
					if(state == UNKNOWNSTATE){
						errorMsg(ERRORTYPE21,current->lineNumber,ptrFirstWord,filename); /*print error message*/
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(ptrFirstWord,"-1",NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
						continue;
					}
					else{
						errorMsg(ERRORTYPE22,current->lineNumber,NULL,filename); /*print error message*/
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(ptrFirstWord,"-1",NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
					ptrCodeChecked = checkCommand(ptrCode,ptrCommandDirective,current->lineNumber,filename); /*check if values after command are valid*/
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
						headFields = addToFieldBuffer(ptrCodeChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrCode,firstFieldFlag); /*add field to field buffer in order to free later*/

				}
				else if(labelFlag == FLAGOFF){
					ptrCodeChecked = checkCommand(ptrCode,ptrFirstWord,current->lineNumber,filename); /*check if values after command are valid*/
					if(ptrCodeChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,ptrFirstWord,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrCodeChecked,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
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
				/*check to see if label is same as saved assembly word*/
				if((checkState(ptrLabel) != UNKNOWNSTATE) || !strcmp("dh", ptrLabel) || !strcmp("dw", ptrLabel) || !strcmp("db", ptrLabel) || !strcmp("asciz", ptrLabel)){
					errorMsg(ERRORTYPE3,current->lineNumber,ptrLabel,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
				/*check to see if label exceeds maximum label length*/
				else if(strlen(ptrLabel) >= MAXLABELLEN){
					errorMsg(ERRORTYPE0,current->lineNumber,ptrLabel,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
				/*check to see if label starts with valid character*/
				else if(!((ptrLabel[0] >= 'a' && ptrLabel[0] <= 'z') || (ptrLabel[0] >= 'A' && ptrLabel[0] <= 'Z'))){
					errorMsg(ERRORTYPE1,current->lineNumber,ptrLabel,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
					/*check to see if label is composed of all valid characters*/
					for(i=1;ptrLabel[i] != '\0';i++){
						if(!((ptrLabel[i] >= 'a' && ptrLabel[i] <= 'z') || (ptrLabel[i] >= 'A' && ptrLabel[i] <= 'Z') || (ptrLabel[i] >= '0' && ptrLabel[i] <= '9'))){
							errorMsg(ERRORTYPE2,current->lineNumber,ptrLabel,filename); /*print error message*/
							errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
							firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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

				if(labelWithExtEntFlag == FLAGON){
					firstPass(ptrFirstWord,ptrCommandDirective,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
				}
				else{
					firstPass(NULL,ptrFirstWord,ptrLabel,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
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
				/*check if label is detected*/
				if(labelFlag == FLAGON){
					ptrDataChecked = checkData(ptrData,ptrCommandDirective,current->lineNumber,filename); /*check if values after directive are valid*/
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(ptrFirstWord,ptrCommandDirective,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					}
					else{
						firstPass(ptrFirstWord,ptrCommandDirective,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
						headFields = addToFieldBuffer(ptrDataChecked,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					}
					headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
					firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
					headFields = addToFieldBuffer(ptrCommandDirective,firstFieldFlag); /*add field to field buffer in order to free later*/
					headFields = addToFieldBuffer(ptrData,firstFieldFlag); /*add field to field buffer in order to free later*/
				}
				else if(labelFlag == FLAGOFF){
					ptrDataChecked = checkData(ptrData,ptrFirstWord,current->lineNumber,filename); /*check if values after directive are valid*/
					if(ptrDataChecked == NULL){
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(NULL,ptrFirstWord,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
					}
					else{
						firstPass(NULL,ptrFirstWord,ptrDataChecked,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
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
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
				}
				else if(state != UNKNOWNSTATE){
					errorMsg(ERRORTYPE22,current->lineNumber,ptrCommandDirective,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
				}
				else{
					errorMsg(ERRORTYPE20,current->lineNumber,ptrCommandDirective,filename); /*print error message*/
					errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
					firstPass(ptrFirstWord,ptrCommandDirective,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
				ptrFirstWord = calloc(MAXLINELEN,sizeof(char)); /*allocate memory for first field*/
				firstWordIndex = STARTINDEX;
				ptrFirstWord[firstWordIndex] = current->inputChar[index]; /*set first character of first field to current character*/
				firstWordIndex++; /*increment first word index*/
				state = FIRSTWORD; /*set the state to FIRSTWORD*/
				index++; /*increment characters in line index*/
				break;
			case FIRSTWORD:
				ptrFirstWord[firstWordIndex] = current->inputChar[index]; /*set current character of first field to current character of line*/
				firstWordIndex++; /*increment first word index*/
				index++; /*increment characters in line index*/
				break;
			case POSTFIRSTWORD:
				/*printf("entered POSTFIRSTWORD\n");*/
				ptrFirstWord[firstWordIndex] = '\0'; /*place null terminator to complete string*/
				/*check if label detected*/
				if(labelFlag == FLAGON){
					state = POSTLABEL; /*set state to POSTLABEL*/
					/*check if label is same as saved assembly word*/
					if((checkState(ptrFirstWord) != UNKNOWNSTATE) || !strcmp("dh", ptrFirstWord) || !strcmp("dw", ptrFirstWord) || !strcmp("db", ptrFirstWord) || !strcmp("asciz", ptrFirstWord)){
						errorMsg(ERRORTYPE3,current->lineNumber,ptrFirstWord,filename); /*print error message*/
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(ptrFirstWord,"-1",NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					}
					/*check if label exceeds maximum label length*/
					else if(strlen(ptrFirstWord) >= MAXLABELLEN){
						errorMsg(ERRORTYPE0,current->lineNumber,ptrFirstWord,filename); /*print error message*/
						/*printf("Line %u: Unrecognized command or directive %s (or label with missing :)\n",current->lineNumber,ptrFirstWord);*/
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(ptrFirstWord,"-1",NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					}
					/*check if label begins with valid character*/
					else if(!((ptrFirstWord[0] >= 'a' && ptrFirstWord[0] <= 'z') || (ptrFirstWord[0] >= 'A' && ptrFirstWord[0] <= 'Z'))){
						errorMsg(ERRORTYPE1,current->lineNumber,ptrFirstWord,filename); /*print error message*/
						errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
						firstPass(ptrFirstWord,"-1",NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
					}
					else{
						/*check if label contains all valid characters*/
						for(i=1;ptrFirstWord[i] != '\0';i++){
							if(!((ptrFirstWord[i] >= 'a' && ptrFirstWord[i] <= 'z') || (ptrFirstWord[i] >= 'A' && ptrFirstWord[i] <= 'Z') || (ptrFirstWord[i] >= '0' && ptrFirstWord[i] <= '9'))){
								errorMsg(ERRORTYPE2,current->lineNumber,ptrFirstWord,filename); /*print error message*/
								errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
								firstPass(ptrFirstWord,"-1",NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
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
							errorMsg(ERRORTYPE4,current->lineNumber,NULL,filename); /*print error message*/
							firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/						
						}
						else if(errorDetected == FLAGOFF){
							firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber);  /*send fields from current line to firstPass*/
						}
						headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
						firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
						state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
						break;
					}
					else{
						state = checkState(ptrFirstWord);
						if(state == UNKNOWNSTATE){
							errorMsg(ERRORTYPE21,current->lineNumber,ptrFirstWord,filename);
							errorDetected = FLAGON; /*set errorDetected to on as error has been detected*/
							firstPass(NULL,ptrFirstWord,NULL,labelFlag,errorDetected,filename,current->lineNumber); /*send errorDetected with flag on to firstPass*/
							headFields = addToFieldBuffer(ptrFirstWord,firstFieldFlag); /*add field to field buffer in order to free later*/
							firstFieldFlag = FLAGOFF; /*set firstFieldFlag to off as the first field has already been analyzed*/
							state = newLine(&errorDetected,&current,&labelFlag,&index); /*call newLine function to transition to next line in file*/
							break;
							/*exit(0);*/
						}
					}
				}
				if(state == POSTEXTERN){
					ptrLabel = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store label after extern*/
					labelIndex = STARTINDEX;
				}
				else if(state == POSTENTRY){
					ptrLabel = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store label after entry*/
					labelIndex = STARTINDEX;
				}
				else if(state == POSTLABEL){
					index++;
				}
				break;
			case POSTCOMMAND:
				ptrCode = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store label values after command*/ 
				codeIndex = STARTINDEX;
				state = CODE; /*set current state to CODE*/			
				break;
			case POSTDIRECTIVE:
				ptrData = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store label values after directive*/ 
				dataIndex = STARTINDEX;
				state = MYDATA; /*set current state to DATA*/
				break;
			case POSTLABEL:
				ptrCommandDirective = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store command or directive*/ 
				CommandDirectiveIndex = STARTINDEX;
				ptrCommandDirective[CommandDirectiveIndex] = current->inputChar[index]; /*store current character in first character of command or directive field*/
				CommandDirectiveIndex++; /*increment command or directive index*/
				state = COMMANDORDIRECTIVE; /*set current state to COMMANDORDIRECTIVE*/
				index++; /*increment characters in current line index*/

				break;
			case POSTEXTERN:
				midLabel = FLAGON; /*set midLabel flag to on*/
				ptrLabel[labelIndex] = current->inputChar[index]; /*store current character in first character of label field*/ 
				labelIndex++; /*increment label index*/
				index++; /*increment characters in current line index*/
				break;
			case POSTENTRY:
				midLabel = FLAGON; /*set midLabel flag to on*/
				ptrLabel[labelIndex] = current->inputChar[index]; /*store current character in first character of label field*/ 
				labelIndex++; /*increment label index*/
				index++; /*increment characters in current line index*/
				break;
			case COMMANDORDIRECTIVE:
				ptrCommandDirective[CommandDirectiveIndex] = current->inputChar[index]; /*store current character in first character of label field*/ 
				CommandDirectiveIndex++; /*increment command or directive index*/
				index++; /*increment characters in current line index*/
				break;
			case MYDATA:
				ptrData[dataIndex] = current->inputChar[index]; /*store current character in first character of data field*/
				dataIndex++; /*increment data index*/
				index++; /*increment characters in current line index*/
				break;
			case CODE:
				ptrCode[codeIndex] = current->inputChar[index]; /*store current character in first character of code field*/
				codeIndex++; /*increment code index*/
				index++; /*increment characters in current line index*/
				break;
		}		
	}
}


/*The checkState function receives ptrInput of type pointer to char and returns an int of the current state of the state machine in manageContents. The function acheives this by comparing the string in ptrInput to all the possible commands and directives and returns the corresponding state as an integer or UNKNOWNSTATE (-1) if the state is unknown.*/
int checkState(char *ptrInput){
	int i;
	enum status state;

	/*if input is a directive set state to POSTDIRECTIVE*/
	for(i=STARTINDEX;i<NUMDIRECTIVES;i++){
		if(!strcmp(directives[i], ptrInput)){
			state = POSTDIRECTIVE;
			return state;
		}
	}
	/*if input is .extern set state to POSTEXTERN*/
	if(!strcmp(".extern", ptrInput)){
		state = POSTEXTERN;
		return state;
	}
	/*if input is .entry set state to POSTENTRY*/
	if(!strcmp(".entry", ptrInput)){
		state = POSTENTRY;
		return state;
	}
	/*if input is stop set state to POSTCOMMAND*/
	if(!strcmp("stop", ptrInput)){
		state = POSTCOMMAND;
		return state;
	}
	/*if input is a command set state to POSTCOMMAND*/
	for(i=STARTINDEX;i<NUMCOMMANDS;i++){
		if(!strcmp(commands[i], ptrInput)){
			state = POSTCOMMAND;
			return state;
		}
	}
	return UNKNOWNSTATE; /*if state is unknown*/
}

/*The errorMsg function receives error of type int, lineNumber of type int, fieldName of type pointer to char, and fileName of type pointer to char. The function is void so it does not return anything but it prints error messages according to the error type specified in the error input, along with the corressponding line number from the lineNumber input, field name from the fieldName input, and file name from the fileName input.*/
void errorMsg(int error,int lineNumber,char *fieldName,char *fileName)
{
	/*error input enters switch case and prints out corresponding error message*/
	switch (error)
	{
		case ERRORTYPE0:
			printf("File \"%s.as\", Line %u: Label length of %s greater than maximum label length of 31\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE1:
			printf("File \"%s.as\", Line %u: Label %s must start with alphabetic letter\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE2:
			printf("File \"%s.as\", Line %u: Label %s must be composed of only alphabetic letters or numbers\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE3:
			printf("File \"%s.as\", Line %u: Label %s must not have the same name as a saved Assembly word\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE4:
			printf("File \"%s.as\", Line %u: Extraneous character(s) at end of line\n",fileName,lineNumber);
			break;
		case ERRORTYPE5:
			printf("File \"%s.as\", Line %u: Expected $ sign\n",fileName,lineNumber);
			break;
		case ERRORTYPE6:
			printf("File \"%s.as\", Line %u: Illegal consecutive commas\n",fileName,lineNumber);
			break;
		case ERRORTYPE7:
			printf("File \"%s.as\", Line %u: Expected number after $ sign\n",fileName,lineNumber);
			break;
		case ERRORTYPE8:
			printf("File \"%s.as\", Line %u: Unexpected comma\n",fileName,lineNumber);
			break;
		case ERRORTYPE9:
			printf("File \"%s.as\", Line %u: Extraneous character(s) at end of line\n",fileName,lineNumber);
			break;
		case ERRORTYPE10:
			printf("File \"%s.as\", Line %u: Unexpected character\n",fileName,lineNumber);
			break;
		case ERRORTYPE11:
			printf("File \"%s.as\", Line %u: Missing comma\n",fileName,lineNumber);
			break;
		case ERRORTYPE12:
			printf("File \"%s.as\", Line %u: Missing register(s)\n",fileName,lineNumber);
			break;
		case ERRORTYPE13:
			printf("File \"%s.as\", Line %u: Missing label\n",fileName,lineNumber);
			break;
		case ERRORTYPE14:
			printf("File \"%s.as\", Line %u: Expected register or (legal) label after jmp command\n",fileName,lineNumber);
			break;
		case ERRORTYPE15:
			printf("File \"%s.as\", Line %u: Expected (legal) label after la or call command\n",fileName,lineNumber);
			break;
		case ERRORTYPE16:
			printf("File \"%s.as\", Line %u: Expected quotation marks after .asciz directive\n",fileName,lineNumber);
			break;
		case ERRORTYPE17:
			printf("File \"%s.as\", Line %u: Expected addtional quotation marks to close initial quotation marks\n",fileName,lineNumber);
			break;
		case ERRORTYPE18:
			printf("File \"%s.as\", Line %u: Expected number or + or - after .db/.dw/.dh directive\n",fileName,lineNumber);
			break;
		case ERRORTYPE19:
			printf("File \"%s.as\", Line %u: Expected number after +/- sign\n",fileName,lineNumber);
			break;
		case ERRORTYPE20:
			printf("File \"%s.as\", Line %u: Unrecognized command or directive \"%s\"\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE21:
			printf("File \"%s.as\", Line %u: Unrecognized command or directive \"%s\" (or label with missing :)\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE22:
			printf("File \"%s.as\", Line %u: Line incomplete\n",fileName,lineNumber);
			break;
		case ERRORTYPE23:
			printf("File \"%s.as\", Line %u: Line exceeded maximum length of 80 characters\n",fileName,lineNumber);
			break;
		case ERRORTYPE24:
			printf("File \"%s.as\", Line %u: Label \"%s\" not found in symbol table\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE25:
			printf("File \"%s.as\", Line %u: Label \"%s\" not found!\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE26:
			printf("File \"%s.as\", Line %u: External label \"%s\" used for conditional branching!\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE27:
			printf("File \"%s.as\", Line %u: Register value must be between 0 and 31\n",fileName,lineNumber);
			break;
		case ERRORTYPE28:
			printf("File \"%s.as\", Line %u: Immediate value out of range\n",fileName,lineNumber);
			break;
		case ERRORTYPE29:
			printf("File \"%s.as\", Line %u: .db value out of range\n",fileName,lineNumber);
			break;
		case ERRORTYPE30:
			printf("File \"%s.as\", Line %u: .dw value out of range\n",fileName,lineNumber);
			break;
		case ERRORTYPE31:
			printf("File \"%s.as\", Line %u: .dh value out of range\n",fileName,lineNumber);
			break;
		case ERRORTYPE32:
			printf("File \"%s.as\", Line %u: Label \"%s\" already defined previously not as external!\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE33:
			printf("File \"%s.as\", Line %u: Label \"%s\" already defined previously!\n",fileName,lineNumber,fieldName);
			break;
		case ERRORTYPE34:
			printf("File \"%s.as\", Line %u: Entry label \"%s\" defined also as external!\n",fileName,lineNumber,fieldName);
			break;
		default:
			printf("Default error message!\n");
			break;
	}
}

/*The newLine function receives errorDetected of type pointer to int, current of type pointer to pointer to NODE_T, labelFlag of type pointer to int, index of type pointer to int, and returns an int. The newLine function transitions to analyze a new line from the input file by setting the errorDetected flag to off, setting the current node to the next node, setting the labelFlag to off, and setting the index back to zero, and returns an integer of the state back to the initial state (PREFIRSTWORD) of the state machine switch case in manageContents.*/				
int newLine(int *errorDetected,NODE_T **current,int *labelFlag,int *index){
	*errorDetected = FLAGOFF; /*sets errorDetected flag to off*/
	*current = (*current)->next; /*sets current node to next node (next line)*/
	*labelFlag = FLAGOFF; /*sets labelFlag to off*/
	*index = STARTINDEX; /*zeros index*/
	return PREFIRSTWORD; /*returns state as PREFIRSTWORD*/
}

/*The checkExtraneousChars function receives current of type pointer to pointer to NODE_T, and index of type pointer to index and returns an int, symboling if an error was detected or not. The function iterates over the remaining characters in the corresponding index in the current node and ensures that only white space characters are found. If only white space characters are detected, then the function returns the error flag as FLAGOFF, and if not the function returns the error flag as FLAGON.*/
int checkExtraneousChars(NODE_T **current,int *index){
	/*while null terminator and new line character not reached*/
	while(((*current)->inputChar[*index] != '\0') && ((*current)->inputChar[*index] != '\n')){
		if((*current)->inputChar[*index] == '\t' || (*current)->inputChar[*index] == ' '){
			(*index)++; /*if white space increment index*/
		}
		else{
			return FLAGON; /*if not white space, return error flag as FLAGON (error detected)*/
		}
	}		
	return FLAGOFF; /*if reached end of line without non-whitespace, return error flag as FLAGOFF (no error detected)*/
}

/*The function addToFieldBuffer receives field of type pointer to char, and firstFieldFlag of type int and returns a pointer to FIELD_BUFFER_T which is the head of the created linked list. The function uses the firstField flag to specify the head of the linked list when the flag is on and if it is off it continues to add to the linked list. The function creates a linked list of all the fields allocated in manageContents so all the fields can eventually be freed.*/
FIELD_BUFFER_T* addToFieldBuffer(char *field,int firstFieldFlag){
	static FIELD_BUFFER_T *head; /*head of the linked list*/
	static FIELD_BUFFER_T *current; /*current node of the linked list*/
	FIELD_BUFFER_T *ptrNode; /*pointer to linked list*/
	FIELD_BUFFER_T *tmpPtr; /*temporary pointer to linked list*/

	tmpPtr = (FIELD_BUFFER_T*)calloc(SINGLENODE, sizeof(FIELD_BUFFER_T)); /*allocating memory for node of linked list*/
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/


	if(firstFieldFlag == FLAGON){
		head = ptrNode; /*set head to ptrNode if firstFieldFlag is on.*/
	}
	else{
		current->next = ptrNode; /*set next node to ptrNode if firstFieldFlag is off.*/
	}

	ptrNode->field = field; /*set field in ptrNode struct to field*/
	ptrNode->next = NULL; /*set next in ptrNode struct to NULL*/

	current = ptrNode; /*set current to ptrNode*/

	return head; /*return head of linked list*/

}

/*The freeFields function receives head of type pointer to FIELD_BUFFER_T and returns nothing as it is a void function. The function receives the head of the field buffer created in the addToFieldBuffer function, and iterates through the buffer and frees each field and struct from within the buffer. The function iterates through the linked list until the final node is reached*/
void freeFields(FIELD_BUFFER_T *head){
	FIELD_BUFFER_T *current; /*current node*/
	FIELD_BUFFER_T *tmp; /*temporary node*/
	current = head; /*set current node to head*/
	/*iterate until end of linked list*/
	while(1){
		tmp = current; /*set temp to current*/
		if(tmp == NULL){
			break; /*break if end of linked list reached*/
		}
		current = tmp->next; /*set current node to next node of temp*/
		free(tmp->field); /*free field from tmp struct*/
		free(tmp); /*free tmp*/
	}
}





