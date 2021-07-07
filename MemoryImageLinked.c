#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct memoryImage{
    int address;
    char op[5];
    struct memoryImage *next;
} memIm;

char *str[]={"0000","0001","0010","0011","0100"};
int main()
{
    int add=100;
    memIm *head;
    memIm *current;
    memIm *temp;
    
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
        printf("%s",head->op);
        printf("\n");
        head=head->next;
        free(temp);
        if (head->next==NULL)
            break;
        temp=head;
    }

    return 0;
}
