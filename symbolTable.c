#include "main.h"


void symbolTable(char *symbol,int value,int attribute){
	/*printf("\n%s\n",symbol);*/

	TABLE_NODE_T *ptrTableNode; /*initialize pointer to TABLE_NODE_T, variable ptrTableNode*/
	TABLE_NODE_T *tmpPtr; /*initialize pointer to tmpPtr of type TABLE_NODE_T*/

	tmpPtr = (TABLE_NODE_T*)calloc(1, sizeof(TABLE_NODE_T));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrTableNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	ptrTableNode->symbol = symbol;
	ptrTableNode->value = value;
	ptrTableNode->attribute[0] = attribute;
	ptrTableNode->next = NULL;

}

