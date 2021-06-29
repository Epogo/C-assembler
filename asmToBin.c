#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBIT 16

void Registers(char reg[NUMBIT]);
void DecToBin(char reg[NUMBIT]);
void regScan(char *op,char *rs, char *rt, char *rd, FILE *fpin,FILE *fout);

int main(int argc, char *argv[])
{
    char code[NUMBIT],
         rs[NUMBIT],
         rt[NUMBIT],
         rd[NUMBIT];
    int b[NUMBIT], dec, i, j, cnt = 0,cp,command;
    char *commands[]={"add","sub","and","or","nor","move","mvhi","mvlo","addi"};
    FILE *fpin, *fpout;

    if (argc != 3)
    {
        printf("encoder <input_file.ext> <output_file.ext>\n");
        exit(EXIT_FAILURE);
    }
    if ((fpin = fopen(argv[1], "r")) == NULL)
    {
        printf("Error opening input file\n");
        exit(EXIT_FAILURE);
    }
    if ((fpout = fopen(argv[2], "w")) == NULL)
    {
        printf("Error opening output file\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fpin, "%s", code) != EOF)
    {
	for (i=0; i<9; i++){
		if (!strcmp(code, commands[i])){
			command=i;
			break;
		}
	}
	regScan(code,rs,rt,rd,fpin,fpout);		
	switch(command) 
		{
		    case 0:
		    fprintf(fpout, "Bin: 000000 %s %s %s 00001 000000\n",
		           rs, rt, rd);
		    break;
		    case 1:
		    fprintf(fpout, "Bin: 000000 %s %s %s 00010 000000\n",
		           rs, rt, rd);
		    break;
		    case 2:
		    fprintf(fpout, "Bin: 000000 %s %s %s 00011 000000\n",
		           rs, rt, rd);
		    case 3:
		    fprintf(fpout, "Bin: 000000 %s %s %s 00100 000000\n",
		           rs, rt, rd);
		    case 4:
		    fprintf(fpout, "Bin: 000000 %s %s %s 00101 000000\n",
		           rs, rt, rd);
		    case 5:
		    fprintf(fpout, "Bin: 000001 %s 00000 %s 00001 000000\n",
		           rs, rt, rd);
		    case 6:
		    fprintf(fpout, "Bin: 000001 %s 00000 %s 00010 000000\n",
		           rs, rt, rd);
		    case 7:
		    fprintf(fpout, "Bin: 000001 %s 00000 %s 00011 000000\n",
		           rs, rt, rd);
		    break;
                    case 8:
                    DecToBin(rd);
		    fprintf(fpout, "Bin: 001010 %s %s %s\n", rs, rt, rd);
		    break;
		    default:
		    break;
		                                     
		}

		           	
     }
	 

    fclose(fpin);
    fclose(fpout);

    return EXIT_SUCCESS;
}

void Registers(char reg[NUMBIT])
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
    strcpy(reg,str);
    free(str);
    return;
}

void regScan(char *op,char *rs, char *rt, char *rd, FILE *fpin, FILE *fpout)
{
            fscanf(fpin, "%s", rs);
            fscanf(fpin, "%s", rt);
            fscanf(fpin, "%s", rd);

	    fprintf(fpout, "%s %s,%s,%s\t",op, rs, rt, rd);

            Registers(rs);
            Registers(rt);
            Registers(rd);
	
}



void DecToBin(char reg[NUMBIT])
{
    int num;
    int i,j;
    char *str=(char*)malloc(16);
    num=atoi(reg);
    str[15]='\0';
    j=14;
    while (j>=0)
	   {
		str[j--]=num%2+'0';
                num=num/2;
                i++;		
	   }
    strcpy(reg,str);
    free(str);
    return;

}
