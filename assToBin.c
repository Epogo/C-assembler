#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define NUMBIT 16
#define RCOMLEN 8
#define ICOMLEN 15
#define JCOMLEN 4
#define INITIC 100
#define BITSINBYTE 9
#define SYMBOL_NUM_OF_CHARS 32
#define NUM_OF_BITS_OP 33
#define NUM_OF_CHARS_IN_LINE 80
#define SYMBOL_NUM_OF_CHARS 32
#define NUM_OF_BITS_IMM 17
#define NUM_OF_BITS_IMM_STOP 27


typedef struct data{
    char byte[BITSINBYTE];/*Chars array with length of 8 bits (for a byte) and a NULL terminator*/
    struct data *next;
}DATA;

/*Maybe a union should be added*/
typedef struct memoryImage{
    char symbol[SYMBOL_NUM_OF_CHARS];/*The symbol of a node*/
    char op[NUM_OF_BITS_OP];/*An operation (expressed by binary bits)*/
    DATA *p;/*A pointer to data*/
    int ic;/*Instruction counter.*/
    int dc;
    struct memoryImage *next;
} MEMIM;

struct tableNode{
	char symbol[SYMBOL_NUM_OF_CHARS];
	int value;
	int attribute[2];
	struct tableNode* next;
};
typedef struct tableNode TABLE_NODE_T;

MEMIM *memAdd(char*,char*,char*,TABLE_NODE_T*);
char *Registers(char*);
char *decToBin(int);
char *decToBinJ(int);
char *ascizToBin(int);
char *decToBinDir(char*);
char *decToBinDirW(char*);
char *decToBinDirH(char*);
void deleteNode(MEMIM*);
void addNode(MEMIM *headCom,MEMIM *headData, MEMIM *node);
void printList(MEMIM *head);
char binToHex(char *bin);
void concatNodes(MEMIM *headCom,MEMIM *headData);
TABLE_NODE_T* symbolTable(char *symbol,int value,int attribute1,int attribute2);
void printSymbolTable(TABLE_NODE_T *symbolTable);

char *rCommands[]={"add","sub","and","or","nor","move","mvhi","mvlo"};/*R commands*/
char *iCommands[]={"addi","subi","andi","ori","nori","bne","beq","blt","bgt","lb","sb","lw","sw","lh","sh"};/*I commands*/
char *jCommands[]={"jmp","la","call","stop"};/*J commands*/
char *directives[]={".db",".dw",".dh",".data",".asciz",".entry",".extern"};/*Directives*/
char *rComFunct[]={"00001","00010","00011","00100","00101"};/*A list of functs for r commands*/
char *rOpCode[]={"000000","000001"};/*A list of opcodes for r commands*/
char *iOpCode[]={"001010","001011","001100","001101","001110","001111","010000","010001","010010","010011","010100","010101","010110","010111","011000"};/*A list of opcodes for i commands*/
char *jOpCode[]={"011110","011111","100000","111111"};/*A list of opcodes for j commands*/

int main()
{
    MEMIM *headCom,*headData;
    TABLE_NODE_T *tableHead;
    int ic=INITIC;
    tableHead = symbolTable("YU",122,0,0);
    tableHead = symbolTable("ALL2",187,0,0);
    tableHead = symbolTable("ALL4",210,0,0);
    tableHead = symbolTable("YU2",152,0,0);
    tableHead = symbolTable("ALL6",214,0,0);
    headCom=memAdd(NULL,"add","$3,$5,$9",tableHead);
    headData=memAdd(NULL,".asciz","a",tableHead);
    addNode(headCom,headData,memAdd("YU","add","$4,$8,$9",tableHead));
    addNode(headCom,headData,memAdd("ALL2","jmp","$9",tableHead));
    addNode(headCom,headData,memAdd("ALL3","blt","$4,$5,ALL6",tableHead));
    addNode(headCom,headData,memAdd("ALL4","bne","$7,$8,ALL4",tableHead));
    addNode(headCom,headData,memAdd("YU2","add","$4,$8,$9",tableHead));
    addNode(headCom,headData,memAdd(NULL,"stop",NULL,tableHead));
    concatNodes(headCom,headData);
    printList(headCom);
    deleteNode(headCom);
    return 0;
}

MEMIM *memAdd(char *ptrField1,char *ptrField2,char *ptrField3,TABLE_NODE_T *symTable){
    char *lineStr;/*A pointer to a line*/
    char opString[NUM_OF_BITS_OP];/*Operation string*/
    lineStr = (char *)calloc(NUM_OF_CHARS_IN_LINE, sizeof(char));/*Memory allocation for a line*/
    const char s[2] = ",";/*A comma token*/
    char *token;/*A pointer to a token*/
    char *reg;/*A char pointer to a register*/
    int count=0;/*A counter*/
    char *imm=(char *)calloc(NUM_OF_BITS_IMM, sizeof(char));/*Memory allocation for immediate value (16 bits).*/
    char *immStop=(char *)calloc(NUM_OF_BITS_IMM_STOP, sizeof(char));/*Memory allocation for immediate value (26 bits).*/
    char *immJ;/*A pointer to an immediate value of type j.*/
    MEMIM *node=(MEMIM *)malloc(sizeof(MEMIM));/*Memory allocation for memory image node.*/
    DATA *newNode;/*An adress of a new node*/
    char *registers[3];/*An array of pointers of registers.*/
    char *opStrPoint;/*A pointer to a Operation string*/
    opStrPoint=opString;
    char *notInUse="000000";/*An array of bin chars for "not in use" bits within the 32bits slot*/
    char *emptyReg="00000";/*An array of bin chars for "empty register" bits within the 32bits slot*/
    char *zero="0";/*A "zero" string*/
    char *one="1";/*A "one" string*/
    char *null="00000000";/*NULL terminator*/
    DATA *temp;

    if (ptrField3)
        strcpy(lineStr,ptrField3);/*If the third field isn't empty-copy the content of this field to the lineStr field.*/
    token = strtok(lineStr, s);/*A definition of a token*/
    
    /*Check if the command is a RCOMMAND*/
    for(int i=0;i<RCOMLEN;i++){
        if(!strcmp(ptrField2,rCommands[i])){
            while( token != NULL ) {
                    reg=Registers(token);/*Convert the registers tokens to a binary reg string.*/
                    registers[count]=(char*) malloc(6 * sizeof(char));/*Allocate enough memory to store a reg*/
                    /*If the command is a copy command*/
                    if((i>4)&&(count==1))
                    {
                        strcpy(registers[count],emptyReg);
                        count++;
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
                for (int j=0;j<3;j++)
                {
                    strcat(opStrPoint,registers[j]);
                    free(registers[j]);
                }
                strcat(opStrPoint,rComFunct[i]);/*Copy the suitable funct code to the Operation string.*/
            }
            else{
                strcpy(opStrPoint,rOpCode[1]);
                for (int k=2;k>=0;k--)
                {
                    if (k==1){
                        strcat(opStrPoint,emptyReg);/*Copy the suitable opcode to the Operation string.*/
                        free(registers[k]);
                        continue;
                    }
                    strcat(opStrPoint,registers[k]);
                    free(registers[k]);
                }
                strcat(opStrPoint,rComFunct[i%5]);/*Copy the suitable funct code to the Operation string.*/
            }
            strcat(opStrPoint,notInUse);/*Set the last bits in the operation string as zeros.*/
        }
    }
    
    /*Check if the command is a ICOMMAND*/
    for(int i=0;i<ICOMLEN;i++){
        if(!strcmp(ptrField2,iCommands[i])){
            if(i<5||i>8){
                while( token != NULL ) {
                        if (count==1)
                        {
                            registers[count]=NULL;
                            strcpy(imm,decToBin(atoi(token)));
                            count++;
                            token = strtok(NULL, s);
                            continue;
                        }
                        reg=Registers(token);/*Convert the registers tokens to a binary reg string.*/
                        registers[count]=(char*) malloc(6 * sizeof(char));/*Allocate enough memory to store a reg*/
                        strcpy(registers[count],reg);/*Copy each reg to a place in the registers array*/
                        free(reg);
                        token = strtok(NULL, s);
                        count++;
                }

                strcpy(opStrPoint,iOpCode[i]);/*Copy the suitable opcode to the Operation string.*/
                for (int j=0;j<3;j++)
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
                        while (symTable!=NULL){
                            if (!strcmp(symTable->symbol,token))
                            {
                                imm=decToBin(symTable->value);
                                //printf("\n%s ",immJ);
                            }
                            symTable=symTable->next;
                        }    
                            registers[2]=NULL;
                            break;
                    }
                    reg=Registers(token);/*Convert the registers tokens to a binary reg string.*/
                    registers[count]=(char*) malloc(6 * sizeof(char));
                    strcpy(registers[count],reg);
                    free(reg);
                    token = strtok(NULL, s);
                    count++;
                }
                    
                strcpy(opStrPoint,iOpCode[i]);
                for (int j=0;j<2;j++)
                    {
                        strcat(opStrPoint,registers[j]);
                        free(registers[j]);
                    }
                strcat(opStrPoint,imm);
                free(imm);
            }
        }    
    }
    
    for(int i=0;i<JCOMLEN;i++){
        if(!strcmp(ptrField2,jCommands[i])){
            
            if (i<3){
                strcpy(opStrPoint,jOpCode[i]);
                while (symTable!=NULL){
                    if (!strcmp(symTable->symbol,ptrField3))
                    {
                        immJ=decToBinJ(symTable->value);
                    }
                    symTable=symTable->next;
                }
                if(*ptrField3!='$'){
                        strcat(opStrPoint,zero);
                        strcat(opStrPoint,immJ);
                }
                else{
                        reg=Registers(ptrField3);
                        strcat(opStrPoint,one);
                        strcat(opStrPoint,reg);
                        free(reg);
                }
            }
            else{
                strcpy(opStrPoint,jOpCode[i]);
                strcpy(immStop,decToBinJ(0));
                strcat(immStop,zero);
                strcat(opStrPoint,immStop);
            }
        }

    }
    
    if(!strcmp(ptrField2,".asciz"))
    {
        temp=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for a data node.*/
        node->p=temp;/*Point to the allocated memory.*/
        while (*ptrField3!='\0'){
            int asciCode=*ptrField3;/*Extracting the ascii code of any letter.*/
            char *letter=ascizToBin(asciCode);/*Extracting a suitable string from the asciCode.*/
            strcat(temp->byte,letter);/*Copy the string to byte array within the temp node.*/
            newNode=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for the new node.*/
            temp->next=newNode;/*Point to the new node.*/
            temp=newNode;/*Set the temp node to point on the newNode.*/
            free(letter);/*Free the allocated space for the string*/
            ptrField3++;/*Advance to the next byte.*/
        }
        strcat(temp->byte,null);/*Concatenate the null bits to the end of the ascii linked-list.*/
    }
    
    if(!strcmp(ptrField2,".db"))
    {
        temp=(DATA*)calloc(1, sizeof(DATA));/*Allocate memory for a data node.*/
        node->p=temp;/*Point to the allocated memory.*/
        while( token != NULL ){
            char *binNum=decToBinDir(token);
            strcat(temp->byte,binNum);
            free(binNum);
            token = strtok(NULL, s);
            if (token==NULL)
                break;
            newNode=(DATA*)calloc(1, sizeof(DATA));
            temp->next=newNode;
            temp=newNode;
        }
        newNode->next=NULL;
    }
    
    if(!strcmp(ptrField2,".dw"))
    {
        temp=(DATA*)calloc(1, sizeof(DATA));
        node->p=temp;
        char *binNum,*binNumStart;
        while( token != NULL ){
            binNum=decToBinDirW(token);
            binNumStart=binNum;
            binNum+=24;
            strncpy(temp->byte,binNum,8);
            //printf("temp->byte is:%s\n",temp->byte);
            for(int i=0;i<3;i++){
                newNode=(DATA*)calloc(1, sizeof(DATA));
                temp->next=newNode;
                temp=newNode;
                binNum-=8;
                strncpy(temp->byte,binNum,8);
                //printf("temp->byte is:%s\n",temp->byte);
            }
            token = strtok(NULL, s);
            if (token != NULL)
            {
                newNode=(DATA*)calloc(1, sizeof(DATA));
                temp->next=newNode;
                temp=newNode;
            }
            free(binNumStart);
        }

    }
    
    if(!strcmp(ptrField2,".dh"))
    {
        temp=(DATA*)calloc(1, sizeof(DATA));
        node->p=temp;
        DATA *head;
        char *binNum,*binNumStart;
        while( token != NULL ){
            binNum=decToBinDirH(token);
            binNumStart=binNum;
            binNum+=8;
            strncpy(temp->byte,binNum,8);
            newNode=(DATA*)calloc(1, sizeof(DATA));
            temp->next=newNode;
            temp=newNode;
            binNum-=8;
            strncpy(temp->byte,binNum,8);
            token = strtok(NULL, s);
            if (token != NULL)
            {
                newNode=(DATA*)calloc(1, sizeof(DATA));
                temp->next=newNode;
                temp=newNode;
            }
            free(binNumStart);
        }
        
    }
    strcpy(node->op,opString);
    return node;
    free(lineStr);
    count=0;
}
char *Registers(char *reg)
{
    int num;
    char *temp;
    int i,j;
    char *str=(char*)malloc(6);
    temp=reg;
    if (reg[0]!='$'){
        /*printf("Not a register!");
    	exit(EXIT_FAILURE);*/
    	return NULL;
    }
    temp++;
    num=atoi(temp);
    if (num>31)
    {
        printf("Error: register can be Ri, with i=0,1,...,31\n");
        exit(EXIT_FAILURE);
    }
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
    int i;
    char *str=(char*)malloc(17);/*Allocate memory for a binary immediate representation*/
    for(unsigned int i=0; i<16; i++)
    {
      unsigned int mask = 1 << (16 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[16] = '\0';
    return str;

}

char *decToBinJ(int num)
{
    int i;
    char *str=(char*)malloc(26);/*Allocate memory for a binary immediate representation*/
    for(unsigned int i=0; i<25; i++)
    {
      unsigned int mask = 1 << (25 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[25] = '\0';
    return str;
}

char *ascizToBin(int num)
{
    int i;
    char *str=(char*)malloc(9);/*Allocate memory for byte representation*/
    for(unsigned int i=0; i<8; i++)
    {
      unsigned int mask = 1 << (8 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[8] = '\0';/*Put a zero at the end of the byte string*/
    return str;
}

char *decToBinDir(char *number)
{
    int num;
    int i;
    char *str=(char*)malloc(9);/*Allocate memory for byte representation*/
    num=atoi(number);
    for(unsigned int i=0; i<8; i++)
    {
      unsigned int mask = 1 << (8 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      str[i] = (num & mask) ? '1' : '0';
    }
    str[8] = '\0';
    return str;
}

char *decToBinDirW(char *number)
{
    int num;
    int i;
    char *str=(char*)calloc(33,sizeof(char));/*Allocate memory for word representation*/
    num=atoi(number);
    for(unsigned int i=0; i<32; i++)
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
    int i;
    char *str=(char*)malloc(17);/*Allocate memory for a binary immediate representation*/
    num=atoi(number);
    for(unsigned int i=0; i<32; i++)
    {
      unsigned int mask = 1 << (32 - 1 - i);/*Implement a mask in order to turn on appropriate bits*/
      if(i<16)
        continue;
      str[i-16] = (num & mask) ? '1' : '0';
    }
    str[16] = '\0';
    return str;
}

void addNode(MEMIM *headCom,MEMIM *headData, MEMIM *node)
{
    MEMIM *nodePointer;/*A pointer to a memory image node*/
    /*Add a new node the Commands list.*/
    if (node->p==NULL){
        nodePointer=headCom;
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        nodePointer->next=node;
    }
    /*Add a new node the Directives list.*/
    else{
        nodePointer=headData;
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        nodePointer->next=node;
    }
    
}

void concatNodes(MEMIM *headCom,MEMIM *headData){
        MEMIM *nodePointer;/*A pointer to a memory image node*/
        nodePointer=headCom;
        /*Concatenate commands list and directives list*/
        while(nodePointer->next!=NULL)
        {
            nodePointer = nodePointer->next;
        }
        nodePointer->next=headData;
}

void printList (MEMIM *head)
{
    MEMIM *q;
    q=head;
    char *bin;
    char mem[5];
    char* printArr=(char *) malloc(8);
    char *startArr;
    startArr=printArr;
    int j=0;
    int k=0;
    int count=0;
    int inCount;
    int lastNodeFlag=0;
    char hex;
    DATA *temp;
    static int ic=100;
    while(q!=NULL)
    {
        if (q->next==NULL)
            lastNodeFlag=1;
        //printf("%d ",q->address);
        if((q->p)!=NULL){
            k=0;
            int bitsNum;
            while((q->p)!=NULL){
                bin=q->p->byte;
                j=0;
                for(int i=0;i<8;i++){
                    mem[j]=*bin;
                    if((i+1)%4==0){
                        mem[4]='\0';
                        hex=binToHex(mem);
                        j=0;
                        printArr[k++]=hex;
                        //printf("hex is:%c, k is:%d\n", hex, k);
                        bin++;
                        continue;
                    }
                    j++;
                    bin++;
                }
                if (k%8==0){
                    printArr = (char *) realloc(printArr, k+8);
                }
                temp=q->p;
                //printf("%s\n",q->p->byte);
                q->p=q->p->next;
                free(temp);
            }
            //printf("k is:%d\n", k);
            inCount=0;
            if (count==0)
                printf("%d ",ic);
            for(int i=0;i<k;i+=2){
                printf("%c",printArr[i]);
                printf("%c ",printArr[i+1]);
                inCount+=2;
                count+=2;
                //printf(" count: %d k is:%d ",count,k);
                if ((count%8==0)){
                    printf("\n");
                    //printf(" count: %d k is:%d ",count,k);
                    ic+=4;
                    if ((lastNodeFlag==1)&&(inCount==k))
                        break;
                    printf("%d ",ic);
                    
                }
                   
            }
        }
        else
            {
            bin=q->op;
            k=0;
            for(int i=0;i<32;i++){
                mem[j]=*bin;
                if((i+1)%4==0){
                mem[4]='\0';
                //printf("%s ",mem);
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
            int count=0;
            printf("%d ",ic);
            for(k=7;k>0;){
                count++;
                printf("%c",printArr[k-1]);
                printf("%c ",printArr[k]);
                if (count%8==0){
                    printf("\n");
                    printf("%d ",ic);
                }
                k-=2;

            }
            printf("\n");
            ic+=4;
        }
           
        q=q->next;
    }
    free(startArr);
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
        free(temp);
    }
}

void printSymbolTable(TABLE_NODE_T *symbolTable){
        TABLE_NODE_T *q;
        q=symbolTable;
        while(q->next!=NULL)
        {
            printf("%s\n",q->symbol);
            q = q->next;
        }
}

char binToHex(char *bin)
{
   char hex;/*The hex value which will be returned as a char*/
   int count=3;/*Intialize the counter*/
   int num=0;/*Initialize the num*/
   /*While the bin string is not null-continue to sum numbers*/
   for(char *i=bin;*i;i++){
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

TABLE_NODE_T* symbolTable(char *symbol,int value,int attribute1,int attribute2){
	/*printf("\n%s\n",symbol);*/

	TABLE_NODE_T *ptrTableNode; /*initialize pointer to TABLE_NODE_T, variable ptrTableNode*/
	TABLE_NODE_T *tmpPtr; /*initialize pointer to tmpPtr of type TABLE_NODE_T*/
	static TABLE_NODE_T *current;
	static TABLE_NODE_T *head;
	static int firstSymbolFlag = 1;
	
	tmpPtr = (TABLE_NODE_T*)calloc(1, sizeof(TABLE_NODE_T));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrTableNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	if(firstSymbolFlag == 0){
		current->next = ptrTableNode;
	}

	if(firstSymbolFlag == 1){
		head = ptrTableNode;
	}

	strcpy(ptrTableNode->symbol,symbol);
	ptrTableNode->value = value;
	ptrTableNode->attribute[0] = attribute1;
	ptrTableNode->attribute[1] = attribute2;
	ptrTableNode->next = NULL;

	current = ptrTableNode;
	
	firstSymbolFlag = 0;

	return head;

}
