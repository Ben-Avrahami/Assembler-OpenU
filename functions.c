#include "Project.h"

/*
 * This method divides the sentence into an array that contains the words of the sentence
 */
int makeItArray(char line[MAX_LINE_SIZE], char **wordArray) {
    int wordIndex = ZERO;
    char *token; /* Will be used with strtok method*/

    token = strtok(line, " ");
    while (token != NULL) {
        wordArray[wordIndex] = token;
        wordIndex++;
        token = strtok(NULL, " ");
    }
    return wordIndex;
}

/*
This method removes a new line from the end of the line if there is
Changes the word "word" to be the updated word without removing a line at the end
*/
void removeNewLine(char line[MAX_LINE_SIZE], char newWord[MAX_LINE_SIZE]) {
    int charIndex = ZERO;
    while (line[charIndex] != '\n') {
        newWord[charIndex] = line[charIndex];
        charIndex++;
    }
    newWord[charIndex] = '\0';

}

/*
 * This method wil return:
 * -2 if there is a problem with the label name
 * -1 if the label name wasn't found
 * Else the number of the label in the labels array
 */
int checkLabelName(char word[], label labels[], int labelCount) {
    int savedWordIndex = ZERO;
    int labelsIndex = ZERO;
/*  An array containing the saved words of the program*/
    char savedWord[SAVED_WORDS_AMOUNT][TEN] =
            {

                    {"mov"},
                    {"cmp"},
                    {"add"},
                    {"sub"},
                    {"lea"},
                    {"clr"},
                    {"not"},
                    {"inc"},
                    {"bec"},
                    {"jmp"},
                    {"bne"},
                    {"jsr"},
                    {"red"},
                    {"prn"},
                    {"rts"},
                    {"stop"},
                    {"r0"},
                    {"r1"},
                    {"r2"},
                    {"r3"},
                    {"r4"},
                    {"r5"},
                    {"r6"},
                    {"r7"},
                    {"r8"},
                    {"r9"},
                    {"r10"},
                    {"r11"},
                    {"r12"},
                    {"r13"},
                    {"r14"},
                    {"r15"},
                    {".data"},
                    {".string"},
                    {".entry"},
                    {".extern"}
            };
/* Compares the given label name to all saved words to make sure it isn't identical*/
    for (savedWordIndex = ZERO; savedWordIndex < SAVED_WORDS_AMOUNT; savedWordIndex++) {
        if (strcmp(word, savedWord[savedWordIndex]) == ZERO) {
            return MINUS_TWO;
        }

    }
/*  Compares the given label name to all previous labels names to make sure it isn't identical to any of the previous labels*/
    for (labelsIndex = ZERO; labelsIndex < labelCount; labelsIndex++) {
        if (strcmp(word, labels[labelsIndex].name) == ZERO) {
            return labelsIndex;     /* Returns the index of the label with the same name*/
        }
    }
    return MINUS_ONE;
}

/*
 * This method calculates and sets the base and offset for labels
 */
void enterBaseAndOffset(label *currentLabel, int RAMIndex) {
    currentLabel->address = RAMIndex;
    currentLabel->base = (RAMIndex - (RAMIndex % SIXTEEN));
    currentLabel->offset = (RAMIndex % SIXTEEN);
}

/*
 * Enter the array to a string of numbers
 */
void makeIntLine(char *wordArray[MAX_LINE_SIZE], int wordAmount, char tempLine[MAX_LINE_SIZE], int labelFlag) {
	int ind;
    strcpy(tempLine, "");
    for (ind = ONE + labelFlag; ind < wordAmount; ind++) {
        strcat(tempLine, wordArray[ind]);
    }
    removesNewLine(tempLine);
}

/*
 * Count the number of commas in a row
 */
int countCommas(char line[MAX_LINE_SIZE]) {
    int ind;
    int count = ZERO;
/*  Counts how many commas are and returns the count*/
    for (ind = START; ind < strlen(line); ind++) {
        if (line[ind] == ',') {
            count++;
        }
    }
    return count;
}

/*
 * Prepares an array of numbers from the data in a row
 */
int makeItNumArray(int IntArray[MAX_LINE_SIZE], char intLine[MAX_LINE_SIZE]) {
    int index = ZERO;
    char tempIntCharLines[MAX_LINE_SIZE][MAX_LINE_SIZE];
    int intArrayIndex;
    int tempNum;
    char *endPtr;
    char *token;

    removesNewLine(intLine);

    token = strtok(intLine, ",");
    /* Checking that we did not reach the end of the string */
    while (token != NULL) {
        strcpy(tempIntCharLines[index], token);
        token = strtok(NULL, ",");
        index++;
    }
    /* Check if hte current cell is number without char - if it is fo add it to the array - else return -2*/
    for (intArrayIndex = ZERO; intArrayIndex <= index; intArrayIndex++) {
        tempNum = strtol(tempIntCharLines[intArrayIndex], &endPtr, TEN);

        /*Check if the first char is not end of line ir end of string*/
        if (*endPtr != '\0' && *endPtr != '\n') {
            return MINUS_TWO;
        }
        IntArray[intArrayIndex] = tempNum;
    }
    return index;
}

/*
 * Removes the colon of the label
 */
void labelWithoutColon(char labelBefore[MAX_LINE_SIZE], char labelWithoutColon[MAX_LINE_SIZE]) {
    char *token;

    token = strtok(labelBefore, ":");
    strcpy(labelWithoutColon, token);
}

/*
 * This method used to find the number of command in the commands array
 */
int searchCommand(command commands[], char *name) {
    int i = ZERO;
    removesNewLine(name);
    for (i = ZERO; i < AMOUNT_OF_COMMANDS; i++) {
        if ((strcmp(name, commands[i].name)) ==
            ZERO) {   /* The name sent has the same name as one of the commands in commands array*/
            return i;
        }
    }
    return MINUS_ONE;
}

/*
 * This method does 2 tasks:
 * 1. Check if the commas and the operands are balanced
 * 2. Split the operands to different cells in commandOperands array
 *
 * Return -1 if the commas and the operands are not balanced
 * Else will return the number of operands;
 */
int lineForCommand(char *wordArray[MAX_LINE_SIZE], int labelFlag, int wordAmount,
                   char commandOperands[MAX_LINE_SIZE][MAX_LINE_SIZE], int lineCounter) {
    char line[MAX_LINE_SIZE];
    int index;
    char betterLine[MAX_LINE_SIZE];
    char *token;
    int numOfOperands = ZERO;
    int numOfCommas = ZERO;

    /*
     * Make the operands one line seperated by " " or ",";
     */
    strcpy(line, wordArray[labelFlag + ONE]);
    for (index = labelFlag + TWO; index < wordAmount; index++) {
        strcat(line, wordArray[index]);
        strcat(line, " ");
    }
    /*
     * Count how many operands the line contain
     */
    numOfOperands = numOfOperandsInLine(line);
    /*
     * Count how many commas the line contain
     */
    numOfCommas = countCommas(line);

    /*
     * Checks whether the number of commas and the number of operands are balanced
     */
    if (numOfCommas + ONE != numOfOperands) {
        /* Error */
        printf("Line %d - the number of commas and number of operands is not matching accordingly\n", lineCounter);
        return MINUS_ONE;
    }

    /*
     * Removes all white spaces in the line
     */
    strcpy(betterLine, remove_white_spaces(line));

    index = ZERO;

    /*
     * Separate the line by ","
     */
    token = strtok(betterLine, ",");
    /* Checking that we did not reach the end of the string */
    while (token != NULL) {
        strcpy(commandOperands[index], token);
        token = strtok(NULL, ",");
        index++;
    }

    return index;
}

/*
 * This method will remove all spaces from a line of chars
 */
char *remove_white_spaces(char *str) {
    int i = ZERO, j = ZERO;
    while (str[i]) {
        if (str[i] != ' ')
            str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
    return str;
}

/*
 * This method count how many operands are in the line
 * Every operands in the line separated by " " or ","
 */
int numOfOperandsInLine(char *line) {
    int index = ZERO;
    int operandCount = ZERO;
    int operandFlag = ZERO;

    while (line[index] != '\0') {
        if (operandFlag == ZERO && line[index] != ' ' &&
            line[index] != ',') /* If we reached a ' ' or ',' will increase the count*/
        {
            operandCount++;
            operandFlag = TRUE;     /* Turns on the operand flag */
        }

        if (line[index] == ' ' || line[index] == ',')/* If we reached another ' ' or ',' */
        {
            operandFlag = ZERO;/* Turns off the operand flag */
        }
        index++;
    }
    return operandCount;/* Returns operand count*/
}

/* This method prints an error when the number of operands does not match */
void numOfOperandsNotMatch(int lineCounter) {
    printf("Line %d -The number of operands does not match ", lineCounter);
    return;
}

/*
 * Finds the addressing method by the input
 */
int findAddressMethod(char *operand, char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE], int lineCounter) {
    int numberIndex = ONE;
    int registerIndex;
    char labelToken[MAX_LINE_SIZE];
    char registerToken[MAX_LINE_SIZE];
    char tempOperand[MAX_LINE_SIZE];
    int label3possibility = ZERO;
    removesNewLine(operand);
    if (operand[ZERO] == '#') {
        while (operand[numberIndex] != '\0' && operand[numberIndex] != '\n') {
            if (((isdigit(operand[numberIndex])) == ZERO) &&
                operand[numberIndex] != '-')/* Checks if all the chars are digits or '-'*/
            {
                printf("Line %d- operand is not correct\n", lineCounter);
                return MINUS_ONE;
            }
            numberIndex++;/* Increases number index by one*/
        }
        return ZERO;/* Addressing method zero*/
    }


    /*
     * Check if this is a register
    */

    for (registerIndex = ZERO; registerIndex < NUM_OF_REGISTER; registerIndex++) {
        if ((strcmp(operand, registerTable[registerIndex])) == ZERO) {
            return THREE;/* Addressing method three*/
        }
    }


    /* Checks whether this is the second addressing method */


    /*check if it's possible to be address method 3*/
    label3possibility = checkAddressMethod3Possibility(operand);

    strcpy(tempOperand, operand);
    if (label3possibility == TRUE) {
        /* Separates the addressing method for the command and registers and checks if they are correct */
        strcpy(labelToken, strtok(tempOperand, "["));
        strcpy(registerToken, strtok(NULL, "]"));

        for (registerIndex = TEN; registerIndex < NUM_OF_REGISTER; registerIndex++) {
            if ((strcmp(registerToken, registerTable[registerIndex])) == ZERO) {
                return TWO;/* Addressing method two */
            }
        }
    }
    return ONE; /* Means may be a label that defined later */
}


/*
 * This method returns the number of additional words to be added according to the addressing method from the receipt
 */

int addLength(int addressingMethod) {
    if (addressingMethod == ZERO) {
        return ONE;
    } else if (addressingMethod == ONE || addressingMethod == TWO) {
        return TWO;
    } else {

    }
    return ZERO;

    /* Returns a value depends on the addressing method */
}

/*
 *The method receives the command name and addressing methods and fills the appropriate memory accordingly
 */
void changeMemoryFirstCross(int firstOperandAddressMethod, int secondOperandAddressMethod, int *RAM, int IC, int commandIndex, command commands[], int numOfOperands,char registerLabel[MAX_LINE_SIZE][MAX_LINE_SIZE],char commandOperands[MAX_LINE_SIZE][MAX_LINE_SIZE]) {
    int registerNumber = ZERO;
    /*
     * Enter the opcode line
     */

    RAM[IC] |= (ONE << commands[commandIndex].opcode);       /* Inserts the opcode to the first cell in the label memory*/
    RAM[IC] |= (ONE << INT_A);  /*Turns on the bit in the place of A (the 18 bit)*/

    /*
    * If mumOfOperands equals ZERO then there is only one line, no need to create another line for the command
     */
    if (numOfOperands != ZERO) {

        IC++;   /* Move to the next line*/
        RAM[IC] |= (commands[commandIndex].funct << FUNCT);    /* Inserts the funct to its cell in the label memory*/
        RAM[IC] |= (ONE << INT_A);  /*Turns on the bit in the place of A (the 18 bit)*/
    }

    /*
     * If the method gets 1 operand
     */
    if (numOfOperands == ONE) {

        RAM[IC] |= (firstOperandAddressMethod);    /*Inserts the target addressing method to its cell in the label memory*/

        if (firstOperandAddressMethod == TWO || firstOperandAddressMethod == THREE) {
            registerNumber = findRegister(commandOperands[ZERO], registerLabel, firstOperandAddressMethod);

            RAM[IC] |= (registerNumber<< TARGET_REGISTER);     /*Inserts the target register to its cell in the label memory*/

        } else if (firstOperandAddressMethod == ZERO) {
            IC++;
            RAM[IC] |= getNumAddressMethod1(commandOperands[ZERO]); /* Inserts the target addressing method to its cell in the label memory*/
            RAM[IC] &= SIXTEEN_BITS_ON;    /*Turns off bits higher then 16*/
            RAM[IC] |= (ONE << INT_A);  /*Turns on the bit in the place of A (the 18 bit)*/
        }


    }

    /*
     * If the method gets 2 operand
     */
    if (numOfOperands == TWO) {
        /*
         * Check the first Line need to be entered
         */
        {
            /*Insert ORIGIN_ADDRESS*/
            RAM[IC] |= (firstOperandAddressMethod << ORIGIN_ADDRESS);

            /*Check if ORIGIN register need to be change*/
            if (firstOperandAddressMethod == TWO || firstOperandAddressMethod == THREE) {
                registerNumber = findRegister(commandOperands[ZERO], registerLabel, firstOperandAddressMethod);
                RAM[IC] |= (registerNumber << ORIGIN_REGISTER); /* Inserts the origin register to its cell in the label memory*/
            }

            /* Insert TARGET_ADDRESS method*/
            RAM[IC] |= (secondOperandAddressMethod);

            /*Check if ORIGIN register need to be change*/
            if (secondOperandAddressMethod == TWO || secondOperandAddressMethod == THREE) {
                registerNumber = findRegister(commandOperands[ONE], registerLabel, secondOperandAddressMethod);
                RAM[IC] |= (registerNumber << TARGET_REGISTER);     /* Inserts the target register to its cell in the label memory*/
            }
            IC++;
        }

        /*
         *  First Address method add words
         */

        if (firstOperandAddressMethod == ZERO) {
            RAM[IC] |= getNumAddressMethod1(
                    commandOperands[ZERO]); /* Inserts the target addressing method to its cell in the label memory*/
            RAM[IC] &= SIXTEEN_BITS_ON; /* Turns off bits higer then 16*/
            RAM[IC] |= (ONE << INT_A); /* Turns on the bit in the place of A (the 18 bit)*/
            IC++;
        }

        if (firstOperandAddressMethod == ONE || firstOperandAddressMethod == TWO) {
            IC += TWO;
        }


        /*
        * Check second operand method add words
        */

        if (secondOperandAddressMethod == ZERO) {
            RAM[IC] |= getNumAddressMethod1(
                    commandOperands[ONE]); /*Inserts the target addressing method to its cell in the label memory*/
            RAM[IC] &= SIXTEEN_BITS_ON; /* Turns off bits higer then 16*/
            RAM[IC] |= (ONE << INT_A); /* Turns on the bit in the place of A (the 18 bit)*/
        }
    }
    return;
}

/*
 * This method converts a string containing the register name to a int representing the register index number in the array of registers
 */
int
findRegister(char charRegister[MAX_LINE_SIZE], char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE], int addressMethod) {
    int registerIndex = ZERO;
    char labelToken[MAX_LINE_SIZE];
    char registerToken[MAX_LINE_SIZE];

    if (addressMethod == THREE) {
        for (registerIndex = ZERO; registerIndex < NUM_OF_REGISTER; registerIndex++) {
            if ((strcmp(charRegister, registerTable[registerIndex])) == ZERO) {
                return registerIndex;   /* Returns the register index*/
            }
        }
        return MINUS_ONE;   /* Shouldn't reach here, would have fallen before in the previous methods */
    } else    /* If its address method 2*/
    {
        strcpy(labelToken, strtok(charRegister, "["));
        strcpy(registerToken, strtok(NULL, "]"));

        for (registerIndex = ZERO; registerIndex < NUM_OF_REGISTER; registerIndex++) {
            if ((strcmp(registerToken, registerTable[registerIndex])) == ZERO) {
                return registerIndex;   /* Returns the register index*/
            }
        }
        return MINUS_ONE;   /* Shouldn't reach here, would have fallen before in the previous methods */
    }
}

/*
 * This method gets a string and converts it to a int works for positive and negative numbers
 * The method ignores the first char which is supposed to be '#'
 */
int getNumAddressMethod1(char word[MAX_LINE_SIZE]) {
    int tempNum = ZERO;
    char *endPtr;

    tempNum = strtol((word + ONE), &endPtr, TEN);

    return tempNum;
}

/*
 *  This method finds the label number in the table
 */
int labelNum(char *labelName, label *labels, int labelsCount) {

    int labelIndex = ZERO;
    char token[MAX_LINE_SIZE];
    if ((checkAddressMethod3Possibility(labelName)) == TRUE) {
        strcpy(token, strtok(labelName, "["));
        strcpy(labelName, token);
    }

    removesNewLine(labelName);

    for (labelIndex = ZERO; labelIndex < labelsCount; labelIndex++) {
        if ((strcmp(labelName, labels[labelIndex].name) == ZERO)) {
            return labelIndex;
        }
    }

    return MINUS_ONE;   /* Will return -1 if the word is not equal to any of the previously defined labels*/
}

/*
 *This method goes through all the chars in a string and check if they are equal to ]
 */
int checkAddressMethod3Possibility(char *operand) {
    int index = ZERO;
    while (operand[index] != '\0') {
        if (operand[index] == ']') {
            return TRUE;
        }
        index++;
    }
    return FALSE;
}

/*
 * This method removes a line drop at the end of the incoming output
 */
int removesNewLine(char *str) {
    int index = ZERO;
    while (str[index] != '\n' && str[index] != '\0') {
        index++;
    }
    str[index] = '\0';
    return ZERO;
}







