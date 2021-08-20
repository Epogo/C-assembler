

/*The checkState function receives ptrInput of type pointer to char and returns an int of the current state of the state machine in manageContents. The function acheives this by comparing the string in ptrInput to all the possible commands and directives and returns the corresponding state as an integer or UNKNOWNSTATE (-1) if the state is unknown.*/
int checkState(char *ptrInput);



/*The addToFieldBuffer function receives field of type pointer to char and firstFieldFlag of type int and returns a pointer to FIELD_BUFFER_T. The function uses the firstFieldFlag to indicate the head of the buffer (when flag is on) and when to add to the buffer (when flag is off), and creates a buffer of all the fields indicated as input, and returns the head of the buffer. This function is used from manageContents to store all the nodes the are allocated so they can be freed later using the buffer.*/
FIELD_BUFFER_T* addToFieldBuffer(char *field,int firstFieldFlag);


/*The freeFields function receives head of type pointer to FIELD_BUFFER_T and returns nothing as it is a void function. The function receives the head of the field buffer created in the addToFieldBuffer function, and iterates through the buffer and frees each field and struct from within the buffer.*/
void freeFields(FIELD_BUFFER_T *head);
