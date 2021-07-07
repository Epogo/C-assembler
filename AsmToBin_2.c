#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMBIT 16
#define RCOMLEN 8
#define ICOMLEN 15
#define JCOMLEN 4

void firstPass(char*,char*,char*);
char *Registers(char*);
char *decToBin(char*);
char *decToBinJ(char*);
char *ascizToBin(int);
char *decToBinDir(char*);

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
    firstPass(NULL,".db","6,-9" );
    return 0;
}

void firstPass(char *ptrField1,char *ptrField2,char *ptrField3){
    char *str;
    
    str = (char *)calloc(80, sizeof(char));
    const char s[2] = ",";
    char *token;
    char *reg;
    int count=0;
    char *imm=(char *)calloc(17, sizeof(char));
    char *imm2=(char *)calloc(25, sizeof(char));
    char *registers[3];
    
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
                printf("%s",rOpCode[0]);
                for (int j=0;j<3;j++)
                {
                    printf(" %s",registers[j]);
                    free(registers[j]);
                }
                printf(" %s",rComFunct[i]);
            }
            else{
                printf("%s",rOpCode[1]);
                for (int k=2;k>=0;k--)
                {
                    if (k==1){
                        printf(" %s","00000");
                        free(registers[k]);
                        continue;
                    }
                    printf(" %s",registers[k]);
                    free(registers[k]);
                }
            }
            printf("%s"," 000000");
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
                    
                printf("%s",iOpCode[i]);
                for (int j=0;j<3;j++)
                    {
                        if (j==1)
                            continue;
                        printf(" %s",registers[j]);
                        free(registers[j]);
                    }
                    printf(" %s",imm);
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
                    
                printf("%s",iOpCode[i]);
                for (int j=0;j<2;j++)
                    {
                        printf(" %s",registers[j]);
                        free(registers[j]);
                    }
                printf(" %s",imm);
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
                printf("0%s ",imm2);
            }
        }
    }
    
    if(!strcmp(ptrField2,".asciz"))
    {
        while (*ptrField3!='\0'){
            int asciCode=*ptrField3;
            char *s=ascizToBin(asciCode);
            printf("%s\n",s);
            free(s);
            ptrField3++;
        }
        printf("00000000");
    }
    
    if(!strcmp(ptrField2,".db"))
    {
        while( token != NULL ) {
            char *s=decToBinDir(token);
            printf("%s\n",s);
            free(s);
            token = strtok(NULL, s);
        }
    }
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

char *dec2hex(int num) {
    int i = 1, j, temp, quotient;
    quotient = num;
    char *arr=(char*)malloc(8);

    while(quotient!=0) {
        temp = quotient % 16;
        //To convert integer into character
        if( temp < 10)
            temp = temp + 48; 
        else
            temp = temp + 55;
        arr[i++]= temp;
        quotient = quotient / 16;
    }

    for (j = i - 1; j > 0; j--)
        printf("%c", arr[j]);
}
