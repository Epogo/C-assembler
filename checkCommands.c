#include "assembler.h"


static char *Rcommands1[]={"add","sub", "and", "or", "nor"};

static char *Rcommands2[]={"move", "mvhi","mvlo"};

static char *Icommands1[]={"addi", "subi", "andi", "ori","nori","lb", "sb", "lw", "sw", "lh","sh"};

static char *Icommands2[]={"bne", "beq", "blt", "bgt"};



enum comType {Rcom1,Rcom2,Icom1,Icom2,jumpCom,laOrcallCom};



char* checkCommand(char *ptrCode,char *ptrCommand,int lineNumber,char *filename){
	int i,currentComType;
	char *ptrCodeChecked;

	for(i=0;i<5;i++){
		if(!strcmp(Rcommands1[i], ptrCommand)){
			currentComType = Rcom1;
		}
	}
	for(i=0;i<3;i++){
		if(!strcmp(Rcommands2[i], ptrCommand)){
			currentComType = Rcom2;
		}
	}
	for(i=0;i<11;i++){
		if(!strcmp(Icommands1[i], ptrCommand)){
			currentComType = Icom1;
		}
	}
	for(i=0;i<4;i++){
		if(!strcmp(Icommands2[i], ptrCommand)){
			currentComType = Icom2;
		}
	}
	if(!strcmp("jmp",ptrCommand)){
		currentComType = jumpCom;
	}
	else if(!strcmp("la",ptrCommand) || !strcmp("call",ptrCommand)){
		currentComType = laOrcallCom;
	}


	if(currentComType == Rcom1){
		ptrCodeChecked = checkCommandR1(ptrCode,lineNumber,filename);
	}
	else if(currentComType == Rcom2){
		ptrCodeChecked = checkCommandR2(ptrCode,lineNumber,filename);
	}
	else if(currentComType == Icom1){
		ptrCodeChecked = checkCommandI1(ptrCode,lineNumber,filename);
	}
	else if(currentComType == Icom2){
		ptrCodeChecked = checkCommandI2(ptrCode,lineNumber,filename);
	}
	else if(currentComType == jumpCom){
		ptrCodeChecked = checkCommandjump(ptrCode,lineNumber,filename);
	}
	else if(currentComType == laOrcallCom){
		ptrCodeChecked = checkCommandlaOrcall(ptrCode,lineNumber,filename);
	}
	return ptrCodeChecked;

}


char* checkCommandR1(char *ptrCode,int lineNumber,char *filename){

	int dollarCount, dollarFlag,index,newIndex,commaFlag,numberFlag,endLineFlag,expectCommaFlag,numDollars;
	char *ptrCodeChecked;

	index = 0;
	newIndex = 0;
	dollarCount = 0;
	numDollars = 3;
	dollarFlag = FLAGOFF;
	commaFlag = FLAGOFF;
	endLineFlag = FLAGOFF;
	numberFlag = FLAGOFF;
	expectCommaFlag = FLAGOFF;

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char));

	while(ptrCode[index] != '\0'){
		if(commaFlag == FLAGON){
			if(!((ptrCode[index] == '\t') || (ptrCode[index] == ' '))){
				if(ptrCode[index] == ','){
					errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else{
					commaFlag = FLAGOFF;
				}
			}
		}
		if(dollarFlag == FLAGON){
			if(!(ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			else{
				dollarFlag = FLAGOFF;
				numberFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				continue;
			}
		}
		if(numberFlag == FLAGON){
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				numberFlag = FLAGOFF;
				if(dollarCount<numDollars){
					expectCommaFlag = FLAGON;
				}
			}
			else if(ptrCode[index] == ','){
				numberFlag = FLAGOFF;
			}
			else if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				continue;
			} 
			else{
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
			index++;
			if(dollarCount>=numDollars){
				endLineFlag = FLAGON;
			}
		}
		else if((expectCommaFlag == FLAGON) && (ptrCode[index] != ',')){
			errorMsg(ERRORTYPE11,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		} 
		else if(endLineFlag == FLAGON){
			errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if((dollarCount == 0) && (ptrCode[index] != '$')){
			errorMsg(ERRORTYPE5,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if(ptrCode[index] == ','){
			expectCommaFlag = FLAGOFF;
			if(dollarCount>=numDollars){
				errorMsg(ERRORTYPE8,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			if(commaFlag == FLAGOFF){
				commaFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			else if(commaFlag == FLAGON){
				errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		else if(ptrCode[index] == '$'){
			dollarFlag = FLAGON;
			dollarCount++;
			ptrCodeChecked[newIndex] = ptrCode[index];
			index++;
			newIndex++;
		}
		else{
			errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
	}
	if(dollarFlag == FLAGON){
		errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(dollarCount<numDollars){
		errorMsg(ERRORTYPE12,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	ptrCodeChecked[newIndex] = '\0';
	return ptrCodeChecked;
}


char* checkCommandR2(char *ptrCode,int lineNumber,char *filename){

	int dollarCount, dollarFlag,index,newIndex,commaFlag,numberFlag,endLineFlag,expectCommaFlag,numDollars;
	char *ptrCodeChecked;

	index = 0;
	newIndex = 0;
	dollarCount = 0;
	numDollars = 2;
	dollarFlag = FLAGOFF;
	commaFlag = FLAGOFF;
	endLineFlag = FLAGOFF;
	numberFlag = FLAGOFF;
	expectCommaFlag = FLAGOFF;

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char));

	while(ptrCode[index] != '\0'){
		if(commaFlag == FLAGON){
			if(!((ptrCode[index] == '\t') || (ptrCode[index] == ' '))){
				if(ptrCode[index] == ','){
					errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else{
					commaFlag = FLAGOFF;
				}
			}
		}
		if(dollarFlag == FLAGON){
			if(!(ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			else{
				dollarFlag = FLAGOFF;
				numberFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				continue;
			}
		}
		if(numberFlag == FLAGON){
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				numberFlag = FLAGOFF;
				if(dollarCount<numDollars){
					expectCommaFlag = FLAGON;
				}
			}
			else if(ptrCode[index] == ','){
				numberFlag = FLAGOFF;
			}
			else if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				continue;
			} 
			else{
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
			index++;
			if(dollarCount>=numDollars){
				endLineFlag = FLAGON;
			}
		}
		else if((expectCommaFlag == FLAGON) && (ptrCode[index] != ',')){
			errorMsg(ERRORTYPE11,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		} 
		else if(endLineFlag == FLAGON){
			errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if((dollarCount == 0) && (ptrCode[index] != '$')){
			errorMsg(ERRORTYPE5,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if(ptrCode[index] == ','){
			expectCommaFlag = FLAGOFF;
			if(dollarCount>=numDollars){
				errorMsg(ERRORTYPE8,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			if(commaFlag == FLAGOFF){
				commaFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			else if(commaFlag == FLAGON){
				errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		else if(ptrCode[index] == '$'){
			dollarFlag = FLAGON;
			dollarCount++;
			ptrCodeChecked[newIndex] = ptrCode[index];
			index++;
			newIndex++;
		}
		else{
			errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
	}
	if(dollarFlag == FLAGON){
		errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(dollarCount<numDollars){
		errorMsg(ERRORTYPE12,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	ptrCodeChecked[newIndex] = '\0';
	return ptrCodeChecked;
}


char* checkCommandI1(char *ptrCode,int lineNumber,char *filename){

	int dollarCount, dollarFlag,index,newIndex,commaFlag,numberFlag,endLineFlag,expectCommaFlag,numDollars,immediateFlag;
	char *ptrCodeChecked;

	index = 0;
	newIndex = 0;
	dollarCount = 0;
	numDollars = 2;
	dollarFlag = FLAGOFF;
	commaFlag = FLAGOFF;
	endLineFlag = FLAGOFF;
	numberFlag = FLAGOFF;
	expectCommaFlag = FLAGOFF;
	immediateFlag = FLAGON;

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char));

	while(ptrCode[index] != '\0'){
		if((dollarCount == 1) && (commaFlag == FLAGON) && (immediateFlag == FLAGON)){
			immediateFlag = FLAGOFF;
			commaFlag = FLAGOFF;
			while((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				index++;
			}
			if((ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			else if((ptrCode[index] == '-' ) && (ptrCode[index+1] >= '0' && ptrCode[index+1] <= '9')){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			else{
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			while(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			if(ptrCode[index] == ','){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				commaFlag = FLAGON;
				continue;
			}
			else if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				expectCommaFlag = FLAGON;
				index++;
				newIndex++;
				continue;
			}
			else{
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		if(commaFlag == FLAGON){
			if(!((ptrCode[index] == '\t') || (ptrCode[index] == ' '))){
				if(ptrCode[index] == ','){
					errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else{
					commaFlag = FLAGOFF;
				}
			}
		}
		if(dollarFlag == FLAGON){
			if(!(ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			else{
				dollarFlag = FLAGOFF;
				numberFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				continue;
			}
		}
		if(numberFlag == FLAGON){
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				numberFlag = FLAGOFF;
				if(dollarCount<numDollars){
					expectCommaFlag = FLAGON;
				}
			}
			else if(ptrCode[index] == ','){
				numberFlag = FLAGOFF;
			}
			else if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				continue;
			} 
			else{
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
			index++;
			if(dollarCount>=numDollars){
				endLineFlag = FLAGON;
			}
		}
		else if((expectCommaFlag == FLAGON) && (ptrCode[index] != ',')){
			errorMsg(ERRORTYPE11,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		} 
		else if(endLineFlag == FLAGON){
			errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if((dollarCount == 0) && (ptrCode[index] != '$')){
			errorMsg(ERRORTYPE5,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if(ptrCode[index] == ','){
			expectCommaFlag = FLAGOFF;
			if(dollarCount>=numDollars){
				errorMsg(ERRORTYPE8,lineNumber,NULL,filename);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			if(commaFlag == FLAGOFF){
				commaFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			else if(commaFlag == FLAGON){
				errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		else if(ptrCode[index] == '$'){
			dollarFlag = FLAGON;
			dollarCount++;
			ptrCodeChecked[newIndex] = ptrCode[index];
			index++;
			newIndex++;
		}
		else{
			errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
	}
	if(dollarFlag == FLAGON){
		errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(dollarCount<numDollars){
		errorMsg(ERRORTYPE12,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	ptrCodeChecked[newIndex] = '\0';
	return ptrCodeChecked;
}


char* checkCommandI2(char *ptrCode,int lineNumber,char *filename){

	int dollarCount, dollarFlag,index,newIndex,commaFlag,numberFlag,endLineFlag,expectCommaFlag,numDollars,labelFlag;
	char *ptrCodeChecked;

	index = 0;
	newIndex = 0;
	dollarCount = 0;
	numDollars = 2;
	dollarFlag = FLAGOFF;
	commaFlag = FLAGOFF;
	endLineFlag = FLAGOFF;
	numberFlag = FLAGOFF;
	expectCommaFlag = FLAGOFF;
	labelFlag = FLAGON;

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char));

	while(ptrCode[index] != '\0'){
		if((dollarCount == numDollars) && (commaFlag == FLAGON) && (labelFlag == FLAGON)){
			commaFlag = FLAGOFF;
			while((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				index++;
			}
			if((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index]<= 'Z')){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			else{
				errorMsg(ERRORTYPE1,lineNumber,"",filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			while(((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index] <= 'Z') || (ptrCode[index] >= '0' && ptrCode[index] <= '9'))){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			while(ptrCode[index] != '\0'){
				if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
					labelFlag = FLAGOFF;
					index++;
					newIndex++;
				}
				else{
					if(labelFlag == FLAGON){
						errorMsg(ERRORTYPE2,lineNumber,"",filename);
						free(ptrCodeChecked);
						ptrCodeChecked = NULL;
						return ptrCodeChecked;
					}
					else if(labelFlag == FLAGOFF){
						errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
						free(ptrCodeChecked);
						ptrCodeChecked = NULL;
						return ptrCodeChecked;
					}
				}
			}
			return ptrCodeChecked;
		}
		if(commaFlag == FLAGON){
			if(!((ptrCode[index] == '\t') || (ptrCode[index] == ' '))){
				if(ptrCode[index] == ','){
					errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else{
					commaFlag = FLAGOFF;
				}
			}
		}
		if(dollarFlag == FLAGON){
			if(!(ptrCode[index] >= '0' && ptrCode[index] <= '9')){
				errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			else{
				dollarFlag = FLAGOFF;
				numberFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				continue;
			}
		}
		if(numberFlag == FLAGON){
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				numberFlag = FLAGOFF;
				if(dollarCount<numDollars){
					expectCommaFlag = FLAGON;
				}
			}
			else if(ptrCode[index] == ','){
				numberFlag = FLAGOFF;
			}
			else if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
				continue;
			} 
			else{
				errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
			index++;
			if(dollarCount>=numDollars){
				expectCommaFlag = FLAGON;
			}
		}
		else if((expectCommaFlag == FLAGON) && (ptrCode[index] != ',')){
			errorMsg(ERRORTYPE11,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		} 
		else if(endLineFlag == FLAGON){
			errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if((dollarCount == 0) && (ptrCode[index] != '$')){
			errorMsg(ERRORTYPE5,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		else if(ptrCode[index] == ','){
			expectCommaFlag = FLAGOFF;
			if(dollarCount>=numDollars+1){
				errorMsg(ERRORTYPE8,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
			if(commaFlag == FLAGOFF){
				commaFlag = FLAGON;
				ptrCodeChecked[newIndex] = ptrCode[index];
				index++;
				newIndex++;
			}
			else if(commaFlag == FLAGON){
				errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
		else if(ptrCode[index] == '$'){
			dollarFlag = FLAGON;
			dollarCount++;
			ptrCodeChecked[newIndex] = ptrCode[index];
			index++;
			newIndex++;
		}
		else{
			errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
	}
	if(dollarFlag == FLAGON){
		errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(dollarCount<numDollars){
		errorMsg(ERRORTYPE12,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;
	}
	if(labelFlag == FLAGON){
		errorMsg(ERRORTYPE13,lineNumber,"",filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;				
	}
	ptrCodeChecked[newIndex] = '\0';
	return ptrCodeChecked;
}


char* checkCommandjump(char *ptrCode,int lineNumber,char *filename){
	int index,newIndex,labelFlag;
	char *ptrCodeChecked;

	index = 0;
	newIndex = 0;
	labelFlag = FLAGOFF;

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char));

	while(ptrCode[index] == '\t' ||	ptrCode[index] == ' '){
		index++;
	}
		
	if(ptrCode[index] == '$'){
		ptrCodeChecked[newIndex] = ptrCode[index];
		index++;
		newIndex++;
		if(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
			ptrCodeChecked[newIndex] = ptrCode[index];
			index++;
			newIndex++;
		}
		else{
			errorMsg(ERRORTYPE7,lineNumber,NULL,filename);
			free(ptrCodeChecked);
			ptrCodeChecked = NULL;
			return ptrCodeChecked;
		}
		while(ptrCode[index] >= '0' && ptrCode[index] <= '9'){
			ptrCodeChecked[newIndex] = ptrCode[index];
			index++;
			newIndex++;
		}
		while(ptrCode[index] != '\0' && ptrCode[index] != '\n'){
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				index++;
				newIndex++;
			}
			else{
				errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
				free(ptrCodeChecked);
				ptrCodeChecked = NULL;
				return ptrCodeChecked;
			}
		}
	}
	else if((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index]<= 'Z')){
		ptrCodeChecked[newIndex] = ptrCode[index];
		index++;
		newIndex++;
		labelFlag = FLAGON;
		while(((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index] <= 'Z') || (ptrCode[index] >= '0' && ptrCode[index] <= '9'))){
			ptrCodeChecked[newIndex] = ptrCode[index];
			index++;
			newIndex++;
		}
		while(ptrCode[index] != '\0' && ptrCode[index] != '\n'){
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				labelFlag = FLAGOFF;
				index++;
				newIndex++;
			}
			else{
				if(labelFlag == FLAGON){
					errorMsg(ERRORTYPE2,lineNumber,"",filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else if(labelFlag == FLAGOFF){
					errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
			}
		}
	}
	else{
		errorMsg(ERRORTYPE14,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;	
	}

	ptrCodeChecked[newIndex] = '\0';
	return ptrCodeChecked;
}


char* checkCommandlaOrcall(char *ptrCode,int lineNumber,char *filename){
	int index,newIndex,labelFlag;
	char *ptrCodeChecked;

	index = 0;
	newIndex = 0;
	labelFlag = FLAGOFF;

	ptrCodeChecked = calloc(MAXLINELEN,sizeof(char));

	while(ptrCode[index] == '\t' ||	ptrCode[index] == ' '){
		index++;
	}	


	if((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index]<= 'Z')){
		ptrCodeChecked[newIndex] = ptrCode[index];
		index++;
		newIndex++;
		labelFlag = FLAGON;
		while(((ptrCode[index] >= 'a' && ptrCode[index] <= 'z') || (ptrCode[index] >= 'A' && ptrCode[index] <= 'Z') || (ptrCode[index] >= '0' && ptrCode[index] <= '9'))){
			ptrCodeChecked[newIndex] = ptrCode[index];
			index++;
			newIndex++;
		}
		while(ptrCode[index] != '\0' && ptrCode[index] != '\n'){
			if((ptrCode[index] == '\t') || (ptrCode[index] == ' ')){
				labelFlag = FLAGOFF;
				index++;
				newIndex++;
			}
			else{
				if(labelFlag == FLAGON){
					errorMsg(ERRORTYPE2,lineNumber,"",filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
				else if(labelFlag == FLAGOFF){
					errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
					free(ptrCodeChecked);
					ptrCodeChecked = NULL;
					return ptrCodeChecked;
				}
			}
		}
	}
	else{
		errorMsg(ERRORTYPE15,lineNumber,NULL,filename);
		free(ptrCodeChecked);
		ptrCodeChecked = NULL;
		return ptrCodeChecked;	
	}

	ptrCodeChecked[newIndex] = '\0';
	return ptrCodeChecked;
}





