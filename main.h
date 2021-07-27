#include <stdio.h> /*standard input output header file*/
#include <stdlib.h> /*standard library header file*/
#include <string.h> /*string header file*/

#define MAXLINELEN 80 /*sets the buffer size to 80*/
#define ERROR 1
#define NOERROR 0
#define MAXLABELLEN 32 /*maximum length of label of 31 + 1 for \0*/
#define MAXDATACODELEN 7 /* maximum length of data or code command (.asciz is 6 chars + 1 for \0)*/
#define FLAGOFF 0
#define FLAGON 1
#define NUMDIRECTIVES 4
#define NUMCOMMANDS 27
#define NOQUOTES 0
#define OPENQUOTES 1
#define CLOSEDQUOTES 2
#define LASTLINE -1

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

/*Maybe a union should be added*/
typedef struct memoryImage{
    char symbol[SYMBOL_NUM_OF_CHARS];/*The symbol of a node*/
    char op[NUM_OF_BITS_OP];/*An operation (expressed by binary bits)*/
    DATA *p;/*A pointer to data*/
    int ic;/*Instruction counter.*/
    int dc;/*Data counter*/
    struct memoryImage *next;
} MEMIM;



struct node {
	int lineNumber;
	char inputChar[MAXLINELEN];
	struct node* next; /*pointer to the next node (for linked list)*/
};
typedef struct node NODE_T; /*node struct defined as type NODE_T*/

struct tableNode{
	char *symbol;
	int value;
	int attribute[2];
	struct tableNode* next;
};
typedef struct tableNode TABLE_NODE_T;

struct lineFields{
	char label[MAXLABELLEN];
	char comOrDir[MAXDATACODELEN];
	char values[MAXLINELEN];
	int labelFlag;
	struct lineFields* next;
};
typedef struct lineFields LINE_FIELDS_T;

void readFile(int argc, char** argv);
void handleFileContents(FILE *fd);
void storeLines(NODE_T *ptrNode, FILE *fd);
void manageContents(NODE_T *ptrNode);
void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int errorDetected);
TABLE_NODE_T* symbolTable(char *symbol,int value,int attribute1,int attribute2);
LINE_FIELDS_T* storeLineFields(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag);
void secondPass(LINE_FIELDS_T* linesHead, TABLE_NODE_T* tableHead, int ICF, int DCF);
void errorMsg(int error,int lineNumber,char *fieldName);
int newLine(int *errorDetected,NODE_T **current,int *labelFlag,int *index);
int checkExtraneousChars(NODE_T **current,int *index);
char* checkCommand(char *ptrCode,char *ptrCommand,int lineNumber);
char* checkCommandR1(char *ptrCode,int lineNumber);
char* checkCommandR2(char *ptrCode,int lineNumber);
char* checkCommandI1(char *ptrCode,int lineNumber);
char* checkCommandI2(char *ptrCode,int lineNumber);
char* checkCommandjump(char *ptrCode,int lineNumber);
char* checkCommandlaOrcall(char *ptrCode,int lineNumber);
char* checkData(char *ptrData,char *ptrDirective,int lineNumber);
MEMIM *memAdd(char*,char*,char*,TABLE_NODE_T*);
char *Registers(char*);
char *decToBin(int);
char *decToBinJ(int);
char *ascizToBin(int);
char *decToBinDirW(char*);
char *decToBinDirH(char*);
void deleteNode(MEMIM*);
void addNode(MEMIM *headCom,MEMIM *headData, MEMIM *node);
void printList(MEMIM *head);
char binToHex(char *bin);
void concatNodes(MEMIM *headCom,MEMIM *headData);
void printSymbolTable(TABLE_NODE_T *symbolTable);








