#include "assembler.h"


char* checkData(char *ptrData,char *ptrDirective,int lineNumber){
	char *ptrDataChecked;
	int index,newIndex;

	index = 0;
	newIndex = 0;

	ptrDataChecked = calloc(MAXLINELEN,sizeof(char));

	if(!strcmp(".asciz", ptrDirective)){
		while((ptrData[index] == '\t') || (ptrData[index] == ' ')){
			index++;
		}
		if(ptrData[index] == '"'){
			index++;
			while(ptrData[index] != '"'){
				if(ptrData[index] == '\0' || ptrData[index] == '\n'){
					errorMsg(17,lineNumber,NULL);
					free(ptrDataChecked);
					ptrDataChecked = NULL;
					return ptrDataChecked;
				}
				else{
					ptrDataChecked[newIndex] = ptrData[index];
					index++;
					newIndex++;
				}
			}
			index++;
			while(ptrData[index] != '\0' && ptrData[index] != '\n'){
				if((ptrData[index] == '\t') || (ptrData[index] == ' ')){
					index++;
				}
				else{
					printf("My Data: %c\n",ptrData[index]);
					errorMsg(9,lineNumber,NULL);
					free(ptrDataChecked);
					ptrDataChecked = NULL;
					return ptrDataChecked;
				}
			}
			ptrDataChecked[newIndex] = '\0';
			return ptrDataChecked;
		}
		else{
			errorMsg(16,lineNumber,NULL);
			free(ptrDataChecked);
			ptrDataChecked = NULL;
			return ptrDataChecked;
		}
	}
	else if(!strcmp(".dh", ptrDirective) || !strcmp(".dw", ptrDirective) || !strcmp(".db", ptrDirective)){
		while((ptrData[index] == '\t') || (ptrData[index] == ' ')){
			index++;
		}
		if(ptrData[index] == '\0' || ptrData[index] == '\n'){
			errorMsg(22,lineNumber,NULL);
			free(ptrDataChecked);
			ptrDataChecked = NULL;
			return ptrDataChecked;
		}
		while(ptrData[index] != '\0' && ptrData[index] != '\n'){
			if((ptrData[index] >= '0' && ptrData[index] <= '9') || ptrData[index] == '-' || ptrData[index] == '+'){
				ptrDataChecked[newIndex] = ptrData[index];
				newIndex++;
				index++;
				if(ptrData[index-1] == '-' || ptrData[index-1] == '+'){
					if(!(ptrData[index] >= '0' && ptrData[index] <= '9')){
						errorMsg(19,lineNumber,NULL);
						free(ptrDataChecked);
						ptrDataChecked = NULL;
						return ptrDataChecked;
					}
				}
				while((ptrData[index] >= '0' && ptrData[index] <= '9')){
					ptrDataChecked[newIndex] = ptrData[index];
					index++;
					newIndex++;
				}
				while(1){
					if((ptrData[index] == '\t') || (ptrData[index] == ' ')){
						index++;
					}
					else if(ptrData[index] == ','){
						ptrDataChecked[newIndex] = ptrData[index];
						index++;
						newIndex++;
						break;
					}
					else if(ptrData[index] == '\0' || ptrData[index] == '\n'){
						ptrDataChecked[newIndex] = '\0';
						return ptrDataChecked;
					}
					else{
						errorMsg(10,lineNumber,NULL);
						free(ptrDataChecked);
						ptrDataChecked = NULL;
						return ptrDataChecked;
					}
				}
				while(1){
					if((ptrData[index] == '\t') || (ptrData[index] == ' ')){
						index++;
					}
					else if(ptrData[index] == ','){
						errorMsg(6,lineNumber,NULL);
						free(ptrDataChecked);
						ptrDataChecked = NULL;
						return ptrDataChecked;
					}
					else if(ptrData[index] == '\0' || ptrData[index] == '\n'){
						errorMsg(9,lineNumber,NULL);
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
				errorMsg(18,lineNumber,NULL);
				free(ptrDataChecked);
				ptrDataChecked = NULL;
				return ptrDataChecked;
			} 
		}

	}

	ptrDataChecked[newIndex] = '\0';
	return ptrDataChecked;

}



