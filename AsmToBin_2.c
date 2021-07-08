#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMBIT 16
#define RCOMLEN 8
#define ICOMLEN 15
#define JCOMLEN 4

typedef struct memoryImage{
    int address;
    char op[33];
    struct memoryImage *next;
} memIm;

memIm *firstPass(char*,char*,char*);
char *Registers(char*);
char *decToBin(char*);
char *decToBinJ(char*);
char *ascizToBin(int);
char *decToBinDir(char*);
char *decToBinDirW(char*);
char *decToBinDirH(char*);
void deleteNode(memIm*);
void addNode(memIm *head, memIm *node);
void printList(memIm *head);


char *rCommands[]={"add","sub","and","or","nor","move","mvhi","mvlo"};
char *iCommands[]={"addi","subi","andi","ori","nori","bne","beq","blt","bgt","lb","sb","lw","sw","lh","sh"};
char *jCommands[]={"jmp","la","call","stop"};
char *directives[]={".db",".dw",".dh",".data",".asciz",".entry",".extern"};
char *rComFunct[]={"00001","00010","00011","00100","00101"};
char *rOpCode[]={"000000","000001"};
char *iOpCode[]={"001010","001011","001100","001101","001110","001111","010000","010001","010010","010011","010100","010101","010110","010111","011000"};
char *jOpCode[]={"011110","011111","100000","111111"};



int main()
{
    char *pointer;
    memIm *head;
    /*firstPass(NULL,".dh","27056");
    firstPass(NULL,"stop",NULL);
    firstPass(NULL,"addi","$4,-12,$17");
    firstPass(NULL,"lw","$0,4,$10");
    firstPass(NULL,"mvlo","$5,$10");
    firstPass(NULL,".asciz","AbZ");
    firstPass(NULL,"bgt","$5,$6,LOOP");*/
    //firstPass(NULL,"bgt","$7,$12,64");
    head=firstPass(NULL,"add","$3,$5,$9");
    addNode(head,firstPass(NULL,"move","$23,$2"));
    addNode(head,firstPass(NULL,"addi","$23,11,$2"));
    addNode(head,firstPass(NULL,"lw","$23,-2,$2"));
    printList(head);
    deleteNode(head);

    return 0;
}

memIm *firstPass(char *ptrField1,char *ptrField2,char *ptrField3){
    char *str;
    char string[33];
    str = (char *)calloc(80, sizeof(char));
    const char s[2] = ",";
    char *token;
    char *reg;
    int count=0;
    char *imm=(char *)calloc(17, sizeof(char));
    char *imm2=(char *)calloc(25, sizeof(char));
    memIm *node=(memIm *)malloc(sizeof(memIm));
    char *registers[3];
    char *pointer;
    pointer=string;
    char *notInUse="000000";
    char *emptyPointer="00000";
    
    if (ptrField3)
        strcpy(str,ptrField3);
    token = strtok(str, s);
    for(int i=0;i<RCOMLEN;i++){
        if(!strcmp(ptrField2,rCommands[i])){
            while( token != NULL ) {
                    reg=Registers(token);
                    registers[count]=(char*) malloc(6 * sizeof(char));
                    if((i>4)&&(count==1))
                    {
                        strcpy(registers[count],"00000");
                        count++;
                        continue;
                    }
                    strcpy(registers[count],reg);
                    free(reg);
                    token = strtok(NULL, s);
                    count++;
            }
            if(i<5){
                //printf("%s",rOpCode[0]);
                strcpy(pointer,rOpCode[0]);
                pointer+=strlen(rOpCode[0]);
                for (int j=0;j<3;j++)
                {
                    //printf(" %s",registers[j]);
                    strcpy(pointer,registers[j]);
                    pointer+=strlen(registers[j]);
                    free(registers[j]);
                }
                //printf(" %s",rComFunct[i]);
                strcpy(pointer,rComFunct[i]);
                pointer+=strlen(rComFunct[i]);
                
                
            }
            else{
                //printf("%s",rOpCode[1]);
                strcpy(pointer,rOpCode[1]);
                pointer+=strlen(rOpCode[1]);
                for (int k=2;k>=0;k--)
                {
                    if (k==1){
                        //printf(" %s","00000");
                        strcpy(pointer,emptyPointer);
                        pointer+=strlen(emptyPointer);
                        free(registers[k]);
                        continue;
                    }
                    //printf(" %s",registers[k]);
                    strcpy(pointer,registers[k]);
                    pointer+=strlen(registers[k]);
                    free(registers[k]);
                }
                //printf(" %s",rComFunct[i%5]);
                strcpy(pointer,rComFunct[i%5]);
                pointer+=strlen(rComFunct[i%5]);
            }
            //printf("%s"," 000000\n");
            strcpy(pointer,notInUse);
            //*pointer='\0';
        }
    }

    for(int i=0;i<ICOMLEN;i++){
        if(!strcmp(ptrField2,iCommands[i])){
            if(i<5||i>8){
                while( token != NULL ) {
                        if (count==1)
                        {
                            registers[count]=NULL;
                            strcpy(imm,decToBin(token));
                            count++;
                            token = strtok(NULL, s);
                            continue;
                        }
                        reg=Registers(token);
                        registers[count]=(char*) malloc(6 * sizeof(char));
                        strcpy(registers[count],reg);
                        free(reg);
                        token = strtok(NULL, s);
                        count++;
                }
                    
                //printf("%s ",iOpCode[i]);
                strcpy(pointer,iOpCode[i]);
                pointer+=strlen(iOpCode[i]);
                for (int j=0;j<3;j++)
                    {
                        if (j==1)
                            continue;
                        //printf(" %s",registers[j]);
                        strcpy(pointer,registers[j]);
                        pointer+=strlen(registers[j]);
                        free(registers[j]);
                    }
                    //printf(" %s\n",imm);
                    strcpy(pointer,imm);
                    pointer+=strlen(imm);
                    free(imm);
            }
            else{
                while( token != NULL ) {
                        if (count==2){
                            if (*token>='A'&&*token<='Z')
                                strcpy(imm,"?");
                            else
                                strcpy(imm,decToBin(token));
                            registers[2]=NULL;
                            break;
                        }
                        reg=Registers(token);
                        registers[count]=(char*) malloc(6 * sizeof(char));
                        strcpy(registers[count],reg);
                        free(reg);
                        token = strtok(NULL, s);
                        count++;
                }
                    
                //printf("%s",iOpCode[i]);
                strcpy(pointer,iOpCode[i]);
                pointer+=strlen(iOpCode[i]);
                for (int j=0;j<2;j++)
                    {
                        //printf(" %s",registers[j]);
                        strcpy(pointer,registers[j]);
                        pointer+=strlen(registers[j]);
                        free(registers[j]);
                    }
                strcpy(pointer,imm);
                pointer+=strlen(imm);
                free(imm);
            }
        }    
    }
    
    /*for(int i=0;i<JCOMLEN;i++){
        if(!strcmp(ptrField2,jCommands[i])){
            if (i<3){
                strcpy(imm2,decToBinJ(ptrField3));
            }
            else
                strcpy(imm2,decToBinJ("0"));
            printf("%s ",jOpCode[i]);
            printf("0%s",imm2);
        }
    }*/
    
    for(int i=0;i<JCOMLEN;i++){
        if(!strcmp(ptrField2,jCommands[i])){
            if (i<3){
                printf("%s ",jOpCode[i]);
                if(*ptrField3!='$')
                    printf("0 ?");
                else
                    printf("1 ?");
            }
            else{
                printf("%s ",jOpCode[i]);
                strcpy(imm2,decToBinJ("0"));
                printf("0%s \n",imm2);
            }
        }
    }
    
    if(!strcmp(ptrField2,".asciz"))
    {
        while (*ptrField3!='\0'){
            int asciCode=*ptrField3;
            char *letter=ascizToBin(asciCode);
            printf("%s\n",letter);
            free(letter);
            ptrField3++;
        }
        printf("00000000\n");
    }
    
    if(!strcmp(ptrField2,".db"))
    {
        while( token != NULL ) {
            char *binNum=decToBinDir(token);
            printf("%s\n",binNum);
            token = strtok(NULL, s);
            free(binNum);
        }
        
    }
    if(!strcmp(ptrField2,".dw"))
    {
        while( token != NULL ) {
            char *binNum=decToBinDirW(token);
            printf("%s\n",binNum);
            token = strtok(NULL, s);
            free(binNum);
        }
        
    }
    
    if(!strcmp(ptrField2,".dh"))
    {
        while( token != NULL ) {
            char *binNum=decToBinDirH(token);
            printf("%s\n",binNum);
            token = strtok(NULL, s);
            free(binNum);
        }
        
    }
    strcpy(node->op,string);
    return node;
    free(str);
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

char *decToBin(char *number)
{
    int num;
    int i,j;
    char *str=(char*)malloc(16);
    num=atoi(number);
    for(unsigned int i=0; i<16; i++)
    {
      unsigned int mask = 1 << (16 - 1 - i);
      str[i] = (num & mask) ? '1' : '0';
    }
    str[16] = '\0';
    return str;

}

char *decToBinJ(char *number)
{
    int num;
    int i,j;
    char *str=(char*)malloc(24);
    num=atoi(number);
    for(unsigned int i=0; i<24; i++)
    {
      unsigned int mask = 1 << (24 - 1 - i);
      str[i] = (num & mask) ? '1' : '0';
    }
    str[24] = '\0';
    return str;
}

char *ascizToBin(int num)
{
    int i,j;
    char *str=(char*)malloc(8);
    for(unsigned int i=0; i<8; i++)
    {
      unsigned int mask = 1 << (8 - 1 - i);
      str[i] = (num & mask) ? '1' : '0';
    }
    str[8] = '\0';
    return str;
}

char *decToBinDir(char *number)
{
    int num;
    int i,j;
    char *str=(char*)malloc(8);
    num=atoi(number);
    for(unsigned int i=0; i<8; i++)
    {
      unsigned int mask = 1 << (8 - 1 - i);
      str[i] = (num & mask) ? '1' : '0';
    }
    str[8] = '\0';
    return str;
}

char *decToBinDirW(char *number)
{
    int num;
    int i,j;
    char *str=(char*)malloc(32);
    num=atoi(number);
    for(unsigned int i=0; i<32; i++)
    {
      unsigned int mask = 1 << (32 - 1 - i);
      str[i] = (num & mask) ? '1' : '0';
    }
    str[32] = '\0';
    return str;
}

char *decToBinDirH(char *number)
{
    int num;
    int i,j;
    char *str=(char*)malloc(16);
    num=atoi(number);
    for(unsigned int i=0; i<32; i++)
    {
      unsigned int mask = 1 << (32 - 1 - i);
      if(i<16)
        continue;
      str[i-16] = (num & mask) ? '1' : '0';
    }
    str[16] = '\0';
    return str;
}

void addNode(memIm *head, memIm *node)
{
    memIm *q;
    q=head;
    while(q->next!=NULL)
    {
        q = q->next;
    }
    q->next=node;
}

void printList(memIm *head)
{
    memIm *q;
    q=head;
    while(q!=NULL)
    {
        printf("%s\n",q->op);
        q=q->next;
    }

}

void deleteNode(memIm *node){
    memIm *temp;
    temp=node;
    while(1){
        temp=node;
        if (temp==NULL)
            break;
        node=node->next;
        free(temp);
    }
}



