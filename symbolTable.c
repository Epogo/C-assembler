/*The following file contains the symbolTable function. It creates the symbol table by calling the function from the first pass algorithm with the parameters of symbol, value, attribute1, attribute2, and firstSymbolFlag and returns head of the symbol table. The symbol table is developed by creating a linked list that keeps track of each node of the symbol table with the inputted symbol and its corresponding value and attributes.*/

#include "assembler.h" /*include assembler.h header file*/

/*The following function, symbolTable, is called from the firstPass function, and receives a symbol of type pointer to char, value of type int, attribute1 of type int, attribute 2 of type int, and firstSymbolFlag of type int, and returns the head of the symbol table of type pointer to TABLE_NODE_T. The function creates the symbol table by creating a linked list of each node of the symbol table. It does this by using the firstSymbol flag to know when to create the head of the table and then each time the function is called it adds another node to the the symbol table.*/
TABLE_NODE_T* symbolTable(char *symbol,int value,int attribute1,int attribute2,int firstSymbolFlag){

	TABLE_NODE_T *ptrTableNode; /*initialize pointer to TABLE_NODE_T, variable ptrTableNode*/
	TABLE_NODE_T *tmpPtr; /*initialize pointer to tmpPtr of type TABLE_NODE_T*/
	static TABLE_NODE_T *current; /*initialize pointer to current node of symbol table*/
	static TABLE_NODE_T *head; /*initialize pointer to head of symbol table*/

	tmpPtr = (TABLE_NODE_T*)calloc(SINGLENODE, sizeof(TABLE_NODE_T)); /*allocate memory to store node of symbol table*/
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrTableNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	if(firstSymbolFlag == FLAGOFF){
		current->next = ptrTableNode; /*if firstSymbolFlag is off set next node to node just allocated*/
	}

	if(firstSymbolFlag == FLAGON){
		head = ptrTableNode; /*if firstSymbolFlag is on set head of symbol table to node just allocated*/
	}

	ptrTableNode->symbol = symbol; /*set symbol field of struct to symbol from function argument*/
	ptrTableNode->value = value; /*set value field of struct to value from function argument*/
	ptrTableNode->attribute[ATTRIBUTE1] = attribute1; /*set first attribute field of struct to attribute1 from function argument*/
	ptrTableNode->attribute[ATTRIBUTE2] = attribute2;/*set second attribute field of struct to attribute2 from function argument*/
	ptrTableNode->next = NULL; /*set next node to NULL*/

	current = ptrTableNode; /*set current node to node just allocated*/
	
	firstSymbolFlag = FLAGOFF; /*set firstSymbolFlag to off*/

	return head; /*return head of symbol table*/

}

