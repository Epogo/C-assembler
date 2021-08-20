#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assembler.h"
#include "assToBin.h"
#include <math.h>

#define MAX_8_BITS_NUM 127
#define MAX_16_BITS_NUM 32767
#define NUMREGS 3
#define LENCOMMASTRING 2
#define MINREG 0
#define MAXREG 31
#define REGMEM 6

enum Attributes {EMPTY,CODE,MYDATA,ENTRY,EXTERNAL}; /*enum of symbol table attributes*/

static char *rCommands[]={"add","sub","and","or","nor","move","mvhi","mvlo"};/*R commands table*/
static char *iCommands[]={"addi","subi","andi","ori","nori","bne","beq","blt","bgt","lb","sb","lw","sw","lh","sh"};/*I commands table*/
static char *jCommands[]={"jmp","la","call","stop"};/*J commands table*/
static char *rComFunct[]={"00001","00010","00011","00100","00101"};/*A table of functs for r commands*/
static char *rOpCode[]={"000000","000001"};/*A table of opcodes for r commands table*/
static char *iOpCode[]={"001010","001011","001100","001101","001110","001111","010000","010001","010010","010011","010100","010101","010110","010111","011000"}; /*A table of opcodes for i commands*/
static char *jOpCode[]={"011110","011111","100000","111111"};/*A table of opcodes for j commands*/

/*This function is responsible for adding nodes the memory image (first pass only).
@param ptrField1-This parameter is used for the label field.
@param ptrField2-This parameter is used for the command/directive name.
@param ptrField3-This parameter is used for the registers/Data.
@return node-a memory image struct is returned. 
This function is responsible for converting assembly lines to binary digits lines (binary memory image).
The function is building the memory image after the first pass, it detects the type of the command or directive
and converts the content of the assembly line to a bits array (by using "a table" which stores opcodes/functs of each command).
For commands from J type (excluding "stop" command) and branching commands-the missing symbol value will be completed by using a function
which has been written in order to perform the second pass.*/
MEMIM *memAdd(char *ptrField1,char *ptrField2,char *ptrField3){
    int count=STARTINDEX;/*A counter for commands.*/
    int comFlag=STARTINDEX;/*A flag which turns on if a given command or direcrive has been found.*/
    int dataCounter=STARTINDEX;/*A counter for data.*/
    int i,j,k;/*Local indices.*/
    char *lineStr;/*A pointer to a line string.*/
    const char s[LENCOMMASTRING] = ",";/*A comma token.*/
    char *token;/*A pointer to a token.*/
    char *reg;/*A char pointer to a register string.*/
    char opString[NUM_OF_BITS_OP];/*Operation string.*/
    char *imm;/*A pointer to an immediate (16 bits number)*/
    char *immJ;/*A pointer to an immediate (25 bits number)*/
    char *registers[NUMREGS];/*An array of pointers to registers.*/
    char *opStrPoint;/*A pointer to an operation string.*/
    char *binNum,*binNumStart;/*Binary number pointer and a pointer to the the first byte.*/
    char *zero="0";/*A string which represents "zero".*/
    char *emptyReg="00000";/*An array of bin chars for "empty register" bits within the 32bits slot.*/
    char *notInUse="000000";/*An array of bin chars for "not in use" bits within the 32bits slot.*/
    char *null="00000000";/*NULL terminator.*/
    char *one="100000000000000000000";/*In case of J command with reg.*/
    char *regPoint;/*A pointer to a register string*/
    DATA *temp;/*A Temporary pointer.*/
    DATA *newNode;/*An adress of a new node.*/

    MEMIM *node=(MEMIM *)calloc(SINGLENODE,sizeof(MEMIM));/*Memory allocation for a memory image node.*/
    if(!node)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
    
    lineStr = (char *)calloc(NUM_OF_CHARS_IN_LINE, sizeof(char));/*Memory allocation for a line*/
    if(!lineStr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
    opStrPoint=opString;/*Points to the operation string*/

    node->errorFlag=FLAGOFF;/*A default value for the error flag*/

    node->next = NULL;/*A default value for the next pointer of a node struct.*/
    opString[STARTINDEX] = '\0';/*An initialization of the operation string.*/

    if (ptrField3)
        strcpy(lineStr,ptrField3);/*If the third field isn't empty-copy the content of this field to the lineStr field.*/
    token = strtok(lineStr, s);/*A definition of a token*/
    
    /*This process will check if ptrField2 attribute is either a legal command or attribute*/
    /*Check if a given command is r-type command.*/
    for(i=STARTINDEX;i<RCOMLEN;i++){
        if(!strcmp(ptrField2,rCommands[i])){
            comFlag=FLAGON;/*Raise a flag to prevent further search for a match*/
	    node->p = NULL;/*The data linked-list pointer is null (for non-directives memory images).*/
            while( token != NULL ) {
                    reg=Registers(token);/*Converts the registers tokens to a binary reg string.*/
		    regPoint = token;/*Points to the start of the token string.*/
		    regPoint++;/*Skip the dollar sign.*/
                    if (atoi(regPoint)<MINREG||atoi(regPoint)>MAXREG){
                        node->errorFlag=ERRORFLAG1;/*A flag which signs that a register is not between 0 and 31.*/
                    }
                    registers[count]=(char*) malloc(REGMEM * sizeof(char));/*Allocate enough memory to store a register*/
	            if(!registers[count])
	                        {
		                        printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                        exit(0);
	                        }
                    /*If the command is a copy command*/
                    if((i>4)&&(count==1))
                    {
                        strcpy(registers[count],emptyReg);/*Set the suitable register to zero.*/
                        count++;/*Raise the counter by 1*/
			free(reg);/*Free the memory which has been allocated for the register.*/
                        continue;
                    }
                    strcpy(registers[count],reg);/*Copy each reg to the registers pointers array*/
                    free(reg);/*Free the memory which has been allocated for the register.*/
                    token = strtok(NULL, s);/*Continue to loop over the tokens until the token is null*/
                    count++;/*Raise the counter by 1*/
            }
            /*If the command is a logical or arithmetic command*/
            if(i<5){
                strcpy(opStrPoint,rOpCode[0]);/*Copy the suitable opcode to the Operation string.*/
                /*Copy each binary code for each register to the suitable place.*/
                for (j=STARTINDEX;j<3;j++)
                {
                    strcat(opStrPoint,registers[j]);/*Concatenate each register to the operation string within the node struct.*/
                    free(registers[j]);/*Free each pointer.*/
                }
                strcat(opStrPoint,rComFunct[i]);/*Copy the suitable funct code to the Operation string.*/
            }
	    /*If the command a copy command.*/
            else{
                strcpy(opStrPoint,rOpCode[1]);
                for (k=STARTINDEX;k<=2;k++)
                {
                    strcat(opStrPoint,registers[k]);/*Concatenate each register to the operation string within the node struct.*/
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
        for(i=STARTINDEX;i<ICOMLEN;i++){
            if(!strcmp(ptrField2,iCommands[i])){
                comFlag=FLAGON;/*Raise a flag to prevent further search for a match*/
	        node->p = NULL;/*The data linked-list pointer is null (for non-directives memory images).*/
                if(i<5||i>8){
                    while( token != NULL ) {
                            if (count==1)
                            {
                                if (atoi(token)>MAX_16_BITS_NUM||atoi(token)<-(MAX_16_BITS_NUM+1))
                                {
                                    node->errorFlag=ERRORFLAG2;/*A flag which signs that an immediate is bigger than the size of 16 bits.*/
                                }
				imm=decToBin(atoi(token));/*Convert a given token to an immediate.*/
                                count++;/*Raise the count by 1*/
                                token = strtok(NULL, s);/*Split a line which consists from tokens seperated by a comma.*/
                                continue;
                            }
                            reg=Registers(token);/*Convert the registers tokens to a binary reg string.*/
			    regPoint = token;/*Points to the start of the token string.*/
			    regPoint++;/*Skip the dollar sign.*/
                            if (atoi(regPoint)<MINREG||atoi(regPoint)>MAXREG){
                                node->errorFlag=ERRORFLAG1;/*A flag which signs that a register is not between 0 and 31.*/
                            }
                            registers[count]=(char*) malloc(REGMEM * sizeof(char));/*Allocate enough memory to store a register*/
		            if(!registers[count])
	                        {
		                        printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                        exit(0);
	                        }
                            strcpy(registers[count],reg);/*Copy each reg to a place in the registers array*/
                            free(reg);/*Free the register*/
                            token = strtok(NULL, s);/*Split a line which consists from tokens seperated by a comma.*/
                            count++;/*Raise the count by 1*/
                    }
    
                    strcpy(opStrPoint,iOpCode[i]);/*Copy the suitable opcode to the Operation string.*/
                    for (j=STARTINDEX;j<3;j++)
                        {
                            if (j!=1){
                            	strcat(opStrPoint,registers[j]);/*Concatenate each register to the operation string within the node struct.*/
                            	free(registers[j]);/*Free registers.*/
			    }

                        }
                        strcat(opStrPoint,imm);/*Concatenate the immediate value to the opearation string.*/
                        free(imm);/*Free the immediate value.*/
                }
                else{
                    while( token != NULL ) {
                        if (count==2){
                                node->missLabelFlag=2;/*Turn on the flag which indicates that a label is missing (within a branching command).*/
                                strcpy(node->symbol,token);/*Copy the symbol to symbol's field in the node struct.*/
                                registers[2]=NULL;
                                break;
                        }
                        reg=Registers(token);/*Convert the registers tokens to a binary reg string.*/
                        registers[count]=(char*) malloc(REGMEM * sizeof(char));/*Allocate memory for the registers.*/
			if(!registers[count])
	                        {
		                        printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                        exit(0);
	                        }
                        strcpy(registers[count],reg);/*Copy reg value to the registers array.*/
                        free(reg);/*Free allocated memory.*/
                        token = strtok(NULL, s);
                        count++;
                    }
                        
                    strcpy(opStrPoint,iOpCode[i]);
                    for (j=STARTINDEX;j<2;j++)
                        {
                            strcat(opStrPoint,registers[j]);/*Concatenate each register to the operation string within the node struct.*/
                            free(registers[j]);/*Free registers.*/
                        }
                }
                break;
            }    
        }
    }
    
    /*Check if the command is a J-COMMAND*/
    if(!comFlag){
        for(i=STARTINDEX;i<JCOMLEN;i++){
            if(!strcmp(ptrField2,jCommands[i])){
                comFlag=FLAGON;/*Raise a flag to prevent further search for a match*/
	        node->p = NULL;/*The data linked-list pointer is null (for non-directives memory images).*/
                if (i<3){
                    strcpy(opStrPoint,jOpCode[i]);/*Copy the relevant operation code to the operation string.*/
                    /*If the second field is a label field*/
                    if(*ptrField3!='$'){
                        node->missLabelFlag=1;/*Turn on the flag which indicates that a label is missing (within a J-type command).*/
                        strcpy(node->symbol,ptrField3);/*Copy the symbol to symbol's field in the node struct.*/
                    }
                    /*If the second field is a register field*/
                    else{
                        reg=Registers(ptrField3);/*Extract reg value from the function.*/
		        regPoint = token;/*Points to the start of the token string.*/
		        regPoint++;/*Skip the dollar sign.*/
                        if (atoi(regPoint)<MINREG||atoi(regPoint)>MAXREG){
                            node->errorFlag=ERRORFLAG1;/*The reg is not between 0 and 31.*/
                        }
                        strcat(opStrPoint,one);/*Concat one to the Operation string.*/
                        strcat(opStrPoint,reg);/*Concat reg to the Operation string.*/
                        free(reg);
                    }
                }
                else{
                    strcpy(opStrPoint,jOpCode[i]);/*Copy the immediate for the suitable J-COMMAND*/
		    immJ=decToBinJ(0);/*Put zeros as the immediate value (for stop command).*/
		    strcat(immJ,zero);
		    strcat(opStrPoint,immJ);/*Concatenate the immediate value to the operation string.*/
		    free(immJ);/*Free the immediate pointer.*/		    
                }
                break;
            }
    
        }
    }
    if(!comFlag){
        if(!strcmp(ptrField2,".asciz"))
        {
            comFlag=FLAGON;/*Raise a flag to prevent further search for a match*/
            temp=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for a data node.*/
	    if(!temp)
	        {
		        printf("\nError! memory not allocated.");/*Prints error message if no more memory could be allocated*/
			exit(0);
		} 
		   
            node->p=temp;/*Point to the allocated memory.*/
            while (*ptrField3!='\0'){
                int asciCode=*ptrField3;/*Extracting the ascii code of any letter.*/
                char *letter=ascizToBin(asciCode);/*Extracting a suitable string from the asciCode.*/
                dataCounter+=1;/*Counts each byte of data*/
                strcat(temp->byte,letter);/*Copy the string to byte array within the temp node.*/
                newNode=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for the new node.*/
		if(!newNode)
	                {
		                printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                exit(0);
	                }
                temp->next=newNode;/*Point to the new node.*/
                temp=newNode;/*Set the temp node to point on the newNode.*/
                free(letter);/*Free the allocated space for the string*/
                ptrField3++;/*Advance to the next byte.*/
            }
            strcat(temp->byte,null);/*Concatenate the null bits to the end of the ascii linked-list.*/
            dataCounter+=1;/*Counts each byte of data*/
            node->localDc=dataCounter;/*Set the local data counter as the calculated data counter.*/
        }
    }
    
    if(!comFlag){
        if(!strcmp(ptrField2,".db"))
        {
            comFlag=FLAGON;/*Raise a flag to prevent further search for a match.*/
            temp=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for a data node.*/
	    if(!temp)
	        {
		        printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		        exit(0);
	        }
            node->p=temp;/*Point to the allocated memory.*/
            while( token != NULL ){
                dataCounter+=1;/*Counts each byte of data*/
                if (atoi(token)>127||atoi(token)<-128){
                    node->errorFlag=ERRORFLAG3;/*Set an error flag which indicates that the immediate number is bigger than 8 bits.*/
                }
                binNum=ascizToBin(atoi(token));/*Convert a token to binary string.*/
                strcat(temp->byte,binNum);/*Copy the binary number to the byte's string.*/
                free(binNum);/*Free the allocated memory.*/
                token = strtok(NULL, s);
                if (token==NULL)
                    break;
                newNode=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for the new node.*/
		if(!newNode)
	                {
		                printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                exit(0);
	                }
                temp->next=newNode;/*Point to the new node.*/
                temp=newNode;/*Set the temp node to point on the newNode.*/
            }
            node->localDc=dataCounter;/*Set the local data counter as the calculated data counter.*/
        }
    }
    
    if(!comFlag){
        if(!strcmp(ptrField2,".dw"))
        {
            comFlag=FLAGON;/*Raise a flag to prevent further search for a match.*/
            temp=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for a data node.*/
	    if(!temp)
	        {
		        printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		        exit(0);
	        }
            node->p=temp;/*Point to the allocated memory.*/
            while( token != NULL ){
                dataCounter+=4;/*Counts each word of data*/
                if (atol(token)>=2147483647||atol(token)<=(-2147483647 - 1))
                {
                    node->errorFlag=ERRORFLAG4;/*Set an error flag which indicates that the immediate number is bigger than 32 bits.*/
                }
                binNum=decToBinDirW(token);/*Convert a token to a binary string.*/
                binNumStart=binNum;/*Points to the first byte of the word.*/
                binNum+=24;/*Advance to the last byte in the word.*/
                strncpy(temp->byte,binNum,8);/*Copy each 8 bits to the bytes linked list.*/

                for(i=STARTINDEX;i<3;i++){
                    newNode=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for the new node.*/
	            if(!newNode)
	                {
		                printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                exit(0);
	                }
                    temp->next=newNode;/*Point to the new node.*/
                    temp=newNode;/*Set the temp node to point on the newNode.*/
                    binNum-=8;/*Go the the previous byte.*/
                    strncpy(temp->byte,binNum,8);/*Copy each 8 bits to the bytes linked list.*/

                }
                token = strtok(NULL, s);
                if (token != NULL)
                {
                    newNode=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for the new node.*/
		    if(!newNode)
	                {
		                printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                exit(0);
	                }
                    temp->next=newNode;/*Point to the new node.*/
                    temp=newNode;/*Set the temp node to point on the newNode.*/
                }
                free(binNumStart);/*Free the entire word.*/
            }
            node->localDc=dataCounter;/*Set the local data counter as the calculated data counter.*/
        }
    }
    
    if(!comFlag){
        if(!strcmp(ptrField2,".dh"))
        {
            temp=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for a data node.*/
	    if(!temp)
	                {
		                printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                exit(0);
	                }
            node->p=temp;/*Point to the allocated memory.*/
            
            while( token != NULL ){
                dataCounter+=2;/*Counts each half word of data*/
                if (atoi(token)>32767||atoi(token)<-32768)
                {
                    node->errorFlag=ERRORFLAG5;/*Set an error flag which indicates that the immediate number is bigger than 16 bits.*/
                }
                binNum=decToBinDirH(token);/*Convert a token to a binary string.*/
                binNumStart=binNum;/*Points to the first byte of the word.*/
                binNum+=8;/*Go the the next byte.*/
                strncpy(temp->byte,binNum,8);
                newNode=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for the new node.*/
		if(!newNode)
	                {
		                printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                exit(0);
	                }
                temp->next=newNode;/*Point to the new node.*/
                temp=newNode;/*Set the temp node to point on the newNode.*/
                binNum-=8;/*Go the the previous byte.*/
                strncpy(temp->byte,binNum,8);/*Copy each 8 bits to the bytes linked list.*/
                token = strtok(NULL, s);
                if (token != NULL)
                {
                    newNode=(DATA*)calloc(SINGLENODE, sizeof(DATA));/*Allocate memory for the new node.*/
			if(!newNode)
	                {
		                printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		                exit(0);
	                }
                    temp->next=newNode;/*Point to the new node.*/
                    temp=newNode;/*Set the temp node to point on the newNode.*/
                }
                free(binNumStart);/*Free the entire word.*/
            }
            node->localDc=dataCounter;/*Set the local data counter as the calculated data counter.*/
        }
    }

    if( !(!strcmp(ptrField2,".asciz") || !strcmp(ptrField2,".db") || !strcmp(ptrField2,".dw") || !strcmp(ptrField2,".dh"))   ){
        node->p = NULL;/*Set the data pointer to NULL*/
	node->next = NULL;/*Set the next pointer of the node to NULL.*/
    }

    strcpy(node->op,opString);/*Copy the opString to the operation field in the node's structure*/
    free(lineStr);/*Free the line string*/
    return node;/*Return the updated node.*/
}

/*This function takes as an input a register string and then-convert the string to a binary string.
@param reg-The register string (with the "$" sign).
@return str-the returned binary string from the function.
The function gets the register string from the user, converts the string to a int number, then-converts the number to a binary number
and then-the binary number is converted to a string.*/
char *Registers(char *reg)
{
    int num;/*Integer number*/
    char *temp;/*Temporary char pointer.*/
    int i,j;/*Indecies*/
    char *str=(char*)malloc(REGMEM);/*Allocate memory for a reg string.*/
	if(!str)
	{
	    printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
    temp=reg;/*Point to the register string.*/
    if (reg[STARTINDEX]!='$'){
    	return NULL;/*Not a valid register.*/
    }
    temp++;/*Raise the temp value by 1.*/
    num=atoi(temp);/*Convert a temp char string to a integer number.*/
    str[5]='\0';/*Null terminator.*/
    j=4;
    /*Convert the string reg to a number.*/
    while (j>=STARTINDEX)
	   {
		str[j--]=num%2+'0';
                num=num/2;
                i++;		
	   }
    return str;
}

/*This function takes an integer and return a string of 16 bits which represents an immediate number (for i type commands).
@param num-the immediate num which should be converted.
@return str-bits string.
The function is using bit manipulation in order to convert a given number (positive or negative) to a bits string.*/
char *decToBin(int num)
{
    unsigned int i;
    char *str=(char*)malloc(17);/*Allocate memory for a binary immediate representation*/
    if(!str)
	{
	    printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
    for(i=STARTINDEX; i<16; i++)
    {
      unsigned int mask = 1 << (16 - 1 - i);/*Implement a mask in order to turn on the appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[16] = '\0';/*Set null terminator.*/
    return str;

}

char *decToBinJ(int num)
{
    unsigned int i;
    char *str=(char*)malloc(27);/*Allocate memory for a binary immediate representation*/
	if(!str)
	{
	    printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
    for(i=STARTINDEX; i<25; i++)
    {
      unsigned int mask = 1 << (25 - 1 - i);/*Implement a mask in order to turn on the appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[25] = '\0';/*Set null terminator.*/
    return str;
}

char *ascizToBin(int num)
{
    unsigned int i;
    char *str=(char*)malloc(9);/*Allocate memory for byte representation*/
	if(!str)
	{
	    printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
    for(i=0; i<8; i++)
    {
      unsigned int mask = 1 << (8 - 1 - i);/*Implement a mask in order to turn on the appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[8] = '\0';/*Set null terminator.*/
    return str;
}

char *decToBinDirW(char *number)
{
    int num;
    unsigned int i;
    char *str=(char*)calloc(33,sizeof(char));/*Allocate memory for word representation*/
	if(!str)
	{
	    printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
    num=atoi(number);/*Converts a number which is represented as a string to an int number.*/
    for(i=STARTINDEX; i<32; i++)
    {
      unsigned int mask = 1 << (32 - 1 - i);/*Implement a mask in order to turn on the appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[32] = '\0';/*Set null terminator.*/
    return str;
}

char *decToBinDirH(char *number)
{
    int num;
    unsigned int i;
    char *str=(char*)malloc(17);/*Allocate memory for a binary immediate representation*/
        if(!str)
	{
	    printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}

    num=atoi(number);/*Converts a number which is represented as a string to an int number.*/
    for(i=STARTINDEX; i<32; i++)
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
    int headDc;/*The value of data counter of the first memory image node.*/
    int prevDc;/*The value of data counter of the previous memory image node.*/
    static int ic;/*Instructions counter.*/
    if((firstDataNodeAddflag == FLAGOFF) && (firstComNodeAddflag == FLAGOFF)){
        ic = 100;/*Initialize the instruction counter.*/
    }
    /*Add a given memory image node to the commands list.*/
    if (node->p==NULL){
        nodePointer=headCom;/*the node pointer points to the head of the commands list.*/
        if (firstComNodeAddflag==FLAGOFF){
            nodePointer->ic=ic;/*Set the current data counter value is to the initialized value of dc.*/
        }
	/*Iterate through the linked list until the last node is reached.*/
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        nodePointer->next=node;
        ic+=4;/*Raise instructions counter by 4.*/
        node->ic=ic;/*Set the ic value to the current data counter value.*/
    }
    /*Add a new node the Directives list.*/
    else{
        nodePointer=headData;/*the node pointer points to the head of the data list.*/
        headDc=headData->localDc;/*Set the head dc to the local data counter of the head data.*/
	/*Iterate through the linked list until the last node is reached.*/
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        prevDc=nodePointer->dc;/*Set the previous dc to the current data counter value of the node.*/
        nodePointer->next=node;/*Advance to the next node.*/
        if (firstDataNodeAddflag==FLAGOFF){
            node->dc=prevDc+node->localDc+headDc;
            /*firstDataNodeAddflag=1;*/
        }
        else
            node->dc=prevDc+node->localDc;/*Calculate the current value of data counter*/
    }
}


void concatNodes(MEMIM *headCom,MEMIM *headData){
        MEMIM *nodePointer;/*A pointer to a memory image node*/
        nodePointer=headCom;/*Point to the head command.*/
        /*Concatenate the directives list to the commands list*/
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        nodePointer->next=headData;
}

char binToHex(char *bin)
{
   char *i;
   char hex;/*The hex value which will be returned as a char*/
   int count=3;/*Initialize the counter*/
   int num=STARTINDEX;/*Initialize the num*/
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
    char *imm;/*16 bits immediate.*/
    char *immJ;/*25 bits immediate.*/
    char *zero="0";/*A zero string.*/
    static MEMIM *currentMem;/*Current memory image node.*/
    TABLE_NODE_T *currentTable;/*Current table image node.*/
    /*static int firstEntry = FLAGON;*/
    SYMBOL_ADD_STRUCT_T *tmpPtr;/*A temporary pointer.*/
    SYMBOL_ADD_STRUCT_T *structPtr;/*A struct pointer.*/
    int labelFoundFlag = FLAGOFF;/*A flag which indicates that a label has been found.*/

    tmpPtr = (SYMBOL_ADD_STRUCT_T*)calloc(1, sizeof(SYMBOL_ADD_STRUCT_T));
    if(!tmpPtr)
    {
	    printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated.*/
	    exit(0);
    }
    structPtr = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated.*/

    structPtr->externalFlag = FLAGOFF;/*Set the the flag which indicates that an external flag has been found.*/
    structPtr->errorFlag = FLAGOFF;/*Set the the flag which indicates that an error has been found.*/

    if(firstEntry == FLAGON){
    	currentMem = head;/*Point with the current memory pointer to the head of the memory image.*/
	/*firstEntry = FLAGOFF;*/
    }
    currentTable = table;/*Point to the head of the table linked list.*/
    if(currentMem!=NULL){
	/*In case of J-type command which uses a label*/
        if(currentMem->missLabelFlag==1){
            while(currentTable!=NULL){
                if(!strcmp(currentMem->symbol,currentTable->symbol)){
		    labelFoundFlag = FLAGON;/*If a label has been found in the symbol table.*/
		    if(currentTable->attribute[ATTRIBUTE1] == EXTERNAL){
			structPtr->address = currentMem->ic;/*Set the address of a node to the current instruction count value.*/
			strcpy(structPtr->label,currentMem->symbol);/*Set the label of a node to the current symbol value.*/
			structPtr->externalFlag = FLAGON;/*Turn on the flag which indicates that an external flag has been found.*/
			
		    }
                    immJ=decToBinJ(currentTable->value);/*Convert the value of a symbol from the symbol table to a suitable immediate value.*/
                    strcat(currentMem->op,zero);/*Concatenate a zero to the operation string.*/
                    strcat(currentMem->op,immJ);/*Concatenate the immediate to the operation string.*/
                    free(immJ);/*Free the allocated memory.*/
                    break;
                }
		if(currentTable->next!=NULL){
                    currentTable=currentTable->next;/*Iterate through the symbol table until the last node has been reached.*/
		}
		else{
		    break;
		}
            }
	    if(labelFoundFlag == FLAGOFF){
		structPtr->errorFlag = FLAGON;/*If a given label hasn't been found in a symbol table-raise an error flag.*/
		errorMsg(ERRORTYPE25,lineNumber,currentMem->symbol,filename);/*Raise an error message.*/
	    }
            
        }
	/*In case of a branching command which uses a label*/
        else if(currentMem->missLabelFlag==2){
            while(currentTable!=NULL){
                if(!strcmp(currentMem->symbol,currentTable->symbol)){
		    labelFoundFlag = FLAGON;/*Turn on a flag which indicates that a label has been found.*/
		    if(currentTable->attribute[ATTRIBUTE1] == EXTERNAL){
			structPtr->errorFlag = FLAGON;/*Raise a flag which indicates that an error has been found if a given label is an external label.*/
			errorMsg(ERRORTYPE26,lineNumber,currentMem->symbol,filename);/*Raise an error message.*/
			break;
		    }
                    imm=decToBin((currentTable->value)-(currentMem->ic));/*Calculate the 16-bits immediate number which indicates the "distance" between the instruction count and the value of 
		    a given label.*/
                    strcat(currentMem->op,imm);/*Concatenate the calculated immediate value to the operation string.*/
                    free(imm);/*Free the allocated memory.*/
                    break;
                }
		if(currentTable->next!=NULL){
                    currentTable=currentTable->next;  /*Iterate over the symbol table until the last node has been reached.*/
		} 
		else{
		    break;
		}
            }
	    if(labelFoundFlag == FLAGOFF){
		structPtr->errorFlag = FLAGON;/*If a label hasn't been found-raise a flag.*/
		errorMsg(ERRORTYPE25,lineNumber,currentMem->symbol,filename);/*Raise an error message.*/
	    }
            
        }
	if(currentMem->next!=NULL){
            currentMem=currentMem->next; 
	    currentTable = table; /*Iterate over the symbol table until the last node has been reached and point on the last node of the symbol table.*/
	}
    }
    return structPtr;/*Return a pointer to the added symbol table node.*/
}


void printListToFile (MEMIM *head,FILE *fptrObject)
{
    int i=STARTINDEX;/*A counter index.*/
    int j=STARTINDEX;/*A counter index.*/
    int k=STARTINDEX;/*A counter index.*/
    int count=STARTINDEX;/*A counter index.*/
    int comCount;/*Commands counter*/
    int inCount;/*Inside counter.*/
    int lastNodeFlag=FLAGOFF;/*A flag which signs.*/
    int ic=ICINIT;/*Initial value of the instruction counter.*/
    char *bin;/*Binary number pointer.*/
    char *startArr;/*Stores the first array.*/
    char mem[5];/*A temp memory which is used to store regs.*/
    char* printArr;/*The array which will be printed (represented with hexa chars).*/
    char hex;/*Hex char*/
    MEMIM *nodePointer;/*A pointer to a memory image node.*/
    DATA *temp;/*temporary data node.*/
    printArr=(char *) malloc(8);/*Memory allocation for the first array.*/
    startArr=printArr;/*Point to the start of the array which will be printed.*/
    nodePointer=head;/*Point to the head of the linked list.*/

    while(nodePointer!=NULL)
    {
        if (nodePointer->next==NULL)
            lastNodeFlag=FLAGON;/*Set flag to 1 if the current node is the last node.*/
        temp = nodePointer->p;/*pointer to the data pointer*/
	/*If the current memory image node is of data type.*/
        if((temp)!=NULL){
            k=STARTINDEX;
            while((temp)!=NULL){
                bin=temp->byte;/*Point to byte array*/
                j=STARTINDEX;
		/*Forming of couples of hex chars*/
                for(i=STARTINDEX;i<8;i++){
                    mem[j]=*bin;
                    if((i+1)%4==0){
                        mem[4]='\0';
                        hex=binToHex(mem);
                        j=STARTINDEX;
                        printArr[k++]=hex;
                        bin++;
                        continue;
                    }
                    j++;
                    bin++;
                }
                if (k%8==0){
                    printArr = (char *) realloc(printArr, k+8);/*If a new line has been reached - re-allocate memory for the printing array.*/
		    if(!printArr)
			{
	    			printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
				exit(0);
			}
		    startArr = printArr;/*Save the first byte of the pointed address.*/
                }

                temp=temp->next;
            }
            inCount=STARTINDEX;/*Initialize the inner counter.*/
            if (count==STARTINDEX)
                fprintf(fptrObject,"0%d ",ic);/*Print to the file the instruction counter.*/
            for(i=STARTINDEX;i<k;i+=2){
                fprintf(fptrObject,"%c",printArr[i]);/*Print the couples of hex chars.*/
                fprintf(fptrObject,"%c ",printArr[i+1]);
                inCount+=2;/*Increase the inner counter by 2.*/
                count+=2;/*Increase the counter by 2.*/
                if ((count%8==0)){
                    fprintf(fptrObject,"\n");/*Print a new line into the file*/
                    ic+=4;/*Raise the instructions counter by 4.*/
                    if ((lastNodeFlag==FLAGON)&&(inCount==k))
                        break;/*If the current node is the last node-break the loop.*/
                    fprintf(fptrObject,"0%d ",ic);/*Print the last instruction counter.*/
                    
                }
                   
            }
        }
	/*If the current memory image node is of command type.*/
        else
            {
            bin=nodePointer->op;/*A pointer to an operation string.*/
            k=STARTINDEX;/*Initialize the counter.*/
	    /*Convert each byte to a couple of hex chars.*/
            for(i=STARTINDEX;i<32;i++){
                mem[j]=*bin;
                if((i+1)%4==0){
                mem[4]='\0';
                hex=binToHex(mem);
                printArr[k]=hex;
                j=STARTINDEX;
                bin++;
                k++;
                continue;
                }
                j++;
                bin++;
               
            }
            comCount=STARTINDEX;/*Initialize the command counter.*/
            fprintf(fptrObject,"0%d ",ic);/*Print the last instruction counter.*/
	    /*Print all the 4 couples of hex chars (which are representing a command.*/
            for(k=7;k>STARTINDEX;){
                comCount++;
                fprintf(fptrObject,"%c",printArr[k-1]);
                fprintf(fptrObject,"%c ",printArr[k]);
                if (comCount%8==0){
                    fprintf(fptrObject,"\n");
                    fprintf(fptrObject,"0%d ",ic);
                }
                k-=2;
            }
            fprintf(fptrObject,"\n");/*Print a new line into the file*/
            ic+=4;/*Raise the instructions counter by 4.*/
        }
        nodePointer=nodePointer->next;/*Advance to the next memory image node.*/

    }

    free(startArr);/*Free the allocated memory.*/
}


