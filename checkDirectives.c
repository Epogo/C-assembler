#include "assembler.h"


char* checkData(char *ptrData,char *ptrDirective,int lineNumber,char *filename){
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
					errorMsg(ERRORTYPE17,lineNumber,NULL,filename);
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
					/*printf("My Data: %c\n",ptrData[index]);*/
					errorMsg(ERRORTYPE9,lineNumber,NULL,filename);
					free(ptrDataChecked);
					ptrDataChecked = NULL;
					return ptrDataChecked;
				}
			}
			ptrDataChecked[newIndex] = '\0';
			return ptrDataChecked;
		}
		else{
			errorMsg(ERRORTYPE16,lineNumber,NULL,filename);
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
			errorMsg(ERRORTYPE22,lineNumber,NULL,filename);
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
						errorMsg(ERRORTYPE19,lineNumber,NULL,filename);
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
						errorMsg(ERRORTYPE10,lineNumber,NULL,filename);
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
						errorMsg(ERRORTYPE6,lineNumber,NULL,filename);
						free(ptrDataChecked);
						ptrDataChecked = NULL;
						return ptrDataChecked;
					}
					else if(ptrData[index] == '\0' || ptrData[index] == '\n'){
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
				errorMsg(ERRORTYPE18,lineNumber,NULL,filename);
				free(ptrDataChecked);
				ptrDataChecked = NULL;
				return ptrDataChecked;
			} 
		}

	}

	ptrDataChecked[newIndex] = '\0';
	return ptrDataChecked;

}



