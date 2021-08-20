
/*The createOutputFiles function receives memImHead of type pointer to MEMIM, tableHead of type pointer to TABLE_NODE_T, filename of type pointer to char, external head of type pointer to SYMBOL_ADD_STRUCT_T, ICF of type int, DCF of type int, errorFlag of type int, symbolTableInitFlag of type int and externalFlag of type int, and does not return anything, but creates and writes to files if necessary. The function creates output files of type .ob,.ext, and .ent. To do this, the file receives as inputs the head of the memory image, memImHead, which is used to print to the object file, the head of the symbol table, tableHead, which is used to create the extern file, the input file name which is used to name the output files, externalHead which is used to create the extern files, ICF and DCF which are used to label the object file, errorFlag which alerts if files should be printed or not, symbolTableInitFlag which alerts if symbol table was initialized, and externalFlag, which alerts if externals were used and if an ext file should be created.*/
void createOutputFiles(MEMIM* memImHead, TABLE_NODE_T* tableHead, char *filename, SYMBOL_ADD_STRUCT_T *externalHead, int ICF, int DCF,int errorFlag,int symbolTableInitFlag,int externalFlag);

/*The freeSymbolAddNewStruct function receives headStructPtr of type SYMBOL_ADD_STRUCT_T* and returns nothing as it is a void function. The function iterates through the list of nodes returned from symbolAddNew and frees every node until the end of the linked list is reached.*/
void freeSymbolAddNewStruct(SYMBOL_ADD_STRUCT_T *headStructPtr);


/**This function is used by the second-pass process.
  The function checks if a given node is missing a label.
  If there is a missing label-the function will figure out if the value of the missing label is an address
  or if it is a distance between the instruction counter and the label address.
  If the missing label is external-the value of the label will be loaded with "zero bits".
  If the missing label haven't been found in the symbol table-an error flag will be raised.
  @param head-the head of the memory image linked-list.
  @param table-the head of the symbol table.
  @param lineNumber-the number of a given line.
  @param firstEntry-A flag which indicates the first entrance to the function.
  @param *filename-a pointer to the file name.
  @return structPtr-a struct which contains the symbol value, the address of the symbol, an external flag and an error flag.*/
SYMBOL_ADD_STRUCT_T* symbolAddNew(MEMIM *head,TABLE_NODE_T* table,int lineNumber,int firstEntry,char *filename);


/**This function is responsible for writing the final output (the ICF, DCF and the hexadecimal code) to the output files.
  @param head-the head of the memory image.
  @param fptrObject-A pointer to the output file.
  The function is iterating over each memory image node, for each node-the binary string are converted to hexadecimal
  values. 
  If a given node is a command node-the 32 bits of the command node are converted to 8 hexadecimal chars.
  If a given node is a data node-each byte of data, from the data linked list, is converted to a single hexadecimal char.
  If a single line contains 8 hexadecimal chars-the next hexadecimal char will be printed in the next line.*/
void printListToFile(MEMIM *head,FILE *fptrObject);


