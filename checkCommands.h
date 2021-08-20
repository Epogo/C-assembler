
#define NUMRCOM1 5 /*number of R commands of type 1*/
#define NUMRCOM2 3 /*number of R commands of type 2*/
#define NUMICOM1 11 /*number of I commands of type 1*/
#define NUMICOM2 4 /*number of I commands of type 2*/
#define NUMREGSR1 3 /*number of registers for R1 type command*/
#define NUMREGSR2 2 /*number of registers for R2 type command*/
#define NUMREGSI1 2 /*number of registers for I1 type command*/
#define NUMREGSI2 2 /*number of registers for I2 type command*/
#define REGISTER 1 /*register number*/
#define NEXTINDEX 1 /*constant for next index*/


/*The checkCommandR1 function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "add","sub", "and", "or", "nor" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandR1(char *ptrCode,int lineNumber,char *filename);


/*The checkCommandR2 function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "move", "mvhi","mvlo" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandR2(char *ptrCode,int lineNumber,char *filename);


/*The checkCommandI1 function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode for ("addi", "subi", "andi", "ori","nori","lb", "sb", "lw", "sw", "lh","sh" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandI1(char *ptrCode,int lineNumber,char *filename);


/*The checkCommandI2 function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "bne", "beq", "blt", "bgt" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandI2(char *ptrCode,int lineNumber,char *filename);


/*The checkCommandjump function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "jmp" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandjump(char *ptrCode,int lineNumber,char *filename);


/*The checkCommandlaOrcall function receives ptrCode of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char storing the checked code. The function analyzes the characters stored in ptrCode (for "la", "call" commands), and prints out an error message if detected, with the line number and filename from the function arguments in the error message. The function returns the checked code if no errors were detected, and returns NULL if an error was detected.*/
char* checkCommandlaOrcall(char *ptrCode,int lineNumber,char *filename);


