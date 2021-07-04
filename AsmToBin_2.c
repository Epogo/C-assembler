#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMBIT 16
#define RCOMLEN 8

void firstPass(char*,char*,char*);
char *Registers(char*);
char *decToBin(char*);

char *rCommands[]={"add","sub","and","or","nor","move","mvhi","mvlo"};
char *rComFunct[]={"00001","00010","00011","00100","00101"};
char *rOpCode[]={"000000","000001"};
int main()
{
    firstPass(NULL,"sub","$4,$5,$6");
    return 0;
}

void firstPass(char *ptrField1,char *ptrField2,char *ptrField3){
    char *str;
    str = (char *)calloc(80, sizeof(char));
    strcpy(str,ptrField3);
    const char s[2] = ",";
    char *token;
    char *reg;
    int count=0;
    char *imm;
    token = strtok(str, s);
    for(int i=0;i<RCOMLEN;i++){
        if(!strcmp(ptrField2,rCommands[i])){
            if(i<5)
                printf("%s ",rOpCode[0]);
            else
                printf("%s ",rOpCode[1]); 
            while( token != NULL ) {
                if (count!=1){
                    reg=Registers(token);
                    printf( "%s ", reg);
                    free(reg);
                }
                else
                    printf("%s","00000 ");
                token = strtok(NULL, s);
                if (i>4)
                    count++;
            }
            printf("%s",rComFunct[(i+5)%5]);
            printf("%s"," 000000");
        }
    }
     /*   else if(!strcmp(ptrField2,"addi")){
            while(token!=NULL){
                if (count!=1)
                    reg=Registers(token);
                printf( "%s ", reg);
                if (count==1){
                    imm=decToBin(token);
                }
                if (count!=1)
                    free(reg);
                token = strtok(NULL, s);
                count++;
            }
        }*/
    
    //printf( "%s ", imm);
    //free(imm);
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
        printf("Not a register!");
    	exit(EXIT_FAILURE);	
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
    char *str=(char*)malloc(17);
    num=atoi(number);
    str[16]='\0';
    j=15;
    while (j>=0)
	   {
		str[j--]=num%2+'0';
                num=num/2;
                i++;		
	   }
    return str;

}
