#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memoryImage{
    int address;
    char op[32];
    struct memoryImage *next;
} memIm;

char binToHex(char*);

char *str[]={"00000110111101110101111111111110","11100110111101110101111111111011","00001110111101110101111111111011","00001110111101110101111111111011","00000111111101110101111111111010"};
int main()
{
    int add=100;
    memIm *head;
    memIm *current;
    memIm *temp;
    char *bin;
    char mem[5];
    int j=0;
    char hex;
    
    head=(struct memoryImage*) malloc(sizeof(memIm));
    current=head;
    for (int i=0;i<5;i++)
    {
        current->address=add;
        strcpy(current->op,str[i]);
        temp=(struct memoryImage*) malloc(sizeof(memIm));
        add+=4;
        current->next=temp;
        current=temp;
        
    }
    temp=head;
    while (1){
        printf("%d ",head->address);
        printf("%s ",head->op);
        bin=head->op;
        for(int i=0;i<32;i++){
            mem[j]=*bin;
            if((i+1)%4==0){
                mem[4]='\0';
                //printf("%s ",mem);
                hex=binToHex(mem);
                j=0;
                printf("%c",hex);
                if((i+1)%8==0)
                    printf(" ");
                bin++;
                continue;
            }
            j++;
            bin++;
        }
        printf("\n");
        head=head->next;
        free(temp);
        if (head->next==NULL)
            break;
        temp=head;
    }

    return 0;
}

char binToHex(char *bin)
{
    char hex;
    if(!strcmp(bin, "0000"))
        hex='0';
    else if(!strcmp(bin, "0001"))
        hex='1';
    else if(!strcmp(bin, "0010"))
        hex='2';
    else if(!strcmp(bin, "0011"))
        hex='3';
    else if(!strcmp(bin, "0100"))
        hex='4';
    else if(!strcmp(bin, "0101"))
        hex='5';
    else if(!strcmp(bin, "0110"))
        hex='6';
    else if(!strcmp(bin, "0111"))
        hex='7';
    else if(!strcmp(bin, "1000"))
        hex='8';
    else if(!strcmp(bin, "1001"))
        hex='9';
    else if(!strcmp(bin, "1010"))
        hex='A';
    else if(!strcmp(bin, "1011"))
        hex='B';
    else if(!strcmp(bin, "1100"))
        hex='C';
    else if(!strcmp(bin, "1101"))
        hex='D';
    else if(!strcmp(bin, "1110"))
        hex='E';
    else if(!strcmp(bin, "1111"))
        hex='F';
    return hex;
}
