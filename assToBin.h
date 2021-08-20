
#define MAX_POS_8_BITS_NUM 127 /*max positive number for 8 bits.*/
#define MAX_NEG_8_BITS_NUM -128 /*max negative number for 8 bits.*/
#define MAX_POS_16_BITS_NUM 32767 /*max positive number for 16 bits.*/
#define MAX_NEG_16_BITS_NUM -32768 /*max negative number for 16 bits.*/
#define LENCOMMASTRING 2 /*Length of comma.*/
#define MINREG 0 /*The minimum reg index.*/
#define MAXREG 31/*The maximum reg index.*/
#define REGMEM 6 /*Number of allocated chars to store a reg.*/
#define INDEX_OF_FIRST_BRANCH_COM 5 /*The index of first branching command.*/
#define START_COUNT 1 /*The index of the start count.*/
#define NUM_OF_REGS 3 /*Number of registers in a command line.*/
#define INDEX_ONE 1 /*Index number one.*/
#define NUM_OF_LOGICAL_COM 5 /*The number of logical commands.*/
#define INDEX_OF_FIRST_STORE_COM 8 /*The index of the first store command*/
#define INDEX_TWO 2 /*Index number 2*/
#define LABEL_IS_MISSING_BRANCH_COM 2 /*A flag which indicates that the missing label is in a branch command.*/
#define INDEX_OF_STOP_COM 3/*Index of a stop command.*/
#define ZERO_VALUE 0/*A zero value.*/
#define SINGLE_BYTE 1/*A single byte*/
#define WORD_SIZE_IN_BYTES 4 /*Number of bytes in a word*/
#define THREE_BYTES_IN_BITS 24 /*Number of bits in 3 bytes.*/
#define BYTE_IN_BITS 8 /*Number of bits in a byte.*/
#define THREE_BYTES 3 /*Three bytes.*/
#define HALF_WORD_SIZE_IN_BYTES 2 /*A size of half word in bytes.*/
#define REG_INDEX_OF_NULL_CHAR 5 /*The index of the null terminator in reg string.*/
#define REG_LAST_BIT_INDEX 4 /*The index of the last bit in the reg string.*/
#define TWO 2 /*Number 2*/
#define NUM_ALLOC_BITS_FOR_16_BITS_IMM 17 /*Number of allocated bits for 16 bits number.*/
#define BITS_16 16 /*Number of bits in 16 bits immediate*/
#define FIRST_INSTRUCTIONS_COUNT 100 /*The value of the first address of the instructions*/
#define ASCII_INDEX_OF_ZERO 48 /*Ascii index of zero*/
#define ASCII_INDEX_OF_A_MINUS_TEN 55 /*Ascii index of 'A' minus 10*/
#define LABEL_IS_MISSING_J_COM 1 /*A flag which indicates that the missing label is in a j command.*/
#define SINGLE_BIT 1 /*A single bit*/
#define NUM_OF_BITS_IN_WORD 32 /*Number of bits in a word.*/
#define NUM_OF_BITS_IN_WORD_PLUS_NULL 33 /*Number of allocated bits for 32 bits number.*/
#define NUM_OF_BITS_HALF_WORD 16 /*Number of bits in half word*/
#define NUM_OF_BiTS_FOR_IMMJ 27 /*Number of bits allocated for J-type immediate.*/
#define NUM_OF_BITS_J_TYPE_IMM 25 /*Number of bits in J-type immediate*/
#define BITS_IN_BYTE_PLUS_NULL 9 /*Number of allocated bits for 8 bits number.*/
#define INDEX_TEN 10 /*Index 10.*/
#define BASE_TWO 2 /*Base of 2.*/
#define INDEX_THREE 3/*Index number 3.*/
#define MAX_BITS_IN_HEX 4 /*Maximum bits to represent a hexadecimal number.*/
#define NUM_OF_CON_HEX 2 /*Number of consecutive hex chars.*/
#define LAST_INDEX_OF_BYTE 7 /*The index of the last bit in a byte.*/


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


/*This function takes a binary string of a single byte (8 bits) and returns the suitable hex char.
@param bin-a pointer to a binary string.
@return hex-a single char.*/
char binToHex(char *bin);

/**
This function checks if a given number (given as a string) is in a word size (32 bits).
@param str1-The num string.
@return 0 if a number is not in a word size, else-return 1.
This function compares 2 strings which represents numbers and compares the sizes of the string.
If the size is equal-the function compares each single digit of the numbers.*/
int isInt(char *str1);


