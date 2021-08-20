

/*The storeLineFields function receives ptrField1 of type pointer to char, ptrField2 of type pointer to char, ptrField3 of type pointer to char, labelFlag of type int, lineNumber of type int, and firstLineFlag of type int. The function returns the head of the line fields of type LINE_FIELDS_T. For each line in the input file, the line can be separated into three fields (optional label, command or directive, corresponding values). This function stores each field for each line from the input file, along with a labelFlag alerting if label used in line, corresponding lineNumber, and firstLineFlag to notify if it is the first line of the file. This function is used in the first pass of the file so all the fields and corresponding information can be used in the second pass.*/
LINE_FIELDS_T* storeLineFields(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int lineNumber,int firstLineFlag);


/*The freeLines function receives linesPtr of type LINE_FIELDS_T* and returns nothing as it is a void function. The function iterates through the line fields and frees every node from the line fields until the end of the line fields is reached.*/
void freeLines(LINE_FIELDS_T* linesPtr);

/*The freeTable function receives tablePtr of type TABLE_NODE_T* and returns nothing as it is a void function. The function iterates through the symbol table and frees every node from the table until the end of the table is reached*/
void freeTable(TABLE_NODE_T* tablePtr);


/*The freeMemIm function receives node of type MEMIM* and returns nothing as it is a void function. The function iterates through the memory image and frees every node from the memory image until the end of the memory image is reached. For each node the function also frees the linked list within the p field of the struct.*/
void freeMemIm(MEMIM* memImHead);

/*This function is responsible for adding nodes the memory image (first pass only).
@param ptrField1-This parameter is used for the label field.
@param ptrField2-This parameter is used for the command/directive name.
@param ptrField3-This parameter is used for the registers/Data.
@return node-a memory image struct is returned. 
This function is responsible for converting assembly lines to binary digits lines (binary memory image).
The function is building the memory image after the first pass, it detects the type of the command or directive
and converts the content of the assembly line to a bits array (by using "a table" which stores opcodes/functs of each command).
For commands from J type (excluding "stop" command) and branching commands-the missing symbol value will be completed by using a function
which has been written in order to perform the second pass.*/
MEMIM *memAdd(char*,char*,char*);

/*This function takes a memoryImage node and adds the node to the corresponding memoryImage linked-list (of commands or data).
@param headCom-the head of the memoryImage commands linked-list.
@param headData-the head of the memoryImage data linked-listd.
@param node-the node which will be added to one of the memoryImage linked-lists.
@param firstDataNodeAddflag- flag to indicate if data has been detected already.
@param firstComNodeAddflag - flag to indicate if com has been detected already..
The function is checking whether the added node is a "command type" node or "data type" node.
Correspondingly-the added node will be added to the suitable linked-list.*/
void addNode(MEMIM *headCom,MEMIM *headData, MEMIM *node,int firstDataNodeAddflag,int firstComNodeAddflag);

/*The symbolTable function receives a symbol of type pointer to char, value of type int, attribute1 of type int, attribute 2 of type int, and firstSymbolFlag of type int, and returns the head of the symbol table of type pointer to TABLE_NODE_T. The symbol (label) should be inputted in the first argument, following by its corresponding value (address), attribute1, attribute2, and the final argument, firstSymbolFlag, should be on if it is the first call to the function and the symbol table needs to be initialized, and should be off it is not the first call to the function and an additional node will be added to the existing symbol table. The function returns the head of the symbol table of type pointer to TABLE_NODE_T which contains the fields, symbol of type pointer to char, value of type int, and attributes which is an array of ints, and a pointer to the next node of the symbol table.*/
TABLE_NODE_T* symbolTable(char *symbol,int value,int attribute1,int attribute2,int firstSymbolFlag);


/*This function takes two memoryImage linked-list heads and concatenates two lists to a single list.
The first list is the commands linked-list, the second list is the data linked-list.
@param headCom-the head of the commands memoryImage linked-list.
@param headData-the head of the data memoryImage linked-list.
The function is looping over the commands list until the end of the list has been reached, then-the next field of the last node of the
commands linked-list is changed to pointed to the head of the data linked-list.*/
void concatNodes(MEMIM *headCom,MEMIM *headData);

/*The secondPass function receives linesHead of type pointer to LINE_FIELDS_T, tableHead of type pointer to TABLE_NODE_T, ICF of type int, DCF of type int, filename of type pointer to char, memImHead of type pointer to MEMIM, symbolTableInitFlag of type int, and does not return anything as it is a void function. The function iterates through the second pass algorithm after it is called from the firstPass function, and if no errors were detected, the algorithm proceeds to complete the memory image and finally outputs the corresponding files (.ob,.ext.ent).*/
void secondPass(LINE_FIELDS_T* linesHead, TABLE_NODE_T* tableHead, int ICF, int DCF, char *filename, MEMIM* memImHead,int symbolTableInitFlag);



