#include "main.h"

static char *directives[]={".db",".dw", ".dh", ".asciz"};

enum Attributes {EMPTY,CODE,DATA,ENTRY,EXTERNAL};

void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag){
	static int IC,DC,step,errorFlag;
	int i,directiveFlag,endWhileFlag;
	TABLE_NODE_T* tableHead;
	/*enum Attributes Attribute;*/
	step = 1;
	errorFlag = FLAGOFF;
	directiveFlag = FLAGOFF;
	endWhileFlag = FLAGOFF;
	/*printf("%s, %s, %s\n",ptrField1,ptrField2,ptrField3);*/

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
					tableHead = symbolTable(ptrField1,DC,DATA,EMPTY);
				}
				step = 8;
				break;
			case 8:
				step = 2;
				endWhileFlag = FLAGON;
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
				tableHead = symbolTable(ptrField2,0,EXTERNAL,EMPTY);
				step = 2;
				endWhileFlag = FLAGON;
				break;
			case 12:
				if(labelFlag == FLAGON){
					tableHead = symbolTable(ptrField1,IC,CODE,EMPTY);
				}
				step = 13;
				break;
			case 13:
				step = 14;
				break;
			case 14:
				step = 15;
				break;
			case 15:
				step = 16;
				break;
			case 16:
				IC = IC + 4;
				step = 2;
				endWhileFlag = FLAGON;
				break;
			case 17:
				if(errorFlag == FLAGON){
					endWhileFlag = FLAGON;
				}
				else{
					step = 18;
				}
				break;
			case 18:
				step = 19;
				break;
			case 19:
				step = 20;
				break;
			case 20:
				step = 21;
				break;
			case 21:
				endWhileFlag = FLAGON;
				break;
	
				
		}
		if(endWhileFlag == FLAGON){
			break;
		}

	}
			

}
