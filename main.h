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

void readFile(int argc, char** argv);
void handleFileContents(FILE *fd);
void storeLines(NODE_T *ptrNode, FILE *fd);
void manageContents(NODE_T *ptrNode);
void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag);
TABLE_NODE_T* symbolTable(char *symbol,int value,int attribute1,int attribute2);



