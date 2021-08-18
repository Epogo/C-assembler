#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include <math.h>

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL};

/*MEMIM *memAdd(char*,char*,char*,TABLE_NODE_T*);
char *Registers(char*);
char *decToBin(int);
char *decToBinJ(int);
char *ascizToBin(int);
char *decToBinDirW(char*);
char *decToBinDirH(char*);
void deleteNode(MEMIM*);
void addNode(MEMIM *headCom,MEMIM *headData, MEMIM *node);
void printList(MEMIM *head);
char binToHex(char *bin);
void concatNodes(MEMIM *headCom,MEMIM *headData);
void printSymbolTable(TABLE_NODE_T *symbolTable);*/

char *rCommands[]={"add","sub","and","or","nor","move","mvhi","mvlo"};/*R commands*/
char *iCommands[]={"addi","subi","andi","ori","nori","bne","beq","blt","bgt","lb","sb","lw","sw","lh","sh"};/*I commands*/
char *jCommands[]={"jmp","la","call","stop"};/*J commands*/
char *directives[]={".db",".dw",".dh",".data",".asciz",".entry",".extern"};/*Directives*/
char *rComFunct[]={"00001","00010","00011","00100","00101"};/*A list of functs for r commands*/
char *rOpCode[]={"000000","000001"};/*A list of opcodes for r commands*/
char *iOpCode[]={"001010","001011","001100","001101","001110","001111","010000","010001","010010","010011","010100","010101","010110","010111","011000"};/*A list of opcodes for i commands*/
char *jOpCode[]={"011110","011111","100000","111111"};/*A list of opcodes for j commands*/

int binaryToDecimal(int n){
        int num = n;
        int decValue = 0;
 
        int base = 1;
        int temp = num;
        
        while (temp > 0) {
            int last_digit = temp % 10;
            temp = temp / 10;
 
            decValue += last_digit * base;
 
            base = base * 2;
        }
 
        return decValue;
}


MEMIM *memAdd(char *ptrField1,char *ptrField2,char *ptrField3){
    int count=0;/*A counter*/
    int comFlag=0;/*If a given command or direcrive has been found.*/
    int dataCounter=0;
    int i,j,k;
    char *lineStr;/*A pointer to a line*/
    const char s[2] = ",";/*A comma token*/
    char *token;/*A pointer to a token*/
    char *reg;/*A char pointer to a register*/
    char opString[NUM_OF_BITS_OP];/*Operation string*/
    /*char *imm=(char *)calloc(NUM_OF_BITS_IMM, sizeof(char));*//*Memory allocation for immediate value (16 bits).*/
    char *imm;
    /*char *immStop=(char *)calloc(NUM_OF_BITS_IMM_STOP, sizeof(char));*//*Memory allocation for immediate value (26 bits).*/
    char *immJ;
    /*char *immPointer;*/
    char *registers[3];/*An array of pointers of registers.*/
    char *opStrPoint;/*A pointer to a Operation string.*/
    char *binNum,*binNumStart;/*Binary number pointer& A pointer to the the first byte.*/
    char *notInUse="000000";/*An array of bin chars for "not in use" bits within the 32bits slot.*/
    char *emptyReg="00000";/*An array of bin chars for "empty register" bits within the 32bits slot.*/
    char *zero="0";/*A "zero" string.*/
    char *one="100000000000000000000";/*In case of J command with reg.*/
    char *null="00000000";/*NULL terminator.*/
    char *regPoint;
    DATA *temp;/*A Temporary pointer.*/

    /*MEMIM *node=(MEMIM *)malloc(sizeof(MEMIM));*//*Memory allocation for memory image node.*/
    MEMIM *node=(MEMIM *)calloc(1,sizeof(MEMIM));/*Memory allocation for memory image node.*/
    DATA *newNode;/*An adress of a new node.*/
    lineStr = (char *)calloc(NUM_OF_CHARS_IN_LINE, sizeof(char));/*Memory allocation for a line*/
    opStrPoint=opString;

    node->errorFlag=0;

    node->next = NULL;
    opString[0] = '\0';

    if (ptrField3)
        strcpy(lineStr,ptrField3);/*If the third field isn't empty-copy the content of this field to the lineStr field.*/
    token = strtok(lineStr, s);/*A definition of a token*/
    
    /*Check if the command is a R-COMMAND*/
    for(i=0;i<RCOMLEN;i++){
        if(!strcmp(ptrField2,rCommands[i])){
            comFlag=1;
	    node->p = NULL;
            while( token != NULL ) {
                    reg=Registers(token);/*Convert the registers tokens to a binary reg string.*/
		    regPoint = token;
		    regPoint++;
                    if (atoi(regPoint)<0||atoi(regPoint)>31){
                        node->errorFlag=1;/*The reg is not between 0 and 31.*/
                        /*break;*/
                    }
                    registers[count]=(char*) malloc(6 * sizeof(char));/*Allocate enough memory to store a reg*/
                    /*If the command is a copy command*/
                    if((i>4)&&(count==1))
                    {
                        strcpy(registers[count],emptyReg);/*Set the suitable register to zero.*/

                        count++;
			free(reg);
                        continue;
                    }
                    strcpy(registers[count],reg);
                    free(reg);
                    token = strtok(NULL, s);/*Continue to loop over the tokens until the token is null*/
                    count++;
            }
            /*If the command is logical command*/
            if(i<5){
                strcpy(opStrPoint,rOpCode[0]);/*Copy the suitable opcode to the Operation string.*/
                /*Copy each binary code for each register to the suitable place.*/
                for (j=0;j<3;j++)
                {
                    strcat(opStrPoint,registers[j]);
                    free(registers[j]);
                }
                strcat(opStrPoint,rComFunct[i]);/*Copy the suitable funct code to the Operation string.*/
            }
            else{
                strcpy(opStrPoint,rOpCode[1]);
                for (k=0;k<=2;k++)
                {
                    strcat(opStrPoint,registers[k]);
                    free(registers[k]);/*Free the register pointers*/
                }
                strcat(opStrPoint,rComFunct[i%5]);/*Copy the suitable funct code to the Operation string.*/
            }
            strcat(opStrPoint,notInUse);/*Set the last bits in the operation string as zeros.*/
            break;
        }
    }
    
    /*Check if the command is a I-COMMAND*/
    if(!comFlag){
        for(i=0;i<ICOMLEN;i++){
            if(!strcmp(ptrField2,iCommands[i])){
                comFlag=1;
	        node->p = NULL;
                if(i<5||i>8){
                    while( token != NULL ) {
                            if (count==1)
                            {
                                registers[count]=NULL;
                                if (atoi(token)>32767||atoi(token)<-32768)
                                {
                                    node->errorFlag=2;
                                    /*break;*/
                                }
				imm=decToBin(atoi(token));
                                /*strcpy(imm,immPointer);*/
                                count++;
                                token = strtok(NULL, s);
                                continue;
                            }
                            reg=Registers(token);/*Convert the registers tokens to a binary reg string.*/
			    regPoint = token;
			    regPoint++;
                            if (atoi(regPoint)<0||atoi(regPoint)>31){
                                node->errorFlag=1;/*The reg is not between 0 and 31.*/
                                /*break;*/
                            }
                            registers[count]=(char*) malloc(6 * sizeof(char));/*Allocate enough memory to store a reg*/
                            strcpy(registers[count],reg);/*Copy each reg to a place in the registers array*/
                            free(reg);
                            token = strtok(NULL, s);
                            count++;
                    }
    
                    strcpy(opStrPoint,iOpCode[i]);/*Copy the suitable opcode to the Operation string.*/
                    for (j=0;j<3;j++)
                        {
                            if (j==1)
                                continue;
                            strcat(opStrPoint,registers[j]);
                            free(registers[j]);
                        }
                        strcat(opStrPoint,imm);
                        free(imm);
                }
                else{
                    while( token != NULL ) {
                        if (count==2){
                                node->missLabelFlag=2;
                                strcpy(node->symbol,token);
                                registers[2]=NULL;
                                break;
                        }
                        reg=Registers(token);/*Convert the registers tokens to a binary reg string.*/
                        registers[count]=(char*) malloc(6 * sizeof(char));/*Allocate memory for the registers.*/
                        strcpy(registers[count],reg);/*Copy reg value to the registers array.*/
                        free(reg);/*Free allocated memory.*/
                        token = strtok(NULL, s);
                        count++;
                    }
                        
                    strcpy(opStrPoint,iOpCode[i]);
                    for (j=0;j<2;j++)
                        {
                            strcat(opStrPoint,registers[j]);
                            free(registers[j]);
                        }
                    /*strcat(opStrPoint,imm);
                    free(imm);*/
                }
                break;
            }    
        }
    }
    
        /*Check if the command is a J-COMMAND*/
    if(!comFlag){
        for(i=0;i<JCOMLEN;i++){
            if(!strcmp(ptrField2,jCommands[i])){
                comFlag=1;
	        node->p = NULL;
                if (i<3){
                    strcpy(opStrPoint,jOpCode[i]);
                    /*If the second field is a label field*/
                    if(*ptrField3!='$'){
                        node->missLabelFlag=1;
                        strcpy(node->symbol,ptrField3);
                    }
                    /*If the second field is a register field*/
                    else{
                        reg=Registers(ptrField3);/*Extract reg value from the function.*/
		        regPoint = token;
		        regPoint++;
                        if (atoi(regPoint)<0||atoi(regPoint)>31){
                            node->errorFlag=1;/*The reg is not between 0 and 31.*/
                            /*break;*/
                        }
                        strcat(opStrPoint,one);/*Concat one to the Operation string.*/
                        strcat(opStrPoint,reg);/*Concat reg to the Operation string.*/
                        free(reg);
                    }
                }
                else{
                    strcpy(opStrPoint,jOpCode[i]);/*Copy the immediate for the suitable J-COMMAND*/
		    immJ=decToBinJ(0);
                    /*strcpy(immStop,immJ);
                    strcat(immStop,zero);
                    strcat(opStrPoint,immStop);*/
		    strcat(immJ,zero);
		    strcat(opStrPoint,immJ);
		    free(immJ);
		    /*free(immStop);*/
		    
                }
                break;
            }
    
        }
    }
    if(!comFlag){
        if(!strcmp(ptrField2,".asciz"))
        {
            comFlag=1;
            temp=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for a data node.*/
            node->p=temp;/*Point to the allocated memory.*/
            while (*ptrField3!='\0'){
                int asciCode=*ptrField3;/*Extracting the ascii code of any letter.*/
                char *letter=ascizToBin(asciCode);/*Extracting a suitable string from the asciCode.*/
                dataCounter+=1;
                strcat(temp->byte,letter);/*Copy the string to byte array within the temp node.*/
                newNode=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for the new node.*/
                temp->next=newNode;/*Point to the new node.*/
                temp=newNode;/*Set the temp node to point on the newNode.*/
                free(letter);/*Free the allocated space for the string*/
                ptrField3++;/*Advance to the next byte.*/
            }
            strcat(temp->byte,null);/*Concatenate the null bits to the end of the ascii linked-list.*/
            dataCounter+=1;
            node->localDc=dataCounter;
        }
    }
    
    if(!comFlag){
        if(!strcmp(ptrField2,".db"))
        {
            comFlag=1;
            temp=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for a data node.*/
            node->p=temp;/*Point to the allocated memory.*/
            while( token != NULL ){
                dataCounter+=1;
                if (atoi(token)>127||atoi(token)<-128){
                    node->errorFlag=3;
                    /*break;*/
                }
                binNum=ascizToBin(atoi(token));/*Convert a token to binary string.*/
                strcat(temp->byte,binNum);
                free(binNum);
                token = strtok(NULL, s);
                if (token==NULL)
                    break;
                newNode=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for the new node.*/
                temp->next=newNode;/*Point to the new node.*/
                temp=newNode;/*Set the temp node to point on the newNode.*/
            }
            node->localDc=dataCounter;
            /*newNode->next=NULL;*/
        }
    }
    
    if(!comFlag){
        if(!strcmp(ptrField2,".dw"))
        {
            comFlag=1;
            temp=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for a data node.*/
            node->p=temp;/*Point to the allocated memory.*/
            while( token != NULL ){
                dataCounter+=4;
                if (atol(token)>=2147483647||atol(token)<=(-2147483647 - 1))
                {
                    node->errorFlag=4;
                    /*break;*/
                }
                binNum=decToBinDirW(token);/*Convert a token to a binary string.*/
                binNumStart=binNum;
                binNum+=24;/*Advance to the last byte in the word*/
                strncpy(temp->byte,binNum,8);

                for(i=0;i<3;i++){
                    newNode=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for the new node.*/
                    temp->next=newNode;/*Point to the new node.*/
                    temp=newNode;/*Set the temp node to point on the newNode.*/
                    binNum-=8;/*Go the the next byte.*/
                    strncpy(temp->byte,binNum,8);

                }
                token = strtok(NULL, s);
                if (token != NULL)
                {
                    newNode=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for the new node.*/
                    temp->next=newNode;/*Point to the new node.*/
                    temp=newNode;/*Set the temp node to point on the newNode.*/
                }
                free(binNumStart);
            }
            node->localDc=dataCounter;
        }
    }
    
    if(!comFlag){
        if(!strcmp(ptrField2,".dh"))
        {
            temp=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for a data node.*/
            node->p=temp;/*Point to the allocated memory.*/
            
            while( token != NULL ){
                dataCounter+=2;
                if (atoi(token)>32767||atoi(token)<-32768)
                {
                    node->errorFlag=5;
                    /*break;*/
                }
                binNum=decToBinDirH(token);/*Convert a token to a binary string.*/
                binNumStart=binNum;
                binNum+=8;
                strncpy(temp->byte,binNum,8);
                newNode=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for the new node.*/
                temp->next=newNode;
                temp=newNode;
                binNum-=8;
                strncpy(temp->byte,binNum,8);
                token = strtok(NULL, s);
                if (token != NULL)
                {
                    newNode=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for the new node.*/
                    temp->next=newNode;/*Point to the new node.*/
                    temp=newNode;/*Set the temp node to point on the newNode.*/
                }
                free(binNumStart);
            }
            node->localDc=dataCounter;
        }
    }

    if( !(!strcmp(ptrField2,".asciz") || !strcmp(ptrField2,".db") || !strcmp(ptrField2,".dw") || !strcmp(ptrField2,".dh"))   ){
        node->p = NULL;
	node->next = NULL;
    }

    /*memcpy(node->op,opString,sizeof(*opString));*/
    strcpy(node->op,opString);/*Copy the opString to the operation field in the node's structure*/
    free(lineStr);/*Free the line string*/
    return node;/*Return the updated node.*/
}

char *Registers(char *reg)
{
    int num;
    char *temp;/*Temporary char pointer.*/
    int i,j;
    char *str=(char*)malloc(6);/*Allocate memory for a reg string.*/
    temp=reg;
    if (reg[0]!='$'){
        /*printf("Not a register!");
    	exit(EXIT_FAILURE);*/
    	return NULL;
    }
    temp++;
    num=atoi(temp);/*Convert a temp char string to a integer number.*/
    str[5]='\0';
    j=4;
    while (j>=0)
	   {
		str[j--]=num%2+'0';
                num=num/2;
                i++;		
	   }
    return str;
}

char *decToBin(int num)
{
    unsigned int i;
    char *str=(char*)malloc(17);/*Allocate memory for a binary immediate representation*/
    for(i=0; i<16; i++)
    {
      unsigned int mask = 1 << (16 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[16] = '\0';
    return str;

}

char *decToBinJ(int num)
{
    unsigned int i;
    char *str=(char*)malloc(26+1);/*Allocate memory for a binary immediate representation*/
    for(i=0; i<25; i++)
    {
      unsigned int mask = 1 << (25 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[25] = '\0';
    return str;
}

char *ascizToBin(int num)
{
    unsigned int i;
    char *str=(char*)malloc(9);/*Allocate memory for byte representation*/
    for(i=0; i<8; i++)
    {
      unsigned int mask = 1 << (8 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[8] = '\0';/*Put a zero at the end of the byte string*/
    return str;
}

char *decToBinDirW(char *number)
{
    int num;
    unsigned int i;
    char *str=(char*)calloc(33,sizeof(char));/*Allocate memory for word representation*/
    num=atoi(number);
    for(i=0; i<32; i++)
    {
      unsigned int mask = 1 << (32 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[32] = '\0';
    return str;
}

char *decToBinDirH(char *number)
{
    int num;
    unsigned int i;
    char *str=(char*)malloc(17);/*Allocate memory for a binary immediate representation*/
    num=atoi(number);
    for(i=0; i<32; i++)
    {
      unsigned int mask = 1 << (32 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      if(i<16)
        continue;
      str[i-16] = (num & mask) ? '1' : '0';
    }
    str[16] = '\0';
    return str;
}

void addNode(MEMIM *headCom,MEMIM *headData, MEMIM *node,int firstDataNodeAddflag,int firstComNodeAddflag)
{
    MEMIM *nodePointer;/*A pointer to a memory image node*/
    /*Add a new node the Commands list.*/
    int headDc;
    int prevDc;
    /*static int ic=100;*/
    static int ic;
    /*static int firstDataNodeAddflag=0;
    static int firstComNodeAddflag=0;*/
    if((firstDataNodeAddflag == FLAGOFF) && (firstComNodeAddflag == FLAGOFF)){
        ic = 100;
    }
    if (node->p==NULL){
        nodePointer=headCom;
        if (firstComNodeAddflag==0){
            nodePointer->ic=ic;
            /*firstComNodeAddflag=1;*/
        }
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        nodePointer->next=node;
        ic+=4;
        node->ic=ic;
    }
    /*Add a new node the Directives list.*/
    else{
        nodePointer=headData;
        headDc=headData->localDc;
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        prevDc=nodePointer->dc;
        nodePointer->next=node;
        if (firstDataNodeAddflag==0){
            node->dc=prevDc+node->localDc+headDc;
            /*firstDataNodeAddflag=1;*/
        }
        else
            node->dc=prevDc+node->localDc;
    }
}


void concatNodes(MEMIM *headCom,MEMIM *headData){
        MEMIM *nodePointer;/*A pointer to a memory image node*/
        nodePointer=headCom;/*Point to the head command.*/
        /*Concatenate commands list and directives list*/
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        nodePointer->next=headData;
}


void deleteNode(MEMIM *node){
    MEMIM *temp;/*A temp node which will be deleted from the linked list*/
    temp=node;
    /*While the linked list is not null-continue to delete nodes from the linked-list*/
    while(1){
        temp=node;
        if (temp==NULL)
            break;
        node=node->next;
        /*free(temp);*/
    }
}

void printSymbolTable(TABLE_NODE_T *symbolTable){
        TABLE_NODE_T *q;
        q=symbolTable;
        while(q!=NULL)
        {
            printf("%s\n",q->symbol);
            q = q->next;
        }
}

char binToHex(char *bin)
{
   char *i;
   char hex;/*The hex value which will be returned as a char*/
   int count=3;/*Intialize the counter*/
   int num=0;/*Initialize the num*/
   /*While the bin string is not null-continue to sum numbers*/
   for(i=bin;*i;i++){
        num+=(pow(2,count))*(*i - '0');
        count--;
    }
    /*Return the appropriate hex value*/
    if(num<10){
        hex = num+48;
    }
    else
        hex = num+55;
    return hex;
}

SYMBOL_ADD_STRUCT_T* symbolAddNew(MEMIM *head,TABLE_NODE_T* table,int lineNumber,int firstEntry,char *filename){
    char *immJ;
    char *imm;
    char *zero="0";
    static MEMIM *currentMem;
    TABLE_NODE_T *currentTable;
    /*static int firstEntry = FLAGON;*/
    SYMBOL_ADD_STRUCT_T *tmpPtr;
    SYMBOL_ADD_STRUCT_T *structPtr;
    int labelFoundFlag = FLAGOFF;

    tmpPtr = (SYMBOL_ADD_STRUCT_T*)calloc(1, sizeof(SYMBOL_ADD_STRUCT_T));
    if(!tmpPtr)
    {
	    printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
	    exit(0);
    }
    structPtr = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

    structPtr->externalFlag = FLAGOFF;
    structPtr->errorFlag = FLAGOFF;

    if(firstEntry == FLAGON){
    	currentMem = head;
	/*firstEntry = FLAGOFF;*/
    }
    currentTable = table;
    if(currentMem!=NULL){
        if(currentMem->missLabelFlag==1){
            while(currentTable!=NULL){
                if(!strcmp(currentMem->symbol,currentTable->symbol)){
		    labelFoundFlag = FLAGON;
		    if(currentTable->attribute[0] == EXTERNAL){
			structPtr->address = currentMem->ic;
			strcpy(structPtr->label,currentMem->symbol);
			structPtr->externalFlag = FLAGON;
			
		    }
                    immJ=decToBinJ(currentTable->value);
                    strcat(currentMem->op,zero);
                    strcat(currentMem->op,immJ);
                    free(immJ);
                    break;
                }
		if(currentTable->next!=NULL){
                    currentTable=currentTable->next;  
		}
		else{
		    break;
		}
            }
	    if(labelFoundFlag == FLAGOFF){
		structPtr->errorFlag = FLAGON;
		printf("File \"%s.as\", Line %u: Label \"%s\" not found!\n",filename,lineNumber,currentMem->symbol);
	    }
            
        }
        else if(currentMem->missLabelFlag==2){
            while(currentTable!=NULL){
                if(!strcmp(currentMem->symbol,currentTable->symbol)){
		    labelFoundFlag = FLAGON;
		    if(currentTable->attribute[0] == EXTERNAL){
			structPtr->errorFlag = FLAGON;
			printf("File \"%s.as\", Line %u: External label \"%s\" used for conditional branching!\n",filename,lineNumber,currentMem->symbol);
			break;
		    }
                    imm=decToBin((currentTable->value)-(currentMem->ic));
                    strcat(currentMem->op,imm);
                    free(imm);
                    break;
                }
		if(currentTable->next!=NULL){
                    currentTable=currentTable->next;  
		} 
		else{
		    break;
		}
            }
	    if(labelFoundFlag == FLAGOFF){
		structPtr->errorFlag = FLAGON;
		printf("File \"%s.as\", Line %u: Label \"%s\" not found!\n",filename,lineNumber,currentMem->symbol);
	    }
            
        }
	if(currentMem->next!=NULL){
            currentMem=currentMem->next; 
	    currentTable = table; 
	}
	/*else{
	    break;
	}
	*/
    }
    return structPtr;
}


void printListToFile (MEMIM *head,FILE *fptrObject)
{
    int i=0;
    int j=0;/*A counter index.*/
    int k=0;/*A counter index.*/
    int count=0;/*A counter index.*/
    int comCount;
    int inCount;/*Inside counter.*/
    int lastNodeFlag=0;/*A flag which signs.*/
    /*int bitsNum;*//*An integer which represents a number.*/
    /*static int ic=100;*//*Instruction counter.*/
    int ic=100;
    char *bin;/*Binary number pointer.*/
    char *startArr;/*Stores the first array.*/
    char mem[5];/*A temp memory which is used to store regs.*/
    char* printArr;/*The array which will be printed (represented with hexa chars).*/
    char hex;/*Hex char*/
    MEMIM *nodePointer;/*A pointer to a memory image node.*/
    DATA *temp;/*temporary data node.*/
    printArr=(char *) malloc(8);/*Memory allocation for the first array.*/
    startArr=printArr;
    nodePointer=head;
    /*nodePointerP = head->p;*/

    while(nodePointer!=NULL)
    {
        if (nodePointer->next==NULL)
            lastNodeFlag=1;/*Set flag to 1 if the current node is the last node.*/
        temp = nodePointer->p;
        if((temp)!=NULL){
            k=0;
            while((temp)!=NULL){
                bin=temp->byte;
                j=0;
                for(i=0;i<8;i++){
                    mem[j]=*bin;
                    if((i+1)%4==0){
                        mem[4]='\0';
                        hex=binToHex(mem);
                        j=0;
                        printArr[k++]=hex;
                        bin++;
                        continue;
                    }
                    j++;
                    bin++;
                }
                if (k%8==0){
                    printArr = (char *) realloc(printArr, k+8);
		    startArr = printArr;
                }
                /*temp=nodePointer->p;*/
                temp=temp->next;
                /*free(temp);*/
            }
            inCount=0;
            if (count==0)
                fprintf(fptrObject,"0%d ",ic);
            for(i=0;i<k;i+=2){
                fprintf(fptrObject,"%c",printArr[i]);
                fprintf(fptrObject,"%c ",printArr[i+1]);
                inCount+=2;
                count+=2;
                if ((count%8==0)){
                    fprintf(fptrObject,"\n");
                    ic+=4;
                    if ((lastNodeFlag==1)&&(inCount==k))
                        break;
                    fprintf(fptrObject,"0%d ",ic);
                    
                }
                   
            }
        }
        else
            {
            bin=nodePointer->op;
            k=0;
            for(i=0;i<32;i++){
                mem[j]=*bin;
                if((i+1)%4==0){
                mem[4]='\0';
                hex=binToHex(mem);
                printArr[k]=hex;
                j=0;
                bin++;
                k++;
                continue;
                }
                j++;
                bin++;
               
            }
            comCount=0;
            fprintf(fptrObject,"0%d ",ic);
            for(k=7;k>0;){
                comCount++;
                fprintf(fptrObject,"%c",printArr[k-1]);
                fprintf(fptrObject,"%c ",printArr[k]);
                if (comCount%8==0){
                    fprintf(fptrObject,"\n");
                    fprintf(fptrObject,"0%d ",ic);
                }
                k-=2;
            }
            fprintf(fptrObject,"\n");
            ic+=4;
        }
        nodePointer=nodePointer->next;

    }

    free(startArr);
}


