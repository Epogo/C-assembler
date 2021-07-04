#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUMBIT 16
void firstPass(char*,char*);
char *Registers(char*);
int main()
{
    firstPass("add","$4,$5,$6");
    return 0;
}

void firstPass(char *ptrField1,char *ptrField2){
    char *str;
    str = (char *)calloc(80, sizeof(char));
    printf("%s ",ptrField1);
    strcpy(str,ptrField2);
    const char s[2] = ",";
    char *token;
    char *reg[3];
    char const reg2[6];
    int i=0;
    token = strtok(str, s);
    reg[i++]=Registers(token);
    
        if(!strcmp(ptrField1,"add")){
          while( token != NULL ) {
            printf( "%s ", reg[i]);
            token = strtok(NULL, s);
            reg[i]=Registers(token);
        }
    }
}

char *Registers(char *reg)
{
    int num;
    char *temp;
    int i,j;
    char *str=(char*)malloc(6);
    temp=reg;
    if (reg[0]!='$'){
    	return;	
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
