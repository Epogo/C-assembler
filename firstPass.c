#include "main.h"

static char *directives[]={".db",".dw", ".dh", ".asciz"};

void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag){
	static int IC,DC,step;
	int i,directiveFlag,endWhileFlag;
	step = 1;
	directiveFlag = FLAGOFF;
	endWhileFlag = FLAGOFF;
	/*printf("%s\n",ptrField1);
	printf("%s\n",ptrField2);
	printf("%s\n",ptrField3);*/

	while(1){
		switch(step){
			case 1:
				DC = 0;
				IC = 100;
				step = 2;
				break;
			case 2:
				step = 6;
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				if(labelFlag == FLAGON){
					for(i=0;i<NUMDIRECTIVES;i++){
						if(!strcmp(directives[i], ptrField2)){
							directiveFlag = FLAGON;
							
						}
					}
				}
				else if(labelFlag == FLAGOFF){
					for(i=0;i<NUMDIRECTIVES;i++){
						if(!strcmp(directives[i], ptrField1)){
							directiveFlag = FLAGON;
						}
					}
				}
				if(directiveFlag == FLAGON){
					step = 7;
					directiveFlag = FLAGOFF;
				}
				else if(directiveFlag == FLAGOFF){
					step = 9;
				}				
				break;
			case 7:
				if(labelFlag == FLAGON){
					/*symbolTable(ptrField1,DC,1);*/
				}
				step = 8;
				break;
			case 8:
				step = 9;
				break;
			case 9:
				if(!strcmp(".extern", ptrField1) || !strcmp(".entry", ptrField1)){
					step = 10;
				}
				else{
					step = 12;
				}
				break;
			case 10:
				if(!strcmp(".entry", ptrField1)){
					step = 2;
					endWhileFlag = FLAGON;
				}
				else{
					step = 11;
				}
				break;
			case 11:
				break;
			case 12:
				break;
				
		}
		if(step == 11 || step == 12)
			break;
		if(endWhileFlag == FLAGON){
			break;
		}

	}
			

}
