/*The following file receives the code following a command and checks the code for errors. The file contains the checkCommand function which analyzes the type of command and calls one of the remaing functions in the file to analyze the code for errors accordingly.*/

#include "assembler.h" /*include assembler.h header file*/
#include "checkCommands.h" /*include checkCommands.h header file*/

static char *Rcommands1[]={"add","sub", "and", "or", "nor"}; /*R commands of type 1*/

static char *Rcommands2[]={"move", "mvhi","mvlo"}; /*R commands of type 2*/

static char *Icommands1[]={"addi", "subi", "andi", "ori","nori","lb", "sb", "lw", "sw", "lh","sh"}; /*I commands of type 1*/

static char *Icommands2[]={"bne", "beq", "blt", "bgt"}; /*I commands of type 2*/

enum comType {Rcom1,Rcom2,Icom1,Icom2,jumpCom,laOrcallCom}; /*enum of all com types to check*/


/*The checkCommand function receives ptrCode of type pointer to char, ptrCommand of type pointer to char, lineNumber of type int, filename of type pointer to char, and returns a pointer to char which stores the checked code. The function checks the command stored in ptrCommand and calls the corresponding function to analyze the code following the command according to the type of command. The function returns the checked code if no errors were detected, and returns NULL if errors were detected.*/
char* checkCommand(char *ptrCode,char *ptrCommand,int lineNumber,char *filename){
	int i,currentComType;
	char *ptrCodeChecked; /*pointer to char to store checked code*/

	/*check if current command is "add","sub", "and", "or", "nor"*/
	for(i=STARTINDEX;i<NUMRCOM1;i++){
		if(!strcmp(Rcommands1[i], ptrCommand)){
			currentComType = Rcom1;
		}
	}
	/*check if current command is "move", "mvhi","mvlo"*/
	for(i=STARTINDEX;i<NUMRCOM2;i++){
		if(!strcmp(Rcommands2[i], ptrCommand)){
			currentComType = Rcom2;
		}
	}
	/*check if current command is "addi", "subi", "andi", "ori","nori","lb", "sb", "lw", "sw", "lh","sh"*/
	for(i=STARTINDEX;i<NUMICOM1;i++){
		if(!strcmp(Icommands1[i], ptrCommand)){
			currentComType = Icom1;
		}
	}
	/*check if current command is "bne", "beq", "blt", "bgt"*/
	for(i=STARTINDEX;i<NUMICOM2;i++){
		if(!strcmp(Icommands2[i], ptrCommand)){
			currentComType = Icom2;
		}
	}
	/*check if current command is "jmp"*/
	if(!strcmp("jmp",ptrCommand)){
		currentComType = jumpCom;
	}
	/*check if current command is "la", "call"*/
	else if(!strcmp("la",ptrCommand) || !strcmp("call",ptrCommand)){
		currentComType = laOrcallCom;
	}


	if(currentComType == Rcom1){
		ptrCodeChecked = checkCommandR1(ptrCode,lineNumber,filename); /*enter checkCommandR1 if command is "add","sub", "and", "or", "nor"*/
	}
	else if(currentComType == Rcom2){
		ptrCodeChecked = checkCommandR2(ptrCode,lineNumber,filename); /*enter checkCommandR2 if command is "move", "mvhi","mvlo"*/
	}
	else if(currentComType == Icom1){
		ptrCodeChecked = checkCommandI1(ptrCode,lineNumber,filename); /*enter checkCommandI1 if command is "addi", "subi", "andi", "ori","nori","lb", "sb", "lw", "sw", "lh","sh"*/
	}
	else if(currentComType == Icom2){
		ptrCodeChecked = checkCommandI2(ptrCode,lineNumber,filename); /*enter checkCommandI2 if command is "bne", "beq", "blt", "bgt"*/
	}
	else if(currentComType == jumpCom){
		ptrCodeChecked = checkCommandjump(ptrCode,lineNumber,filename); /*enter checkCommandjump if command is "jmp"*/
	}
	else if(currentComType == laOrcallCom){
		ptrCodeChecked = checkCommandlaOrcall(ptrCode,lineNumber,filename); /*enter checkCommandlaOrcall if command is "la","call"*/
	}
	return ptrCodeChecked; /*return ptrCodeChecked string*/

}


/*The checkCommandR1 function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "add","sub", "and", "or", "nor" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function does this with the assumption that these types of commands contain three registers and they all should be comma separated. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandR1(char *ptrCode,int lineNumber,char *filename){

	int dollarCount, dollarFlag,index,newIndex,commaFlag,numberFlag,endLineFlag,expectCommaFlag,numDollars;
	char *ptrCodeChecked; /*pointer to char to store checked code*/

	index = STARTINDEX; /*set index of code to be checked to zero*/
	newIndex = STARTINDEX; /*set index of checked code to zero*/
	dollarCount = STARTINDEX; /*number of dollars detected so far*/
	numDollars = NUMREGSR1; /*number of dollars in R1 type command*/
	dollarFlag = FLAGOFF; /*set dollar flag to off*/
	commaFlag = FLAGOFF; /*comma not detected yet*/
	endLineFlag = FLAGOFF; /*set end of line flag to off*/
	numberFlag = FLAGOFF;  /*number not detected yet*/
	expectCommaFlag = FLAGOFF; /*set expected comma flag to off*/

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store checked code*/

	/*while end of code to be checked not reached*/
	while(ptrCode[index] != '\0'){
		/*check if comma previously detected*/
		if(commaFlag == FLAGON){
			/*check if white space detected*/
			if(!((ptrCode[index] == '\t') || (ptrCode[index] == ' '))){
				/*check if comma detected*/
				if(ptrCode[index] == ','){
					/*print error of consecutive commas, free ptrCodeChecked and return NULL*/
					errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else{
					commaFlag = FLAGOFF; /*set comma flag to off*/
				}
			}
		}
		/*check if after dollar detection*/
		if(dollarFlag == FLAGON){
			/*check if number detected*/
			if(!(ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			else{
				dollarFlag = FLAGOFF; /*set dollar flag to off*/
				numberFlag = FLAGON; /*set number flag to on*/
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			}
		}
		/*check if after number detection*/
		if(numberFlag == FLAGON){
			/*check if white space detected*/
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				numberFlag = FLAGOFF; /*set number flag to off*/
				if(dollarCount<numDollars){
					expectCommaFlag = FLAGON; /*set expect comma flag to on*/
				}
			}
			/*check if comma detected*/
			else if(ptrCode[index] == ','){
				numberFlag = FLAGOFF; /*set number flag to off*/
			}
			/*check if number detected*/
			else if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			} 
			else{
				/*print error of unexpected character, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if white space detected*/
		if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
			index++; /*increment unchecked code index*/
			if(dollarCount>=numDollars){
				endLineFlag = FLAGON;
			}
		}
		else if((expectCommaFlag == FLAGON) && (ptrCode[index] != ',')){
			/*print error of missing comma, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE11,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		} 
		else if(endLineFlag == FLAGON){
			/*print error of extraneous character at end of line, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if((dollarCount == 0) && (ptrCode[index] != '$')){
			/*print error of expected dollar sign, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE5,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		/*check if comma detected*/
		else if(ptrCode[index] == ','){
			expectCommaFlag = FLAGOFF; /*set expected comma flag to off*/
			if(dollarCount>=numDollars){
				/*print error of unexpected comma, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE8,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			/*if comma detected and comma flag set to off, set comma flag to on*/
			if(commaFlag == FLAGOFF){
				commaFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			/*check if comma previously detected*/
			else if(commaFlag == FLAGON){
				/*print error of consecutive commas, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if dollar sign detected*/
		else if(ptrCode[index] == '$'){
			dollarFlag = FLAGON; /*set dollar flag to on*/
			dollarCount++; /*increment dolar count*/
			ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
			index++; /*increment unchecked code index*/
			newIndex++; /*increment checked code index*/
		}
		else{
			/*print error of unexpected character, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
	}
	/*check if dollar previously detected*/
	if(dollarFlag == FLAGON){
		/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(dollarCount<numDollars){
		/*print error of missing register, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE12,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	ptrCodeChecked[newIndex] = '\0'; /*set final index of checked code to null terminator*/
	return ptrCodeChecked; /*return checked code*/
}

/*The checkCommandR2 function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "move", "mvhi","mvlo" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function achieves this with the assumption that these types of commands are followed with two comma separated registers. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandR2(char *ptrCode,int lineNumber,char *filename){

	int dollarCount, dollarFlag,index,newIndex,commaFlag,numberFlag,endLineFlag,expectCommaFlag,numDollars;
	char *ptrCodeChecked; /*pointer to char to store checked code*/

	index = STARTINDEX; /*set index of code to be checked to zero*/
	newIndex = STARTINDEX; /*set index of checked code to zero*/
	dollarCount = STARTINDEX; /*set dollar count to zero*/
	numDollars = NUMREGSR2; /*number of registers for R2 type command is two*/
	dollarFlag = FLAGOFF; /*set dollar flag to off*/
	commaFlag = FLAGOFF; /*set comma flag to off*/
	endLineFlag = FLAGOFF; /*set end of line flag to off*/
	numberFlag = FLAGOFF; /*set number flag to off*/
	expectCommaFlag = FLAGOFF; /*set expected comma flag to off*/

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store checked code*/

	/*while end of code to be checked not reached*/
	while(ptrCode[index] != '\0'){
		/*check if comma previously detected*/
		if(commaFlag == FLAGON){
			/*check if white space not detected*/
			if(!((ptrCode[index] == '\t') || (ptrCode[index] == ' '))){
				/*check if comma detected*/
				if(ptrCode[index] == ','){
					/*print error of consecutive commas, free ptrCodeChecked and return NULL*/
					errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else{
					commaFlag = FLAGOFF; /*set comma flag to off*/
				}
			}
		}
		/*check if dollar previously detected*/
		if(dollarFlag == FLAGON){
			/*check if number not detected*/
			if(!(ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			else{
				dollarFlag = FLAGOFF; /*set dollar flag to off*/
				numberFlag = FLAGON; /*set number flag to on*/
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			}
		}
		/*check if after number detection*/
		if(numberFlag == FLAGON){
			/*check if white space detected*/
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				numberFlag = FLAGOFF; /*set number flag to off*/
				if(dollarCount<numDollars){
					expectCommaFlag = FLAGON; /*set expect comma flag to on*/
				}
			}
			/*check if comma detected*/
			else if(ptrCode[index] == ','){
				numberFlag = FLAGOFF; /*set number flag to off*/
			}
			/*check if number detected*/
			else if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			} 
			else{
				/*print error of unexpected character, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if white space detected*/
		if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
			index++; /*increment unchecked code index*/
			if(dollarCount>=numDollars){
				endLineFlag = FLAGON;
			}
		}
		else if((expectCommaFlag == FLAGON) && (ptrCode[index] != ',')){
			/*print error of missing comma, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE11,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		} 
		else if(endLineFlag == FLAGON){
			/*print error of extraneous character at end of line, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if((dollarCount == 0) && (ptrCode[index] != '$')){
			/*print error of expected dollar sign, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE5,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		/*check if comma detected*/
		else if(ptrCode[index] == ','){
			expectCommaFlag = FLAGOFF; /*set expected comma flag to off*/
			if(dollarCount>=numDollars){
				/*print error of unexpected comma, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE8,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			/*if comma detected and comma flag set to off, set comma flag to on*/
			if(commaFlag == FLAGOFF){
				commaFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			/*check if comma previously detected*/
			else if(commaFlag == FLAGON){
				/*print error of consecutive commas, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if dollar sign detected*/
		else if(ptrCode[index] == '$'){
			dollarFlag = FLAGON; /*set dollar flag to on*/
			dollarCount++; /*increment dolar count*/
			ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
			index++; /*increment unchecked code index*/
			newIndex++; /*increment checked code index*/
		}
		else{
			/*print error of unexpected character, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
	}
	/*check if dollar previously detected*/
	if(dollarFlag == FLAGON){
		/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(dollarCount<numDollars){
		/*print error of missing register, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE12,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	ptrCodeChecked[newIndex] = '\0'; /*set final index of checked code to null terminator*/
	return ptrCodeChecked; /*return checked code*/
}


/*The checkCommandI1 function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode for ("addi", "subi", "andi", "ori","nori","lb", "sb", "lw", "sw", "lh","sh" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function achieves this with the assumption that these types of command are followed by a register, a comma, an immediate value, another comma, and another register. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandI1(char *ptrCode,int lineNumber,char *filename){

	int dollarCount, dollarFlag,index,newIndex,commaFlag,numberFlag,endLineFlag,expectCommaFlag,numDollars,immediateFlag;
	char *ptrCodeChecked; /*pointer to char to store checked code*/

	index = STARTINDEX; /*set index of code to be checked to zero*/
	newIndex = STARTINDEX; /*set index of checked code to zero*/
	dollarCount = STARTINDEX; /*set dollar count to zero*/
	numDollars = NUMREGSI1; /*number of registers in I1 type command is two*/
	dollarFlag = FLAGOFF; /*set dollar flag to off*/
	commaFlag = FLAGOFF; /*set comma flag to off*/
	endLineFlag = FLAGOFF; /*set end of line flag to off*/
	numberFlag = FLAGOFF; /*set number flag to off*/
	expectCommaFlag = FLAGOFF; /*set expected comma flag to off*/
	immediateFlag = FLAGON; /*set immediate flag to on*/

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store checked code*/
	
	/*while end of code to be checked not reached*/
	while(ptrCode[index] != '\0'){

		/*check if after first register, after comma, and immediate flag on*/
		if((dollarCount == REGISTER) && (commaFlag == FLAGON) && (immediateFlag == FLAGON)){
			immediateFlag = FLAGOFF; /*set immediate flag to off*/
			commaFlag = FLAGOFF; /*set comma flag to off*/
			/*while white space detected*/
			while((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				index++; /*increment unchecked code index*/
			}
			/*check if number detected*/
			if((ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			/*check if number or + or -*/
			else if((ptrCode[index] == '-' || ptrCode[index] == '+') && (ptrCode[index+NEXTINDEX] >= '0' && ptrCode[index+NEXTINDEX] <= '9')){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			else{
				/*print error of unexpected character, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			/*while number detected*/
			while(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			/*while white space detected*/
			while((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				index++; /*increment unchecked code index*/
			}
			/*check if comma detected*/
			if(ptrCode[index] == ','){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				commaFlag = FLAGON; /*set comma flag to on*/
				continue;
			}
			/*check if white space detected*/
			else if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				expectCommaFlag = FLAGON; /*set expect comma flag to on*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			}
			else{
				/*print error of unexpected character, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if comma previously detected*/
		if(commaFlag == FLAGON){
			if(!((ptrCode[index] == '\t') || (ptrCode[index] == ' '))){
				/*check if comma detected*/
				if(ptrCode[index] == ','){
					/*print error of consecutive commas, free ptrCodeChecked and return NULL*/
					errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else{
					commaFlag = FLAGOFF; /*set comma flag to off*/
				}
			}
		}
		/*check if dollar previously detected*/
		if(dollarFlag == FLAGON){
			/*check if number not detected*/
			if(!(ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			else{
				dollarFlag = FLAGOFF; /*set dollar flag to off*/
				numberFlag = FLAGON; /*set number flag to on*/
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			}
		}
		/*check if after number detection*/
		if(numberFlag == FLAGON){
			/*check if white space detected*/
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				numberFlag = FLAGOFF; /*set number flag to off*/
				if(dollarCount<numDollars){
					expectCommaFlag = FLAGON; /*set expect comma flag to on*/
				}
			}
			/*check if comma detected*/
			else if(ptrCode[index] == ','){
				numberFlag = FLAGOFF; /*set number flag to off*/
			}
			/*check if number detected*/
			else if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			} 
			else{
				/*print error of unexpected character, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if white space detected*/
		if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
			index++; /*increment unchecked code index*/
			if(dollarCount>=numDollars){
				endLineFlag = FLAGON;
			}
		}
		/*check if expected comma flag is on and comma not detected*/
		else if((expectCommaFlag == FLAGON) && (ptrCode[index] != ',')){
			/*print error of missing comma, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE11,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		} 
		/*check if end of line flag is on*/
		else if(endLineFlag == FLAGON){
			/*print error of extraneous character at end of line, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if((dollarCount == STARTINDEX) && (ptrCode[index] != '$')){
			/*print error of expected dollar sign, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE5,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		/*check if comma detected*/
		else if(ptrCode[index] == ','){
			expectCommaFlag = FLAGOFF; /*set expected comma flag to off*/
			if(dollarCount>=numDollars){
				/*print error of unexpected comma, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE8,lineNumber,NULL,filename);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			/*if comma detected and comma flag set to off, set comma flag to on*/
			if(commaFlag == FLAGOFF){
				commaFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			/*check if comma previously detected*/
			else if(commaFlag == FLAGON){
				/*print error of consecutive commas, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if dollar sign detected*/
		else if(ptrCode[index] == '$'){
			dollarFlag = FLAGON; /*set dollar flag to on*/
			dollarCount++; /*increment dolar count*/
			ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
			index++; /*increment unchecked code index*/
			newIndex++; /*increment checked code index*/
		}
		else{
			/*print error of unexpected character, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
	}
	/*check if dollar previously detected*/
	if(dollarFlag == FLAGON){
		/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(dollarCount<numDollars){
		/*print error of missing register, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE12,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}

	ptrCodeChecked[newIndex] = '\0'; /*set final index of checked code to null terminator*/

	return ptrCodeChecked; /*return checked code*/
}


/*The checkCommandI2 function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "bne", "beq", "blt", "bgt" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function achieves this with the assumption that these types of commands are followed by two comma seprated registers, another comma, and a label. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandI2(char *ptrCode,int lineNumber,char *filename){

	int dollarCount, dollarFlag,index,newIndex,commaFlag,numberFlag,endLineFlag,expectCommaFlag,numDollars,labelFlag;
	char *ptrCodeChecked; /*pointer to char to store checked code*/

	index = STARTINDEX; /*set index of code to be checked to zero*/
	newIndex = STARTINDEX; /*set index of checked code to zero*/
	dollarCount = STARTINDEX; /*set dollar count to zero*/
	numDollars = NUMREGSI2; /*number of registers for I2 type command is two*/
	dollarFlag = FLAGOFF; /*set dollar flag to off*/
	commaFlag = FLAGOFF; /*set comma flag to off*/
	endLineFlag = FLAGOFF; /*set end of line flag to off*/
	numberFlag = FLAGOFF; /*set number flag to off*/
	expectCommaFlag = FLAGOFF; /*set expected comma flag to off*/
	labelFlag = FLAGON; /*set label flag to on*/

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store checked code*/

	/*while end of code to be checked not reached*/
	while(ptrCode[index] != '\0'){
		if((dollarCount == numDollars) && (commaFlag == FLAGON) && (labelFlag == FLAGON)){
			commaFlag = FLAGOFF; /*set comma flag to off*/
			/*while white space detected*/
			while((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				index++; /*increment unchecked code index*/
			}
			/*check if first letter of label is alphabetical letter uppercase or lowercase*/
			if((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index]<= 'Z')){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			else{
				/*print error of label must begin with alphabetical letter, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE1,lineNumber,"",filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			/*while alphabetical letters or numbers detected*/
			while(((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index] <= 'Z') || (ptrCode[index] >= '0' && ptrCode[index] <= '9'))){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			/*while end of code to be checked not reached*/
			while(ptrCode[index] != '\0'){
				/*check if white space detected*/
				if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
					labelFlag = FLAGOFF;
					index++; /*increment unchecked code index*/
					newIndex++; /*increment checked code index*/
				}
				else{
					if(labelFlag == FLAGON){
						/*print error of label must be composed of alphabetical letters and numbers, free ptrCodeChecked and return NULL*/
						errorMsg(ERRORTYPE2,lineNumber,"",filename);
						free(ptrCodeChecked);
						ptrCodeChecked = NULL;
						return ptrCodeChecked;
					}
					else if(labelFlag == FLAGOFF){
						/*print error of extraneous character at end of line, free ptrCodeChecked and return NULL*/
						errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
						free(ptrCodeChecked);
						ptrCodeChecked = NULL;
						return ptrCodeChecked;
					}
				}
			}
			return ptrCodeChecked; /*return checked code*/
		}
		/*check if comma previously detected*/
		if(commaFlag == FLAGON){
			if(!((ptrCode[index] == '\t') || (ptrCode[index] == ' '))){
				/*check if comma detected*/
				if(ptrCode[index] == ','){
					/*print error of consecutive commas, free ptrCodeChecked and return NULL*/
					errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else{
					commaFlag = FLAGOFF; /*set comma flag to off*/
				}
			}
		}
		/*check if dollar previously detected*/
		if(dollarFlag == FLAGON){
			/*check if number not detected*/
			if(!(ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			else{
				dollarFlag = FLAGOFF; /*set dollar flag to off*/
				numberFlag = FLAGON; /*set number flag to off*/
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			}
		}
		/*check if after number detection*/
		if(numberFlag == FLAGON){
			/*check if white space detected*/
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				numberFlag = FLAGOFF; /*set number flag to off*/
				if(dollarCount<numDollars){
					expectCommaFlag = FLAGON; /*set expect comma flag to on*/
				}
			}
			/*check if comma detected*/
			else if(ptrCode[index] == ','){
				numberFlag = FLAGOFF; /*set number flag to off*/
			}
			/*check if number detected*/
			else if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
				continue;
			} 
			else{
				/*print error of unexpected character, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if white space detected*/
		if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
			index++; /*increment unchecked code index*/
			if(dollarCount>=numDollars){
				expectCommaFlag = FLAGON; /*set expect comma flag to on*/
			}
		}
		else if((expectCommaFlag == FLAGON) && (ptrCode[index] != ',')){
			/*print error of missing comma, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE11,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		} 
		else if(endLineFlag == FLAGON){
			/*print error of extraneous character at end of line, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if((dollarCount == 0) && (ptrCode[index] != '$')){
			/*print error of expected dollar sign, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE5,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		/*check if comma detected*/
		else if(ptrCode[index] == ','){
			expectCommaFlag = FLAGOFF; /*set expected comma flag to off*/
			if(dollarCount>=numDollars+REGISTER){
				/*print error of unexpected comma, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE8,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			/*if comma detected and comma flag set to off, set comma flag to on*/
			if(commaFlag == FLAGOFF){
				commaFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			/*check if comma previously detected*/
			else if(commaFlag == FLAGON){
				/*print error of consecutive commas, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		/*check if dollar sign detected*/
		else if(ptrCode[index] == '$'){
			dollarFlag = FLAGON; /*set dollar flag to on*/
			dollarCount++; /*increment dolar count*/
			ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
			index++; /*increment unchecked code index*/
			newIndex++; /*increment checked code index*/
		}
		else{
			/*print error of unexpected character, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
	}
	/*check if dollar previously detected*/
	if(dollarFlag == FLAGON){
		/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(dollarCount<numDollars){
		/*print error of missing register, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE12,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(labelFlag == FLAGON){
		/*print error of missing label, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE13,lineNumber,"",filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;				
	}
	ptrCodeChecked[newIndex] = '\0'; /*set final index of checked code to null terminator*/
	return ptrCodeChecked; /*return checked code*/
}


/*The checkCommandjump function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "jmp" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function achieves this with the assumption that the jmp command is followed by either a label or a register. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandjump(char *ptrCode,int lineNumber,char *filename){
	int index,newIndex,labelFlag;
	char *ptrCodeChecked; /*pointer to char to store checked code*/

	index = STARTINDEX; /*set index of code to be checked to zero*/
	newIndex = STARTINDEX; /*set index of checked code to zero*/
	labelFlag = FLAGOFF; /*set label flag to off*/

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store checked code*/

	/*while white space detected*/
	while(ptrCode[index] == '\t' ||	ptrCode[index] == ' '){
		index++; /*increment unchecked code index*/
	}
		
	/*check if dollar sign detected*/
	if(ptrCode[index] == '$'){
		ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
		index++; /*increment unchecked code index*/
		newIndex++; /*increment checked code index*/
		/*check if number detected*/
		if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
			ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
			index++; /*increment unchecked code index*/
			newIndex++; /*increment checked code index*/
		}
		else{
			/*print error of expected number after dollar sign, free ptrCodeChecked and return NULL*/
			errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		/*while number detected*/
		while(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
			ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
			index++; /*increment unchecked code index*/
			newIndex++; /*increment checked code index*/
		}
		/*while end of code to be checked and new line character not reached*/
		while(ptrCode[index] != '\0' && ptrCode[index] != '\n'){
			/*check if white space detected*/
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			else{
				/*print error of extraneous character at end of line, free ptrCodeChecked and return NULL*/
				errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
	}
	/*check if alphabetical letter lowercase or uppercase*/
	else if((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index]<= 'Z')){
		ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
		index++; /*increment unchecked code index*/
		newIndex++; /*increment checked code index*/
		labelFlag = FLAGON; /*set label flag to on*/
		while(((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index] <= 'Z') || (ptrCode[index] >= '0' && ptrCode[index] <= '9'))){
			ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
			index++; /*increment unchecked code index*/
			newIndex++; /*increment checked code index*/
		}
		/*while end of code to be checked and new line character not reached*/
		while(ptrCode[index] != '\0' && ptrCode[index] != '\n'){
			/*check if white space detected*/
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				labelFlag = FLAGOFF; /*set label flag to off*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			else{
				if(labelFlag == FLAGON){
					/*print error of label must be composed of alphabetical letters and numbers, free ptrCodeChecked and return NULL*/
					errorMsg(ERRORTYPE2,lineNumber,"",filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else if(labelFlag == FLAGOFF){
					/*print error of extraneous character at end of line, free ptrCodeChecked and return NULL*/
					errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
			}
		}
	}
	else{
		/*print error of expected register or (legal) label after jmp command, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE14,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;	
	}

	ptrCodeChecked[newIndex] = '\0'; /*set final index of checked code to null terminator*/
	return ptrCodeChecked; /*return checked code*/
}


/*The checkCommandlaOrcall function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "la", "call" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function achieves this with the assumption that these types of commands are followed by a label. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandlaOrcall(char *ptrCode,int lineNumber,char *filename){
	int index,newIndex,labelFlag;
	char *ptrCodeChecked; /*pointer to char to store checked code*/

	index = STARTINDEX; /*set index of code to be checked to zero*/
	newIndex = STARTINDEX; /*set index of checked code to zero*/
	labelFlag = FLAGOFF; /*set label flag to flag off*/

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store checked code*/

	/*while white space detected*/
	while(ptrCode[index] == '\t' ||	ptrCode[index] == ' '){
		index++; /*increment unchecked code index*/
	}	


	if((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index]<= 'Z')){
		ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
		index++; /*increment unchecked code index*/
		newIndex++; /*increment checked code index*/
		labelFlag = FLAGON;
		while(((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index] <= 'Z') || (ptrCode[index] >= '0' && ptrCode[index] <= '9'))){
			ptrCodeChecked[newIndex] = ptrCode[index]; /*set value in current index of checked code to value in current index of unchecked code*/
			index++; /*increment unchecked code index*/
			newIndex++; /*increment checked code index*/
		}
		/*while end of code to be checked and new line character not reached*/
		while(ptrCode[index] != '\0' && ptrCode[index] != '\n'){
			/*check if white space detected*/
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				labelFlag = FLAGOFF; /*set label flag to flag off*/
				index++; /*increment unchecked code index*/
				newIndex++; /*increment checked code index*/
			}
			else{
				if(labelFlag == FLAGON){
					/*print error of label must be composed of alphabetical letters and numbers, free ptrCodeChecked and return NULL*/
					errorMsg(ERRORTYPE2,lineNumber,"",filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else if(labelFlag == FLAGOFF){
					/*print error of extraneous character at end of line, free ptrCodeChecked and return NULL*/
					errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
			}
		}
	}
	else{
		/*print error of expected (legal) label after la or call command, free ptrCodeChecked and return NULL*/
		errorMsg(ERRORTYPE15,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;	
	}

	ptrCodeChecked[newIndex] = '\0'; /*set final index of checked code to null terminator*/
	return ptrCodeChecked; /*return checked code*/
}





