#include <stdio.h> /*standard input output header file*/
#include <stdlib.h> /*standard library header file*/
#include <string.h> /*string header file*/

#define MAXLINELEN 80 /*sets the buffer size to 80*/
#define ERROR 1
#define NOERROR 0
#define MAXLABELLEN 31 /*maximum length of label*/
#define FLAGOFF 0
#define FLAGON 1
#define NUMDIRECTIVES 4
#define NUMCOMMANDS 27

struct node {
	int lineNumber;
	char inputChar[MAXLINELEN];
	struct node* next; /*pointer to the next node (for linked list)*/
};
typedef struct node NODE_T; /*node struct defined as type NODE_T*/

void readFile(int argc, char** argv);
void handleFileContents(FILE *fd);
void storeLines(NODE_T *ptrNode, FILE *fd);
void firstPass(NODE_T *ptrNode);
