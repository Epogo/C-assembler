
#define FILEEXTENSIONPOS1 3 /*position from end of file extension "." of ".as"*/
#define FILEEXTENSIONPOS2 2 /*position from end of file extension "a" of ".as"*/
#define FILEEXTENSIONPOS3 1 /*position from end of file extension "s" of ".as"*/
#define ARGCOUNTERSTART 1 /*first index of argument vector with file inputs*/

/*The handleFileContents function receives fd of type pointer to FILE and filename of type pointer to char. The function uses the file pointer fd to call the storeLines function along with a pointer to node that is allocated in the function to store the characters from each line node by node. The head of these nodes is passed to the manageContents function for further analysis unless the input of the file is detected to be empty. The function is void and does not return anything.*/
void handleFileContents(FILE *fd, char *filename);
