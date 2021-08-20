

#define UNKNOWNSTATE -1 /*constant for unknown state*/


/*struct to store linked list of line fields*/
typedef struct fieldBuffer{
    char *field; /*stores field (e.g. label or command or directive or values*/
    struct fieldBuffer *next; /*next node in linked list*/
}FIELD_BUFFER_T;


/*The checkState function receives ptrInput of type pointer to char and returns an int of the current state of the state machine in manageContents. The function acheives this by comparing the string in ptrInput to all the possible commands and directives and returns the corresponding state as an integer or UNKNOWNSTATE (-1) if the state is unknown.*/
int checkState(char *ptrInput);


/*The addToFieldBuffer function receives field of type pointer to char and firstFieldFlag of type int and returns a pointer to FIELD_BUFFER_T. The function uses the firstFieldFlag to indicate the head of the buffer (when flag is on) and when to add to the buffer (when flag is off), and creates a buffer of all the fields indicated as input, and returns the head of the buffer. This function is used from manageContents to store all the nodes the are allocated so they can be freed later using the buffer.*/
FIELD_BUFFER_T* addToFieldBuffer(char *field,int firstFieldFlag);


/*The freeFields function receives head of type pointer to FIELD_BUFFER_T and returns nothing as it is a void function. The function receives the head of the field buffer created in the addToFieldBuffer function, and iterates through the buffer and frees each field and struct from within the buffer.*/
void freeFields(FIELD_BUFFER_T *head);


/*The checkCommand function receives ptrCode of type pointer to char, ptrCommand of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char which holds the checked code. The function analyzes the characters stored in ptrCode according to the type of command stored in ptrCommand, and also takes the line number and file name as inputs in order to print out any error messages with the corresponding file name and line number. The function returns the checked code if no errors were detected and returns NULL if an error was detected (and an error message will be printed)*/
char* checkCommand(char *ptrCode,char *ptrCommand,int lineNumber,char *filename);


/*The checkData function receives ptrData of type pointer to char, ptrDirective of type pointer to char, lineNumber of type int, and filename of type pointer to char, and returns a pointer to char which holds the checked data. The function analyzes the characters stored in ptrCode according to the type of directive stored in ptrData, and also takes the line number and file name as inputs in order to print out any error messages with the corresponding file name and line number. The function returns the checked data if no errors were detected and returns NULL if an error was detected (and an error message will be printed)*/
char* checkData(char *ptrData,char *ptrDirective,int lineNumber,char *filename);


/*The newLine function receives errorDetected of type pointer to int, current of type pointer to pointer to NODE_T, labelFlag ot type pointer to int, and index of type pointer to int, and returns an int which resets the state to PREFIRSTWORD. The function is used when analyzing each line of the input file, and allows for the analysis of the next line. It does so by setting the errorDetected flag to off, sets the current node to the next node, sets the labelFlag to off, and resets the index back to zero. Finally the function returns the state PREFIRSTWORD of type int (defined in enum) which is used to reset the state machine switch case back to its initial state*/
int newLine(int *errorDetected,NODE_T **current,int *labelFlag,int *index);


/*The checkExtraneousChars function receives current of type pointer to pointer to NODE_T, and index of type pointer to int, and returns an error flag of type int. The function receives the current node with is the current line analyzed from the input file, and the current index to access the current character in the line. The function iterates over the remaining characters in the line and ensures there are not any additional unexpected characters that are not white space characters. If no extraneous characters were detected the function returns an error flag of type int as off, otherwise the flag will be returned as on.*/
int checkExtraneousChars(NODE_T **current,int *index);

/*The firstPass function receives ptrField1 of type pointer to char, ptrField2 of type pointer to char, ptrField3 of type pointer to char, labelFlag of type int, errorDetected of type int, filename of type pointer to char, lineNumber of type int, and returns nothing as it is a void function. The function is called from manageContents for every line of the input file, with ptrField1 containing an optional pointer, ptrField2 containing a command or directive, ptrField3 containing the corresponding values, labelFlag symboling if a label was defined, errorDetected symboling if error was detected, filename with the name of the current file, and lineNumber with the current line number. The function begins to build the symbol table and memory image and calls the secondPass function if no errors were detected.*/
void firstPass(char *ptrField1,char *ptrField2,char *ptrField3,int labelFlag,int errorDetected, char *filename,int lineNumber);


