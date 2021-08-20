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

void readFile(int argc, char** argv);
void handleFileContents(FILE *fd, char *filename);
int storeLines(NODE_T *ptrNode, FILE *fd);
void manageContents(NODE_T *ptrNode, char *filename);
void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int errorDetected, char *filename,int lineNumber);
TABLE_NODE_T* symbolTable(char *symbol,int value,int attribute1,int attribute2,int firstSymbolFlag);
LINE_FIELDS_T* storeLineFields(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int lineNumber,int firstLineFlag);
void secondPass(LINE_FIELDS_T* linesHead, TABLE_NODE_T* tableHead, int ICF, int DCF, char *filename, MEMIM* memImHead,int symbolTableInitFlag);
void errorMsg(int error,int lineNumber,char *fieldName,char *fileName);
int newLine(int *errorDetected,NODE_T **current,int *labelFlag,int *index);
int checkExtraneousChars(NODE_T **current,int *index);
char* checkCommand(char *ptrCode,char *ptrCommand,int lineNumber,char *filename);
char* checkCommandR1(char *ptrCode,int lineNumber,char *filename);
char* checkCommandR2(char *ptrCode,int lineNumber,char *filename);
char* checkCommandI1(char *ptrCode,int lineNumber,char *filename);
char* checkCommandI2(char *ptrCode,int lineNumber,char *filename);
char* checkCommandjump(char *ptrCode,int lineNumber,char *filename);
char* checkCommandlaOrcall(char *ptrCode,int lineNumber,char *filename);
char* checkData(char *ptrData,char *ptrDirective,int lineNumber,char *filename);
void freeNodes(NODE_T *ptrNode);
void freeLines(LINE_FIELDS_T* linesPtr);
void freeTable(TABLE_NODE_T* tablePtr);
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




