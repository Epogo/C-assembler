
/*The storeLines function receives ptrNode of type pointer to NODE_T and fd of type pointer to fd. The function iterates over the characters from the input fd, and stores character from each line in a node of ptrNode, and creates a new node for each line. The function returns an int that is used as a flag to symbol if the file is empty or not.*/
int storeLines(NODE_T *ptrNode, FILE *fd);


/*The freeMemIm function receives node of type MEMIM* and returns nothing as it is a void function. The function iterates through the memory image and frees every node from the memory image until the end of the memory image is reached. For each node the function also frees the linked list within the p field of the struct.*/
void freeNodes(NODE_T *ptrNode);

/*The manageContents function receives ptrNode of type pointer to NODE_T, and filename of type pointer to char. The function iterates over the lines from the input file using ptrNode, which points to the head of the lines, and analyzes each line for errors and divides the lines into three fields, which include an optional label, command or directive, and corresponding values. These fields are passed into the firstPass function for further analysis. The manageContents function does not return anything as it is a void function but prints out error messages accordingly with the filename from the input and the line number from the ptrNode struct.*/
void manageContents(NODE_T *ptrNode, char *filename);
