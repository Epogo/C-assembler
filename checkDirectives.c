#include "assembler.h" /*include assembler.h header file*/


char* checkData(char *ptrData,char *ptrDirective,int lineNumber,char *filename){
	char *ptrDataChecked; /*pointer to char to store checked data*/
	int index,newIndex;

	index = STARTINDEX;  /*set index of data to be checked to zero*/
	newIndex = STARTINDEX; /*set index of checked data to zero*/

	ptrDataChecked = calloc(MAXLINELEN,sizeof(char)); /*allocate memory to store checked data*/

	/*check if directive is .asciz*/
	if(!strcmp(".asciz", ptrDirective)){
		/*while white space detected*/
		while((ptrData[index] == '\t') || (ptrData[index] == ' ')){
			index++; /*increment index of data to be checked*/
		}
		/*check if quotation marks detected*/
		if(ptrData[index] == '"'){
			index++; /*increment index of data to be checked*/
			/*while additional quotation marks not detected*/
			while(ptrData[index] != '"'){
				/*if not end of line or new line*/
				if(ptrData[index] == '\0' || ptrData[index] == '\n'){
					/*print error of expected addtional quotation marks to close initial quotation marks, free ptrDataChecked and return NULL*/
					errorMsg(ERRORTYPE17,lineNumber,NULL,filename);
					free(ptrDataChecked);
					ptrDataChecked = NULL;
					return ptrDataChecked;
				}
				else{
					ptrDataChecked[newIndex] = ptrData[index]; /*set value of current index of checked data to value of current index of data to be checked*/
					index++; /*increment index of data to be checked*/
					newIndex++; /*increment index of checked data*/
				}
			}
			index++; /*increment index of data to be checked*/
			/*while not end of line and not new line*/
			while(ptrData[index] != '\0' && ptrData[index] != '\n'){
				if((ptrData[index] == '\t') || (ptrData[index] == ' ')){
					index++; /*increment index of data to be checked*/
				}
				else{
					/*print error of extraneous character at end of line, free ptrDataChecked and return NULL*/
					errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
					free(ptrDataChecked);
					ptrDataChecked = NULL;
					return ptrDataChecked;
				}
			}
			ptrDataChecked[newIndex] = '\0'; /*set final index of checked data to null terminator*/
			return ptrDataChecked; /*return checked data*/
		}
		else{
			/*print error of expected quotation marks after .asciz directive, free ptrDataChecked and return NULL*/
			errorMsg(ERRORTYPE16,lineNumber,NULL,filename);
			free(ptrDataChecked);
			ptrDataChecked = NULL;
			return ptrDataChecked;
		}
	}
	/*check if directive is .dh, .dw, or .db*/
	else if(!strcmp(".dh", ptrDirective) || !strcmp(".dw", ptrDirective) || !strcmp(".db", ptrDirective)){
		/*while white space detected*/
		while((ptrData[index] == '\t') || (ptrData[index] == ' ')){
			index++; /*increment index of data to be checked*/
		}
		/*check if end of line or new line*/
		if(ptrData[index] == '\0' || ptrData[index] == '\n'){
			/*print error of line incomplete, free ptrDataChecked and return NULL*/
			errorMsg(ERRORTYPE22,lineNumber,NULL,filename);
			free(ptrDataChecked);
			ptrDataChecked = NULL;
			return ptrDataChecked;
		}
		/*while not end of line and not new line*/
		while(ptrData[index] != '\0' && ptrData[index] != '\n'){
			if((ptrData[index] >= '0' && ptrData[index] <= '9') || ptrData[index] == '-' || ptrData[index] == '+'){
				ptrDataChecked[newIndex] = ptrData[index]; /*set value of current index of checked data to value of current index of data to be checked*/
				newIndex++; /*increment index of checked data*/
				index++; /*increment index of data to be checked*/
				/*check if previous index was a + or -*/
				if(ptrData[index-PREVINDEX] == '-' || ptrData[index-PREVINDEX] == '+'){
					if(!(ptrData[index] >= '0' && ptrData[index] <= '9')){
						/*print error of expected number after +/- sign, free ptrDataChecked and return NULL*/
						errorMsg(ERRORTYPE19,lineNumber,NULL,filename);
						free(ptrDataChecked);
						ptrDataChecked = NULL;
						return ptrDataChecked;
					}
				}
				/*while number detected*/
				while((ptrData[index] >= '0' && ptrData[index] <= '9')){
					ptrDataChecked[newIndex] = ptrData[index]; /*set value of current index of checked data to value of current index of data to be checked*/
					index++; /*increment index of data to be checked*/
					newIndex++; /*increment index of checked data*/
				}
				while(1){
					/*check if white space detected*/
					if((ptrData[index] == '\t') || (ptrData[index] == ' ')){
						index++; /*increment index of data to be checked*/
					}
					/*check if comma detected*/
					else if(ptrData[index] == ','){
						ptrDataChecked[newIndex] = ptrData[index]; /*set value of current index of checked data to value of current index of data to be checked*/
						index++; /*increment index of data to be checked*/
						newIndex++; /*increment index of checked data*/
						break;
					}
					/*check if end of line or new line character detected*/
					else if(ptrData[index] == '\0' || ptrData[index] == '\n'){
						ptrDataChecked[newIndex] = '\0'; /*set final index of checked data to null terminator*/
						return ptrDataChecked; /*return checked data*/
					}
					else{
						/*print error of unexpected character, free ptrDataChecked and return NULL*/
						errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
						free(ptrDataChecked);
						ptrDataChecked = NULL;
						return ptrDataChecked;
					}
				}
				while(1){
					/*check if white space detected*/
					if((ptrData[index] == '\t') || (ptrData[index] == ' ')){
						index++; /*increment index of data to be checked*/
					}
					/*check if comma detected*/
					else if(ptrData[index] == ','){
						/*print error of illegal consecutive commas, free ptrDataChecked and return NULL*/
						errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
						free(ptrDataChecked);
						ptrDataChecked = NULL;
						return ptrDataChecked;
					}
					/*check if end of line or new line character detected*/
					else if(ptrData[index] == '\0' || ptrData[index] == '\n'){
						/*print error of extraneous character at end of line, free ptrDataChecked and return NULL*/
						errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
						free(ptrDataChecked);
						ptrDataChecked = NULL;
						return ptrDataChecked;
					}
					else{
						break;
					}	
				}
			}
			else{
				/*print error of Expected number or + or - after .db/.dw/.dh directive, free ptrDataChecked and return NULL*/
				errorMsg(ERRORTYPE18,lineNumber,NULL,filename);
				free(ptrDataChecked);
				ptrDataChecked = NULL;
				return ptrDataChecked;
			} 
		}

	}

	ptrDataChecked[newIndex] = '\0'; /*set final index of checked data to null terminator*/
	return ptrDataChecked; /*return checked data*/

}



