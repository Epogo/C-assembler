#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBIT 16

void Registers(char reg[NUMBIT]);
void DecToBin(int bin[NUMBIT], int dec, int n);
void regScan(char *op,char *rs, char *rt, char *rd, FILE *fpin,FILE *fout);

int main(int argc, char *argv[])
{
    char code[NUMBIT],
         rs[NUMBIT],
         rt[NUMBIT],
         rd[NUMBIT];
    int b[NUMBIT], dec, i, j, cnt = 0,cp,command;
    char *commands[]={"add","sub","and","or","nor","move","mvhi","mvlo"};
    FILE *fpin, *fpout;

    if (argc != 3)
    {
        printf("USAGE: MIPS32_encoder <input_file.ext> <output_file.ext>\n");
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
	for (i=0; i<3; i++){
		if (!strcmp(code, commands[i])){
			command=i;
			break;
		}
	}
	regScan(code,rs,rt,rd,fpin,fpout);		
        switch(command) 
        {
	    case 0:
            fprintf(fpout, "Bin: 000000%s%s%s000001000000\n",
                   rs, rt, rd);
	    break;
	    case 1:
	    fprintf(fpout, "Bin: 000000%s%s%s000010000000\n",
                   rs, rt, rd);
	    break;
	    case 2:
	    fprintf(fpout, "Bin: 000000%s%s%s000011000000\n",
                   rs, rt, rd);
	    case 3:
	    fprintf(fpout, "Bin: 000000%s%s%s000100000000\n",
                   rs, rt, rd);
	    case 4:
	    fprintf(fpout, "Bin: 000000%s%s%s000101000000\n",
                   rs, rt, rd);
	    case 5:
	    fprintf(fpout, "Bin: 000001%s%s%s000001000000\n",
                   rs, rt, rd);
	    case 6:
	    fprintf(fpout, "Bin: 000001%s%s%s000010000000\n",
                   rs, rt, rd);
	    case 7:
	    fprintf(fpout, "Bin: 000001%s%s%s000011000000\n",
                   rs, rt, rd);
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
    char *p=reg;
    p++;
    
    if (!strcmp(p, "0"))
        strcpy(reg, "00000");
    else if (!strcmp(p, "1"))
        strcpy(reg, "00001");
    else if (!strcmp(p, "2"))
        strcpy(reg, "00010");
    else if (!strcmp(p, "3"))
        strcpy(reg, "00011");
    else if (!strcmp(p, "4"))
        strcpy(reg, "00100");
    else if (!strcmp(p, "5"))
        strcpy(reg, "00101");
    else if (!strcmp(p, "6"))
        strcpy(reg, "00110");
    else if (!strcmp(p, "7"))
        strcpy(reg, "00111");
    else if (!strcmp(p, "8"))
        strcpy(reg, "01000");
    else if (!strcmp(p, "9"))
        strcpy(reg, "01001");
    else if (!strcmp(p, "10"))
        strcpy(reg, "01010");
    else if (!strcmp(p, "11"))
        strcpy(reg, "01011");
    else if (!strcmp(p, "12"))
        strcpy(reg, "01100");
    else if (!strcmp(p, "13"))
        strcpy(reg, "01101");
    else if (!strcmp(p, "14"))
        strcpy(reg, "01110");
    else if (!strcmp(p, "15"))
        strcpy(reg, "01111");
    else if (!strcmp(p, "16"))
        strcpy(reg, "10000");
    else if (!strcmp(p, "17"))
        strcpy(reg, "10001");
    else if (!strcmp(p, "18"))
        strcpy(reg, "10010");
    else if (!strcmp(p, "19"))
        strcpy(reg, "10011");
    else if (!strcmp(p, "20"))
        strcpy(reg, "10100");
    else if (!strcmp(p, "21"))
        strcpy(reg, "10101");
    else if (!strcmp(p, "22"))
        strcpy(reg, "10110");
    else if (!strcmp(p, "23"))
        strcpy(reg, "10111");
    else if (!strcmp(p, "24"))
        strcpy(reg, "11000");
    else if (!strcmp(p, "25"))
        strcpy(reg, "11001");
    else if (!strcmp(p, "26"))
        strcpy(reg, "11010");
    else if (!strcmp(p, "27"))
        strcpy(reg, "11011");
    else if (!strcmp(p, "28"))
        strcpy(reg, "11100");
    else if (!strcmp(p, "29"))
        strcpy(reg, "11101");
    else if (!strcmp(p, "30"))
        strcpy(reg, "11110");
    else if (!strcmp(p, "31"))
        strcpy(reg, "11111");
    else
    {
        printf("Error: register can be Ri, with i=0,1,...,31\n");
        exit(EXIT_FAILURE);
    }

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
void DecToBin(int bin[NUMBIT], int dec, int n)
{
    int i, k, j = n - 1;

    for (i = 0; i < n; i++)
    {
        if (dec % 2 == 0)
            bin[i] = 0;
        else
            bin[i] = 1;

        dec /= 2;
    }

    for (i = 0; i < n / 2; i++)
    {
        k = bin[i];
        bin[i] = bin[j];
        bin[j] = k;
        j--;
    }
}
