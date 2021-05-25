#include "main.h"

void handleFileContents(FILE *fd){

	int charFromFile; /*declare variable for receiving characters from file*/
	int lineCounter;
	int newLineFlag = 1;
	NODE_T *ptrNode; /*initialize pointer to NODE_T, variable ptr_node*/
	NODE_T *head; /*initialize pointer to head of type NODE_T*/
	NODE_T *current; /*initialize pointer to current node of type NODE_T*/
	NODE_T *tmpPtr; /*initialize pointer to tmpPtr of type NODE_T*/
	lineCounter = 1;

	tmpPtr = (NODE_T*)calloc(1, sizeof(NODE_T));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/
	head = ptrNode; /*sets head equal to ptrNode to point to the first element of the list*/
	head->next = NULL; /*sets pointer to next element to NULL because there are no other elements at this point*/
	/*head->lineNumber = lineCounter;*/

	while(1){
		charFromFile = fgetc(fd); /*store character from file in charFromFile variable*/
		if(!feof(fd)){
			/*printf("%c", charFromFile);*/ /*if the current character from the file is not EOF, then print the character*/

			if(charFromFile != '\n'){
				ptrNode->inputChar[index] = 
			}
			
			if(newLineFlag == 1){
				ptrNode->lineNumber = lineCounter;
				current = ptrNode; /*sets the "current" node equal to ptrNode, pointing to the element whose buffer was just filled*/
				tmpPtr = (NODE_T*)calloc(1, sizeof(NODE_T)); /*uses temporary pointer in case of error to dynamically allocate memory for an additional element to add to the linked list*/
				if(!tmpPtr)
				{
					/*if no more memory can be allocated, set the error flag to ERROR and break out of the while loop*/
					errorFlag = ERROR;
					break;
				}
				ptrNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/
				current->next = ptrNode; /*sets pointer to the next element of the "current" node equal to ptrNode, whose buffer will be filled next*/
			}

		

		}
		else{
			break; /*if the current character from the file is EOF, break from the while loop*/
		}
		ptrNode = head; /*set ptrNode equal to head to point to the first element of the list*/
	}
	
}
