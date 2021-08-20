/*The following file is mainly used to analyze the input file line by line and create a linked list of nodes that store the characters of each line of the input file for further analysis.*/

#include "assembler.h" /*include assembler.h header file*/
#include "handleInput.h" /*include handleInput.h header file*/

/*The handleFileContents function receives fd of type pointer to FILE and filename of type pointer to char. The function allocates memory for the first node of a linked list to store characters from the input file, with a line of input for each node. The function passes this first node to the storeLines function where the linked list is created and if the file is not empty then the manageContents function is called. The handleFileContents is void and does not return anything.*/
void handleFileContents(FILE *fd, char *filename){
	NODE_T *ptrNode; /*initialize pointer to NODE_T, variable ptr_node*/
	NODE_T *tmpPtr; /*initialize pointer to tmpPtr of type NODE_T*/
	int emptyFileFlag; /*flag for alerting if file is empty*/
	
	/*allocating memory for first node of linked list to store characters from each line in file*/
	tmpPtr = (NODE_T*)calloc(SINGLENODE, sizeof(NODE_T));
	if(!tmpPtr)
	{
		printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
		exit(0);
	}
	ptrNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/

	emptyFileFlag = storeLines(ptrNode, fd); /*calls storeLines function with ptrNode and fd file pointer, to create linked list of nodes with characters from each line from the input file*/

	if(emptyFileFlag == FLAGOFF){
		manageContents(ptrNode,filename); /*if the file is not an empty file, call manageContents function with the linked list of each line from the file and the filename to analyze the input file line by line*/
	}

	freeNodes(ptrNode); /*frees the linked list of nodes storing characters from each line of the input file*/
}


/*The storeLines function receives ptrNode of type pointer to NODE_T, and fd of type pointer to ptrNode. The first node was allocated in handleFileContents and passed to the function and characters from fd will be stored in ptrNode. Each time a new line is detected, a new node is created and the following characters are stored in the new node. This creates a linked list of characters from each line of the input file. The function returns an int that is used as a flag to alert if the file is empty or not.*/
int storeLines(NODE_T *ptrNode, FILE *fd){
	int charFromFile,lineCounter,index,emptyFileFlag;
	NODE_T *head; /*initialize pointer to head of type NODE_T*/
	NODE_T *current; /*initialize pointer to current node of type NODE_T*/

	NODE_T *tmpPtr; /*initialize pointer to tmpPtr of type NODE_T*/
	lineCounter = FIRSTLINE; /*initialize line counter*/
	index = STARTINDEX; /*initialize index*/

	head = ptrNode; /*sets head equal to ptrNode to point to the first element of the list*/
	head->next = NULL; /*sets pointer to next element to NULL because there are no other elements at this point*/

	emptyFileFlag = FLAGON; /*set emptyFileFlag to on while no characters have been received yet*/

	while(1){
		charFromFile = fgetc(fd); /*store character from file in charFromFile variable*/

		/*check if EOF reached*/
		if(charFromFile != EOF){

			if(!(charFromFile == '\n' || charFromFile == '\t' || charFromFile == ' ')){
				emptyFileFlag = FLAGOFF; /*if character that is not white space detected, turn off emptyFileFlag*/
			}

			ptrNode->inputChar[index] = charFromFile; /*store current character in array of current node of linked list*/
			index++;  /*increment index*/

			if((charFromFile == '\n') || (index == (MAXLINELEN+EXTRACHARPASTLIMIT))){
				if(index == (MAXLINELEN+EXTRACHARPASTLIMIT)){
					/*checks if line exceeds character limit and iterates until the end of the current line*/
					while(charFromFile != '\n' && charFromFile != EOF){
						charFromFile = fgetc(fd);
					}
				}
				ptrNode->lineNumber = lineCounter; /*assigns lineCounter to the lineNumber field of the current node*/
				lineCounter++; /*increments line counter*/
				current = ptrNode; /*sets the "current" node equal to ptrNode, pointing to the element whose buffer was just filled*/
				tmpPtr = (NODE_T*)calloc(SINGLENODE, sizeof(NODE_T)); /*uses temporary pointer in case of error to dynamically allocate memory for an additional element to add to the linked list*/
				if(!tmpPtr)
				{
					printf("\nError! memory not allocated."); /*Prints error message if no more memory could be allocated*/
					exit(0);
				}
				ptrNode = tmpPtr; /*return the temporary pointer to the original pointer variable pointing to the new element after memory successfully allocated*/
				current->next = ptrNode; /*sets pointer to the next element of the "current" node equal to ptrNode, whose buffer will be filled next*/
				index = STARTINDEX;
			}
		}
		else{
			break; /*break if EOF reached*/
		}

	}
	ptrNode = head; /*set ptrNode equal to head to point to the first element of the list*/
	return emptyFileFlag; /*return flag alerting if file was empty or not*/
}


void freeNodes(NODE_T *ptrNode){
    NODE_T *temp;/*A temp node which will be deleted from the linked list*/
    NODE_T *current;/*A temp node which will be deleted from the linked list*/

    current = ptrNode; /*sets current node to point to head*/

    /*While the linked list is not null-continue to delete nodes from the linked-list*/
    while(1){
        temp=current; /*sets temp node to current node*/

        current=current->next; /*set current to point to next node*/
	
        free(temp); /*free temp node*/
	if (current==NULL){
            break; /*break if current is NULL symbolizing the end of the linked list*/
	}
    }
}


