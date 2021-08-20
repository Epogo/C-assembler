#include <stdio.h> /*standard input output header file*/
#include <stdlib.h> /*standard library header file*/
#include <string.h> /*string header file*/

#define SINGLEARG 1 /*constant for single argument*/
#define MAXLINELEN 80 /*sets the buffer size to 80*/
#define ERROR 1 /*constant symboling error*/
#define NOERROR 0 /*constant symboling no error*/
#define MAXLABELLEN 32 /*maximum length of label of 31 + 1 for \0*/
#define MAXDATACODELEN 8 /* maximum length of data or code command (.extern is 7 chars + 1 for \0)*/
#define FLAGOFF 0 /*constant for flag off*/
#define FLAGON 1 /*constant for flag on*/
#define NUMDIRECTIVES 4 /*number of directive types considering only ".dh",".dw",".db",".asciz"*/
#define NUMCOMMANDS 27 /*number of command types*/
#define LASTLINE -1 /*last line flag*/
#define SINGLENODE 1 /*constant symboling single node*/
#define MAXFILENAMELEN 100 /*maximum file name length assumed to be 100 characters*/
#define STARTINDEX 0 /*zero index*/
#define FIRSTLINE 1 /*first line constant*/
#define EXTRACHARPASTLIMIT 1 /*constant for extraneous char*/
#define PREVINDEX 1 /*constant for previous index*/
#define DCINIT 0 /*initial data counter*/
#define ICINIT 100 /*initial instruction counter*/
#define ICINCREMENT 4 /*instruction counter increment*/
#define ATTRIBUTE1 0 /*index of attribute 1*/
#define ATTRIBUTE2 1 /*index of attribute 2*/
#define ERRORFLAG1 1 /*constant for ERRORFLAG1*/
#define ERRORFLAG2 2 /*constant for ERRORFLAG2*/
#define ERRORFLAG3 3 /*constant for ERRORFLAG3*/
#define ERRORFLAG4 4 /*constant for ERRORFLAG4*/
#define ERRORFLAG5 5 /*constant for ERRORFLAG5*/
#define EXTERNVALUE 0 /*value for extern in symbol table*/
#define RCOMLEN 8 /*length of RCOM*/
#define ICOMLEN 15 /*length of ICOM*/
#define JCOMLEN 4 /*length of JCOM*/
#define BITSINBYTE 9 /*bits in byte plus one*/
#define SYMBOL_NUM_OF_CHARS 32 /*maximum number of chars in symbol*/
#define NUM_OF_BITS_OP 33 /*number of bits in operation*/
#define NUM_OF_CHARS_IN_LINE 80 /*maximum number of chars in line*/
#define NUMATTRIBUTES 2 /*number of attributes for symbol table*/

/*struct to store data*/
typedef struct data{
    char byte[BITSINBYTE];/*Chars array with length of 8 bits (for a byte) and a NULL terminator*/
    struct data *next; /*next node in linked list*/
}DATA;

/*struct to store memory image*/
typedef struct memoryImage{
    char symbol[SYMBOL_NUM_OF_CHARS];/*The symbol of a node*/
    char op[NUM_OF_BITS_OP];/*An operation (expressed by binary bits)*/
    DATA *p;/*A pointer to data linked list*/
    int localDc;/*Data counter*/
    int dc; /*data counter*/
    int ic; /*instruction counter*/
    int missLabelFlag; /*indicates missing label*/
    int errorFlag; /*indicates error*/
    struct memoryImage *next; /*next node in linked list*/
} MEMIM;

/*struct to store linked list of all characters in each line of input file*/
struct node {
	int lineNumber; /*stores current line number*/
	char inputChar[MAXLINELEN+EXTRACHARPASTLIMIT]; /*stores all characters in single line from input file*/
	struct node* next; /*pointer to the next node (for linked list)*/
};
typedef struct node NODE_T; /*node struct defined as type NODE_T*/

/*struct to store linked list of symbol table*/
struct tableNode{
	char *symbol; /*stores label*/
	int value; /*stores value (address)*/
	int attribute[NUMATTRIBUTES]; /*stores attributes of label*/
	struct tableNode* next; /*pointer to the next node of linked list*/
};
typedef struct tableNode TABLE_NODE_T;

/*struct to store linked list of all fields in each line*/
struct lineFields{
	char label[MAXLABELLEN]; /*stores label*/
	char comOrDir[MAXDATACODELEN]; /*stores command or directive*/
	char values[MAXLINELEN]; /*stores corresponding values*/
	int labelFlag; /*indicates label detected*/
	int lineNumber; /*indicates line number*/
	struct lineFields* next; /*pointer to next node in linked list*/
};
typedef struct lineFields LINE_FIELDS_T;

/*struct to store linked list of returns from symbolAddNew*/
struct symbolAddStruct{
	char label[MAXLABELLEN]; /*stores label*/
	int address; /*stores address*/
	int externalFlag; /*indicates if external detected*/
	int errorFlag; /*indicates if label detected*/
	struct symbolAddStruct* next; /*pointer to next node in linked list*/
};
typedef struct symbolAddStruct SYMBOL_ADD_STRUCT_T;

/*The readFile function receives argc of type int, and argv of type pointer to pointer to char. These inputs are the argument counter and argument vector, and allow for the function to iterate over all arguments, open each file in the arguments, and pass the files for further analysis to the handleFileContents functions. The readFile function is void and doesn't return anything but prints errors if file listed in arguments cannot be opened, etc.*/
void readFile(int argc, char** argv);

/*The errorMsg function receives error of type int, lineNumber of type int, fieldName of type pointer to char, and fileName of type pointer to char. It is a void function so it does not return anything but it is used to print out error messages according to the input. The error argument is used to specify the type of error, the lineNumber allows to print out in what line the error was detected, the fieldName allows to print the field of interest, and the fileName allows the print in what file the error was detected.*/
void errorMsg(int error,int lineNumber,char *fieldName,char *fileName);

/*enum of all error types*/
enum errorTypes {ERRORTYPE0, ERRORTYPE1, ERRORTYPE2, ERRORTYPE3,  ERRORTYPE4, ERRORTYPE5, ERRORTYPE6, ERRORTYPE7, ERRORTYPE8, ERRORTYPE9, ERRORTYPE10, ERRORTYPE11, ERRORTYPE12, ERRORTYPE13, ERRORTYPE14, ERRORTYPE15, ERRORTYPE16, ERRORTYPE17, ERRORTYPE18, ERRORTYPE19, ERRORTYPE20, ERRORTYPE21, ERRORTYPE22, ERRORTYPE23, ERRORTYPE24, ERRORTYPE25, ERRORTYPE26, ERRORTYPE27, ERRORTYPE28, ERRORTYPE29, ERRORTYPE30, ERRORTYPE31, ERRORTYPE32, ERRORTYPE33, ERRORTYPE34};




