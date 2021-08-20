#include <stdio.h> /*standard input output header file*/
#include <stdlib.h> /*standard library header file*/
#include <string.h> /*string header file*/

#define MAXLINELEN 80 /*sets the buffer size to 80*/
#define ERROR 1
#define NOERROR 0
#define MAXLABELLEN 32 /*maximum length of label of 31 + 1 for \0*/
#define MAXDATACODELEN 8 /* maximum length of data or code command (.extern is 7 chars + 1 for \0)*/
#define FLAGOFF 0
#define FLAGON 1
#define NUMDIRECTIVES 4
#define NUMCOMMANDS 27
#define NOQUOTES 0
#define OPENQUOTES 1
#define CLOSEDQUOTES 2
#define LASTLINE -1
#define SINGLENODE 1

#define MAXFILENAMELEN 100
#define FILEEXTENSIONPOS1 3 /*position from end of file extension "." of ".as"*/
#define FILEEXTENSIONPOS2 2 /*position from end of file extension "a" of ".as"*/
#define FILEEXTENSIONPOS3 1 /*position from end of file extension "s" of ".as"*/
#define ARGCOUNTERSTART 1 /*first index of argument vector with file inputs*/

#define STARTINDEX 0

#define FIRSTLINE 1
#define EXTRACHARPASTLIMIT 1

#define NUMRCOM1 5
#define NUMRCOM2 3
#define NUMICOM1 11
#define NUMICOM2 4
#define NUMREGSR1 3
#define NUMREGSR2 2
#define NUMREGSI1 2
#define NUMREGSI2 2
#define REGISTER 1
#define NEXTINDEX 1

#define PREVINDEX 1

#define DCINIT 0
#define ICINIT 100
#define ICINCREMENT 4
#define ATTRIBUTE1 0
#define ATTRIBUTE2 1
#define ERRORFLAG1 1
#define ERRORFLAG2 2
#define ERRORFLAG3 3
#define ERRORFLAG4 4
#define ERRORFLAG5 5
#define EXTERNVALUE 0

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


typedef struct memoryImage{
    char symbol[SYMBOL_NUM_OF_CHARS];/*The symbol of a node*/
    char op[NUM_OF_BITS_OP];/*An operation (expressed by binary bits)*/
    DATA *p;/*A pointer to data*/
    int localDc;/*Data counter*/
    int dc;
    int ic;
    int missLabelFlag;
    int errorFlag;
    struct memoryImage *next;
} MEMIM;

typedef struct fieldBuffer{
    char *field;
    struct fieldBuffer *next;
}FIELD_BUFFER_T;

struct node {
	int lineNumber;
	char inputChar[MAXLINELEN+1];
	struct node* next; /*pointer to the next node (for linked list)*/
};
typedef struct node NODE_T; /*node struct defined as type NODE_T*/

#define NUMATTRIBUTES 2

struct tableNode{
	char *symbol;
	int value;
	int attribute[NUMATTRIBUTES];
	struct tableNode* next;
};
typedef struct tableNode TABLE_NODE_T;

struct lineFields{
	char label[MAXLABELLEN];
	char comOrDir[MAXDATACODELEN];
	char values[MAXLINELEN];
	int labelFlag;
	int lineNumber;
	struct lineFields* next;
};
typedef struct lineFields LINE_FIELDS_T;

struct symbolAddStruct{
	char label[MAXLABELLEN];
	int address;
	int externalFlag;
	int errorFlag;
	struct symbolAddStruct* next;
};
typedef struct symbolAddStruct SYMBOL_ADD_STRUCT_T;

/*The readFile function receives argc of type int, and argv of type pointer to pointer to char. These inputs are the argument counter and argument vector, and allow for the function to iterate over all arguments, open each file in the arguments, and pass the files for further analysis to the handleFileContents functions. The readFile function is void and doesn't return anything but prints errors if file listed in arguments cannot be opened, etc.*/
void readFile(int argc, char** argv);

/*The handleFileContents function receives fd of type pointer to FILE and filename of type pointer to char. The function uses the file pointer fd to call the storeLines function along with a pointer to node that is allocated in the function to store the characters from each line node by node. The head of these nodes is passed to the manageContents function for further analysis unless the input of the file is detected to be empty. The function is void and does not return anything.*/
void handleFileContents(FILE *fd, char *filename);

/*The storeLines function receives ptrNode of type pointer to NODE_T and fd of type pointer to fd. The function iterates over the characters from the input fd, and stores character from each line in a node of ptrNode, and creates a new node for each line. The function returns an int that is used as a flag to symbol if the file is empty or not.*/
int storeLines(NODE_T *ptrNode, FILE *fd);


void manageContents(NODE_T *ptrNode, char *filename);
void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int errorDetected, char *filename,int lineNumber);

/*The symbolTable function receives a symbol of type pointer to char, value of type int, attribute1 of type int, attribute 2 of type int, and firstSymbolFlag of type int, and returns the head of the symbol table of type pointer to TABLE_NODE_T. The symbol (label) should be inputted in the first argument, following by its corresponding value (address), attribute1, attribute2, and the final argument, firstSymbolFlag, should be on if it is the first call to the function and the symbol table needs to be initialized, and should be off it is not the first call to the function and an additional node will be added to the existing symbol table. The function returns the head of the symbol table of type pointer to TABLE_NODE_T which contains the fields, symbol of type pointer to char, value of type int, and attributes which is an array of ints, and a pointer to the next node of the symbol table.*/
TABLE_NODE_T* symbolTable(char *symbol,int value,int attribute1,int attribute2,int firstSymbolFlag);

/*The storeLineFields function receives ptrField1 of type pointer to char, ptrField2 of type pointer to char, ptrField3 of type pointer to char, labelFlag of type int, lineNumber of type int, and firstLineFlag of type int. The function returns the head of the line fields of type LINE_FIELDS_T. For each line in the input file, the line can be separated into three fields (optional label, command or directive, corresponding values). This function stores each field for each line from the input file, along with a labelFlag alerting if label used in line, corresponding lineNumber, and firstLineFlag to notify if it is the first line of the file. This function is used in the first pass of the file so all the fields and corresponding information can be used in the second pass.*/
LINE_FIELDS_T* storeLineFields(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int lineNumber,int firstLineFlag);

void secondPass(LINE_FIELDS_T* linesHead, TABLE_NODE_T* tableHead, int ICF, int DCF, char *filename, MEMIM* memImHead,int symbolTableInitFlag);

/*The errorMsg function receives error of type int, lineNumber of type int, fieldName of type pointer to char, and fileName of type pointer to char. It is a void function so it does not return anything but it is used to print out error messages according to the input. The error argument is used to specify the type of error, the lineNumber allows to print out in what line the error was detected, the fieldName allows to print the field of interest, and the fileName allows the print in what file the error was detected.*/
void errorMsg(int error,int lineNumber,char *fieldName,char *fileName);

/*The newLine function receives errorDetected of type pointer to int, current of type pointer to pointer to NODE_T, labelFlag ot type pointer to int, and index of type pointer to int, and returns an int which resets the state to PREFIRSTWORD. The function is used when analyzing each line of the input file, and allows for the analysis of the next line. It does so by setting the errorDetected flag to off, sets the current node to the next node, sets the labelFlag to off, and resets the index back to zero. Finally the function returns the state PREFIRSTWORD of type int (defined in enum) which is used to reset the state machine switch case back to its initial state*/
int newLine(int *errorDetected,NODE_T **current,int *labelFlag,int *index);

/*The checkExtraneousChars function receives current of type pointer to pointer to NODE_T, and index of type pointer to int, and returns an error flag of type int. The function receives the current node with is the current line analyzed from the input file, and the current index to access the current character in the line. The function iterates over the remaining characters in the line and ensures there are not any additional unexpected characters that are not white space characters. If no extraneous characters were detected the function returns an error flag of type int as off, otherwise the flag will be returned as on.*/
int checkExtraneousChars(NODE_T **current,int *index);

/*The checkCommand function receives ptrCode of type pointer to char, ptrCommand of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char which holds the checked code. The function analyzes the characters stored in ptrCode according to the type of command stored in ptrCommand, and also takes the line number and file name as inputs in order to print out any error messages with the corresponding file name and line number. The function returns the checked code if no errors were detected and returns NULL if an error was detected (and an error message will be printed)*/
char* checkCommand(char *ptrCode,char *ptrCommand,int lineNumber,char *filename);

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


/*The checkData function receives ptrData of type pointer to char, ptrDirective of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char which holds the checked data. The function analyzes the characters stored in ptrCode according to the type of directive stored in ptrData, and also takes the line number and file name as inputs in order to print out any error messages with the corresponding file name and line number. The function returns the checked data if no errors were detected and returns NULL if an error was detected (and an error message will be printed)*/
char* checkData(char *ptrData,char *ptrDirective,int lineNumber,char *filename);


void freeNodes(NODE_T *ptrNode);
void freeLines(LINE_FIELDS_T* linesPtr);
void freeTable(TABLE_NODE_T* tablePtr);

/*The createOutputFiles function receives memImHead of type pointer to MEMIM, tableHead of type pointer to TABLE_NODE_T, filename of type pointer to char, external head of type pointer to SYMBOL_ADD_STRUCT_T, ICF of type int, DCF of type int, errorFlag of type int, symbolTableInitFlag of type int and externalFlag of type int, and does not return anything, but creates and writes to files if necessary. The function creates output files of type .ob,.ext, and .ent. To do this, the file receives as inputs the head of the memory image, memImHead, which is used to print to the object file, the head of the symbol table, tableHead, which is used to create the extern file, the input file name which is used to name the output files, externalHead which is used to create the extern files, ICF and DCF which are used to label the object file, errorFlag which alerts if files should be printed or not, symbolTableInitFlag which alerts if symbol table was initialized, and externalFlag, which alerts if externals were used and if an ext file should be created.*/
void createOutputFiles(MEMIM* memImHead, TABLE_NODE_T* tableHead, char *filename, SYMBOL_ADD_STRUCT_T *externalHead, int ICF, int DCF,int errorFlag,int symbolTableInitFlag,int externalFlag);

MEMIM *memAdd(char*,char*,char*);
char *Registers(char*);
char *decToBin(int);
char *decToBinJ(int);
char *ascizToBin(int);
char *decToBinDirW(char*);
char *decToBinDirH(char*);
void deleteNode(MEMIM*);
void addNode(MEMIM *headCom,MEMIM *headData, MEMIM *node,int firstDataNodeAddflag,int firstComNodeAddflag);
char binToHex(char *bin);
void concatNodes(MEMIM *headCom,MEMIM *headData);
void printSymbolTable(TABLE_NODE_T *symbolTable);
SYMBOL_ADD_STRUCT_T* symbolAddNew(MEMIM *head,TABLE_NODE_T* table,int lineNumber,int firstEntry,char *filename);
void printListToFile(MEMIM *head,FILE *fptrObject);

void freeMemIm(MEMIM* memImHead);

enum errorTypes {ERRORTYPE0, ERRORTYPE1, ERRORTYPE2, ERRORTYPE3,  ERRORTYPE4, ERRORTYPE5, ERRORTYPE6, ERRORTYPE7, ERRORTYPE8, ERRORTYPE9, ERRORTYPE10, ERRORTYPE11, ERRORTYPE12, ERRORTYPE13, ERRORTYPE14, ERRORTYPE15, ERRORTYPE16, ERRORTYPE17, ERRORTYPE18, ERRORTYPE19, ERRORTYPE20, ERRORTYPE21, ERRORTYPE22, ERRORTYPE23, ERRORTYPE24, ERRORTYPE25, ERRORTYPE26, ERRORTYPE27, ERRORTYPE28, ERRORTYPE29, ERRORTYPE30, ERRORTYPE31, ERRORTYPE32, ERRORTYPE33, ERRORTYPE34};




