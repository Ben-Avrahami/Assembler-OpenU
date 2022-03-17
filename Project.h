/*
 * Include for libraries needed for the Assembler
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * Defines that will be used in the Assembler program
 */
#define MINUS_TWO -2
#define MINUS_ONE -1
#define FALSE 0
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 9
#define TEN 10
#define ELEVEN 11
#define TWELVE 12
#define THIRTEEN 13
#define FOURTEEN 14
#define FIFTEEN 15
#define SIXTEEN 16
#define START 1
#define TRUE 1
#define LOOP_START 1
#define STEP 1
#define EXIT 1
#define RAM_START 100
#define COMMAND_NAME_LENGTH 5
#define NUM_OF_REGISTER 16
#define TURN_OFF_BITS_FOR_NEGATIVE_NUMBERS 327679
#define MAX_LINE_SIZE 81
#define MAX_LABEL_SIZE 32
#define SAVED_WORDS_AMOUNT 36
#define MAX_ATTRIBUTE_LINE_SIZE 10
#define MAX_ATTRIBUTES 4
#define AMOUNT_OF_COMMANDS 16
#define MIN_ARGUMENTS 2
#define RAM_SIZE 8192
#define SIXTEEN_BITS_ON  65535
#define FOUR_BITS_ON    15
#define FOUR_BITS 4
#define EIGHT_BITS 8
#define TWELVE_BITS 12
#define SIXTEEN_BITS 16

/* Represents the location of parts in the binary*/
#define TARGET_REGISTER 2
#define ORIGIN_ADDRESS 6
#define ORIGIN_REGISTER 8
#define FUNCT 12
#define INT_E 16
#define INT_R 17
#define INT_A 18

/*
 * This struct for keeping a command and its numeric values
 */
typedef struct command {
    char name[COMMAND_NAME_LENGTH];
    int opcode;
    int funct;
} command;

/*
 * This struct to save a macro and its contents
 */
typedef struct macro {
    char name[MAX_LINE_SIZE];
    char content[MAX_LINE_SIZE * MAX_LINE_SIZE];
} macro;

/*
 * This struct for keeping a label and their details
 */
typedef struct label {
    char name[MAX_LABEL_SIZE];
    int length;
    int address;
    int base;
    int offset;
    char attributes[MAX_ATTRIBUTES][MAX_ATTRIBUTE_LINE_SIZE];
    int attributesCount;
} label;


/*Functions prototypes*/

int makeItArray(char line[MAX_LINE_SIZE], char **wordArray);

void removeNewLine(char line[MAX_LINE_SIZE], char word[MAX_LINE_SIZE]);

int checkLabelName(char word[], label labels[], int labelCount);

void enterBaseAndOffset(label *currentLabel, int RAMIndex);

void makeIntLine(char *wordArray[MAX_LINE_SIZE], int wordAmount, char tempLine[MAX_LINE_SIZE], int labelFlag);

int countCommas(char line[MAX_LINE_SIZE]);

int makeItNumArray(int IntArray[MAX_LINE_SIZE], char intLine[MAX_LINE_SIZE]);

void labelWithoutColon(char labelBefore[MAX_LINE_SIZE], char labelWithoutColon[MAX_LINE_SIZE]);

int searchCommand(command commands[], char *name);

char *remove_white_spaces(char *str);

int lineForCommand(char *wordArray[MAX_LINE_SIZE], int labelFlag, int wordAmount,
                   char commandOperands[MAX_LINE_SIZE][MAX_LINE_SIZE], int lineCounter);

int numOfOperandsInLine(char *line);

void numOfOperandsNotMatch(int lineCounter);

int findAddressMethod(char *operand, char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE], int lineCounter);

int addLength(int addressingMethod);

int findRegister(char charRegister[MAX_LINE_SIZE], char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE], int addressMethod);

int getNumAddressMethod1(char word[MAX_LINE_SIZE]);

void changeMemoryFirstCross(int firstOperandAddressMethod, int secondOperandAddressMethod, int *RAM, int IC,
                            int commandIndex, command commands[], int numOfOperands,
                            char registerLabel[MAX_LINE_SIZE][MAX_LINE_SIZE],
                            char commandOperands[MAX_LINE_SIZE][MAX_LINE_SIZE]);

int macrosDeploy(FILE *fp, FILE *newfp);

int firstPass(FILE *newfp, int RAM[RAM_SIZE], label **labels, int *labelCount, command commands[],
              char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE], int *DCF, int *extFlag, int *entryFlag);

int secondPass(FILE *newfp, int RAM[RAM_SIZE], label **labels, int labelCount, command commands[],
               char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE], label **externs, int *externsCount);

int labelNum(char *labelName, label *labels, int labelsCount);

void createExternFile(FILE *externFile, label *externs, int externsCount);

void createObjectFile(FILE *objectFile, int DCF, int ICF, int RAM[RAM_SIZE]);

void createEntryFile(FILE *entryFile, label **labels, int labelsCount);

int checkAddressMethod3Possibility(char *operand);

int removesNewLine(char *str);


