

/*This function takes as an input a register string and then-convert the string to a binary string.
@param reg-The register string (with the "$" sign).
@return str-the returned binary string from the function.
The function gets the register string from the user, converts the string to a int number, then-converts the number to a binary number
and then-the binary number is converted to a string.*/
char *Registers(char*);


/*This function takes an integer and return a string of 16 bits which represents an immediate number (for i type commands).
@param num-the immediate num which should be converted.
@return str-bits string.
The function is using bit manipulation in order to convert a given number (positive or negative) to a bits string.*/
char *decToBin(int);


/*This function takes an integer and return a string of 25 bits which represents an immediate number (for j type commands).
@param num-the immediate num which should be converted.
@return str-bits string.
The function is using bit manipulation in order to convert a given number (positive or negative) to a bits string.*/
char *decToBinJ(int);


/*This function takes an integer and return a string of 8 bits which represents an Ascii char .
@param num-the Ascii num which should be converted to bits string.
@return str-bits string.
The function is using bit manipulation in order to convert a given number to a bits string.*/
char *ascizToBin(int);


/*This function takes a number (which is represented as a string) and convers it to 32 bits string (a word).
@param number-a number which is represented as a string.
@return str-bits string.
The function is using bit manipulation in order to convert a given number to a bits string.*/
char *decToBinDirW(char*);

/*This function takes a number (which is represented as a string) and convers it to 16 bits string (an half word).
@param number-a number which is represented as a string.
@return str-bits string.
The function is using bit manipulation in order to convert a given number to a bits string.*/
char *decToBinDirH(char*);

/*This function takes memoryImage linked-list head and frees all the memory which has been occupied by the linked-list nodes.
@param node-the head of the memoryImage linked-list.*/
void deleteNode(MEMIM*);


/*This function takes a binary string of a single byte (8 bits) and returns the suitable hex char.
@param bin-a pointer to a binary string.
@return hex-a single char.
*/
char binToHex(char *bin);




