#include "Project.h"

/*
 *
 * In this file we will do the firstPass on the text
 *
 */

int firstPass(FILE *newfp, int RAM[RAM_SIZE], label **labels, int *labelCount, command commands[],
              char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE], int *DCF, int *extFlag, int *entryFlag) {
    char line[MAX_LINE_SIZE];            /* Will contain the current line*/
    int wordAmount = ZERO;            /* Amount of words in the line*/
    char *wordArray[MAX_LINE_SIZE];        /* Will contain an array of the line divided into words*/
    int labelFlag = ZERO;                   /* Turned on if we came across a label */
    label *tempLabels;                      /* Will be used to make sure a dynamically allocated labels array is not NULL*/
    int IC = RAM_START;               /* Will be used for RAM memory allocation*/
    char intLine[MAX_LINE_SIZE];           /* The temp line of number in separated by "," */
    int intArray[MAX_LINE_SIZE] = {ZERO};     /* Will be used to save the numbers in array */
    int numInLineCounter = ZERO;            /* Will be used to count how many numbers we have in the line */
    int numOfCommas = ZERO;                 /* Will be used to count how many commas are in the line to check if the line is balanced */
    int dataLineIndex = ZERO;              /* Will be used to run on the numbers in int data entering */
    int distance = ZERO;                    /* Will be used to track the size of the label in the memory*/
    char strLabelWithoutColon[MAX_LINE_SIZE];/* Will be used to remove the colon at the end of label name*/
    int labelStatus = ZERO;                        /* Will be used to hold the number returned from the method checkLabelName */
    int commandIndex = ZERO;                /* Will be used to hold the command number */
    int lengthOfCommand = ZERO;             /* Will be used to hold the variables of the command */
    char commandOperands[MAX_LINE_SIZE][MAX_LINE_SIZE];       /* Will be used to hold the command number */
    int operandsCounter = ZERO;               /* Will be used to hold the number of operands in a line  */
    int firstOperandAddressMethod = ZERO;      /* Will be used to hold the number of the first address method in a line */
    int secondOperandAddressMethod = ZERO;      /* Will be used to hold the number of the second address method in a line */
    int lineCounter = ZERO;        /* Will be used to index the lines*/
    int errorFlag = FALSE;          /* Will be used to Mark if we encountered an error*/
    int DC = ZERO;                  /* Indicates the progress in the data set */
    int DCTable[RAM_SIZE] = {ZERO}; /* Will be used to enter the data*/
    int ICF;                        /* final IC */
    int index = ZERO;       /* Running index for the loop */


    fseek(newfp, ZERO, SEEK_SET); /* Returns to the beginning of newfp*/

    while (fgets(line, MAX_LINE_SIZE, newfp))    /* Reads a line from the file */
    {
        wordAmount = makeItArray(line, wordArray);    /* Divides the line into array containing each word*/
        lineCounter++;
        if (wordArray[ZERO][ZERO] == ';' || strcmp(wordArray[ZERO], "\n") == ZERO ||
            strcmp(wordArray[ZERO], "\t") == ZERO)    /* If it's an empty or a comment line */
        {
            /* Ignores the line */
        } else {
            labelFlag = ZERO;
            if ((wordArray[ZERO][strlen(wordArray[ZERO]) - ONE]) ==
                ':')  /* The last char of the first word is ':' means is a label*/
            {

                labelWithoutColon(wordArray[ZERO], strLabelWithoutColon);
                /* check if we already had that label */
                if (checkLabelName(strLabelWithoutColon, *labels, *labelCount) == MINUS_TWO) {
                    printf("Line %d- Error - error in label name\n", lineCounter);
                    errorFlag = TRUE;
                    continue;   /* Will continue to the next iteration */
                } else {
                    labelFlag = TRUE;/*means we are in a label*/
                    distance = ZERO;/*sets distance to zero*/
                }
            }
            /* Check if it's a data command */
            /*
             * ZERO + labelFlag  can be 0 or 1*/
            if ((strcmp(wordArray[ZERO + labelFlag], ".data") == ZERO ||
                 strcmp(wordArray[ZERO + labelFlag], ".string") == ZERO)) /* If the second word is .data or .string */
            {
                if (labelFlag == TRUE) {
                    if (*labelCount == ZERO)    /* If this is the first label encountere d*/
                    {
                        (*labels) = (label *) malloc(sizeof(label));
                        if ((*labels) == NULL) {
                            puts("Error - wasn't able to dynamically allocate a labels array, will stop the program from running \n");
                            exit(EXIT);
                        }
                        (*labelCount)++; /* Raise the label count*/
                    } else {
                        (*labelCount)++; /* Raise the label count*/
                        tempLabels = (label *) realloc((*labels), (*labelCount) * sizeof(label));
                        if (tempLabels == NULL) {
                            puts("Wasn't able to dynamically allocate a labels array, will stop the program from running \n");
                            exit(EXIT);
                        }
                        (*labels) = tempLabels;/* If realloc worked */

                    }

                    /* Enter the data */
                    removesNewLine(strLabelWithoutColon);
                    strcpy((*labels)[*labelCount - STEP].name, strLabelWithoutColon);
                    (*labels)[(*labelCount) - STEP].attributesCount = ZERO;
                    (*labels)[(*labelCount) - STEP].address = DC;
                    (*labels)[(*labelCount) -
                              STEP].attributesCount++;        /* Num of attributes plus one because we add an attribute */

                    if (strcmp(wordArray[START], ".data") == ZERO) {
                        strcpy((*labels)[*labelCount - STEP].attributes[(*labels)[*labelCount - STEP].attributesCount -
                                                                        STEP], "data");
                        /* Concat the numbers in to line seperated by commas */
                        makeIntLine(wordArray, wordAmount, intLine, labelFlag);

                        /* Count the number of commas */
                        numOfCommas = countCommas(intLine);

                        /* Make the line of numbers to an array. If there are problem with ine of the data we will get -2 */
                        numInLineCounter = makeItNumArray(intArray, intLine);
                        if (numInLineCounter < ZERO) {
                            printf("Line %d- Error - Wrong data was entered \n", lineCounter);
                            errorFlag = TRUE;
                            continue;/* Will continue to the next iteration*/
                        }

                        /*
                         * Check if the line is balanced in numbers and commas
                         */
                        if (numOfCommas + ONE != numInLineCounter) {
                            printf("Line %d- Error - Numbers and commas are not balanced\n", lineCounter);
                            errorFlag = TRUE;
                            continue; /* Will continue to the next iteration*/
                        }
                        for (dataLineIndex = ZERO; dataLineIndex < numInLineCounter; dataLineIndex++) {

                            DCTable[DC] = intArray[dataLineIndex];
                            DCTable[DC] |= (ONE << INT_A);
                            DCTable[DC] &= TURN_OFF_BITS_FOR_NEGATIVE_NUMBERS;
                            DC++;
                            distance++;
                        }
                    } else if (strcmp(wordArray[START], ".string") == ZERO) {
                        strcpy((*labels)[(*labelCount) - STEP].attributes[
                                       (*labels)[*labelCount - STEP].attributesCount - STEP], "data");

                        if (wordAmount != THREE) {
                            printf("Line %d- Error - To many arguments in the data line\n", lineCounter);
                            errorFlag = TRUE;
                            continue;   /* Will continue to the next iteration*/
                        } else {
                            /*
                             * enter each letter as a word in the RAM until
                             * 1.  enter each letter ascii value to line in RAM
                             * 2. RAMIndex++
                             * 3.label.length++
                             * 4. turn on 19 switch on (R is on)
                             */
                            for (dataLineIndex = ZERO; wordArray[TWO][dataLineIndex] != '\0'; dataLineIndex++) {
                                if (wordArray[TWO][dataLineIndex] != '"' && wordArray[TWO][dataLineIndex] != '\n') {
                                    DCTable[DC] = (int) wordArray[TWO][dataLineIndex];
                                    DCTable[DC] |= (ONE << INT_A);
                                    DC++;
                                    distance++;
                                }

                                /*
                                 * 1. Enter each letter ascii value to line in RAM
                                 * 2. RAMIndex++
                                 * 3. Label.length++
                                 * 4. turn on 19 switch on (R is on)
                                 */
                            }
                            DCTable[DC] = ZERO;
                            DCTable[DC] |= (ONE << INT_A);
                            DC++;
                            distance++;
                        }
                    }
                    (*labels)[(*labelCount) - STEP].length = distance;
                }
                    /*
                     * Data but label flag is off
                     */
                else {
                    /* Concat the numbers in to line seperated by commas */

                    makeIntLine(wordArray, wordAmount, intLine, labelFlag);

                    /* Count the number of commas */
                    numOfCommas = countCommas(intLine);

                    /* Make the line of numbers to an array. If there are problem with ine of the data we will get -2 */
                    numInLineCounter = makeItNumArray(intArray, intLine);
                    if (numInLineCounter < ZERO) {
                        printf("Line %d- Error - Wrong data was entered \n", lineCounter);
                        errorFlag = TRUE;
                        continue;   /* Will continue to the next iteration*/
                    }

                    /*
                     * Check if the line is balanced in numbers and commas
                     */
                    if (numOfCommas + ONE != numInLineCounter && numOfCommas != ZERO) {
                        printf("Line %d- Error - Numbers and commas are not balanced\n", lineCounter);
                        errorFlag = TRUE;
                        continue; /* Will continue to the next iteration*/
                    }

                    for (dataLineIndex = ZERO; dataLineIndex < numInLineCounter; dataLineIndex++) {
                        DCTable[DC] = intArray[dataLineIndex];
                        DCTable[DC] |= (ONE << INT_A);
                        DCTable[DC] &= TURN_OFF_BITS_FOR_NEGATIVE_NUMBERS;
                        DC++;
                        distance++;
                    }

                }
            }
                /*
                 * Check if we encountered extern or entry type lines
                 */
            else if ((strcmp(wordArray[ZERO + labelFlag], ".extern") == ZERO) ||
                     (strcmp(wordArray[ZERO + labelFlag], ".entry")) == ZERO) {
                labelWithoutColon(wordArray[START + labelFlag], strLabelWithoutColon);
                labelStatus = checkLabelName(strLabelWithoutColon, (*labels), *labelCount);
                if ((strcmp(wordArray[ZERO + labelFlag], ".entry")) == ZERO) {
                    *entryFlag = TRUE;
                    continue;   /* Will continue to the next iteration */
                } else if (labelStatus == MINUS_TWO) {
                    printf("Line %d-Error- error in label name\n", lineCounter);
                    errorFlag = TRUE;
                    continue;       /* Will continue to the next iteration*/
                } else if (labelStatus == MINUS_ONE) {
                    if (*labelCount == ZERO)    /* If this is the first label encountered*/
                    {
                        (*labels) = (label *) malloc(sizeof(label));
                        if ((*labels) == NULL) {
                            puts("Error - wasn't able to dynamically allocate a labels array, will stop the program from running \n");
                            exit(EXIT);
                        }
                        (*labelCount)++; /* Raise the label count*/
                    } else {
                        (*labelCount)++; /* Raise the label count*/
                        tempLabels = (label *) realloc((*labels), (*labelCount) * sizeof(label));
                        if (tempLabels == NULL) {
                            puts("Error- wasn't able to dynamically allocate a labels array, will stop the program from running \n");
                            exit(EXIT);
                        }
                        (*labels) = tempLabels;/* If realloc worked*/
                    }
                    removesNewLine(wordArray[START + labelFlag]);
                    strcpy((*labels)[(*labelCount) - STEP].name, wordArray[START + labelFlag]);
                    (*labels)[(*labelCount) - STEP].attributesCount = ZERO;
                    (*labels)[(*labelCount) - STEP].attributesCount++;        /* Num of attributes plus one because we add an attribute */
                    (*labels)[(*labelCount) - STEP].address = ZERO;
                    (*labels)[(*labelCount) - STEP].base = ZERO;
                    (*labels)[(*labelCount) - STEP].offset = ZERO;
                    strcpy((*labels)[(*labelCount) - STEP].attributes[(*labels)[(*labelCount) - STEP].attributesCount - STEP], "extern");
                    *extFlag = TRUE;

                } else {
                    /* If the label already exist we need to check if it's have extern attribute to */
                    if (strcmp((*labels)[labelStatus].attributes[ZERO], "extern") != ZERO &&
                        (*labels)[labelStatus].attributesCount != START) {
                        /* Means the label has other attributes beside extern attribute which should result in an error */
                        printf("Line %d- Error - the label has other attributes beside extern attribute, a extern label should only have a extern attribute, extern was not added to the label\n",
                               lineCounter);
                        errorFlag = TRUE;
                        continue;       /* Will continue to the next iteration*/
                    }
                }

            } else {
                /*
               * Line 11 page 37 - it's a command line!
               */
                if (labelFlag == TRUE) {
                    labelWithoutColon(wordArray[ZERO], strLabelWithoutColon);


                    labelStatus = checkLabelName(strLabelWithoutColon, (*labels), *labelCount);
                    if (labelStatus == MINUS_TWO) {
                        printf("Line %d- Error - error in label name\n", lineCounter);
                        errorFlag = TRUE;
                        continue;/*will continue to the next iteration*/
                    } else if (labelStatus > MINUS_ONE) {
                        printf("Line %d- Error - label was already defined, not possible to define it again\n",
                               lineCounter);
                        errorFlag = TRUE;
                        continue;       /* Will continue to the next iteration*/
                    } else {
                        if ((*labelCount) == ZERO)      /* If this is the first label encountered*/
                        {
                            (*labels) = (label *) malloc(sizeof(label));
                            if ((*labels) == NULL) {
                                puts("Error - wasn't able to dynamically allocate a labels array, will stop the program from running \n");
                                exit(EXIT);
                            }
                            (*labelCount)++; /* Raise the label count*/
                        } else {
                            (*labelCount)++; /* Raise the label count*/
                            tempLabels = (label *) realloc((*labels), (*labelCount) * sizeof(label));
                            (*labels)[(*labelCount) - ONE].length = ZERO;
                            if (tempLabels == NULL) {
                                puts("Error- wasn't able to dynamically allocate a labels array, will stop the program from running \n");
                                exit(EXIT);
                            }
                            (*labels) = tempLabels; /* If realloc worked*/
                        }
                        removesNewLine(strLabelWithoutColon);
                        strcpy((*labels)[*labelCount - ONE].name, strLabelWithoutColon);
                        (*labels)[(*labelCount) - ONE].address = IC;
                        (*labels)[(*labelCount) - ONE].attributesCount = ZERO;
                        (*labels)[(*labelCount) - ONE].attributesCount++;        /* Num of attributes plus one because we add an attribute */
                        enterBaseAndOffset(&(*labels)[*labelCount - ONE], IC);
                        strcpy((*labels)[*labelCount - ONE].attributes[(*labels)[(*labelCount) - ONE].attributesCount -
                                                                       ONE], "code");
                    }
                }
                lengthOfCommand = ZERO;
                /* Search the name of the command in the table */
                commandIndex = searchCommand(commands, wordArray[ZERO + labelFlag]);

                if (commandIndex == MINUS_ONE) {
                    printf("Line %d Error - command was not found, continuing to the next line \n", lineCounter);
                    errorFlag = TRUE;
                    continue;   /* Will continue to the next iteration*/
                } else {

                    removesNewLine(wordArray[ZERO + labelFlag]);
                    if ((strcmp(wordArray[ZERO + labelFlag], "rts")) == ZERO ||
                        (strcmp(wordArray[ZERO + labelFlag], "stop")) == ZERO) {
                        operandsCounter = ZERO;
                    } else {
                        operandsCounter = lineForCommand(wordArray, labelFlag, wordAmount, commandOperands,
                                                         lineCounter);
                        if (operandsCounter == MINUS_ONE) {
                            /* An error printing was made inside the lineForCommand function */
                            continue;   /* Will continue to the next iteration*/
                        }
                    }

                    switch (commandIndex) {
                        /* The methods that get 2 operands */
                        case ZERO:
                        case ONE:
                        case TWO:
                        case THREE:
                        case FOUR:

                            /* Check if we have enough operands for that method*/
                            if (operandsCounter != TWO) {
                                numOfOperandsNotMatch(lineCounter);
                                errorFlag = TRUE;
                                continue;   /* Will continue to the next iteration*/
                            }

                            /* Find the number of the Addressing Methods*/
                            firstOperandAddressMethod = findAddressMethod(commandOperands[ZERO], registerTable, lineCounter);
                            secondOperandAddressMethod = findAddressMethod(commandOperands[STEP], registerTable, lineCounter);

                            /* Checks whether there is a match between the method and the operand it can receive */
                            if (firstOperandAddressMethod == MINUS_ONE || secondOperandAddressMethod == MINUS_ONE) {
                                printf("Line %d- Error - Error in address operand \n", lineCounter);
                                errorFlag = TRUE;
                            }

                            if (commandIndex == FOUR &&
                                (firstOperandAddressMethod == ZERO || firstOperandAddressMethod == THREE)) {
                                printf("Line %d- lea command cant use this addressing method for source operand", lineCounter);
                                errorFlag = TRUE;
                            }

                            if (secondOperandAddressMethod == ZERO && commandIndex != ONE) {
                                printf("Line %d- this method cannot get immediate address method in the destination operand", lineCounter);
                                errorFlag = TRUE;
                            }

                            lengthOfCommand += TWO;      /* Two more lines - One for the opceode - Second for funct and  the addresing methods and source and destination registers */

                            /* Adds the number of rows according to the appropriate addressing method */
                            lengthOfCommand += addLength(firstOperandAddressMethod);    /* Will add the length needed based on the addressing method */
                            lengthOfCommand += addLength(secondOperandAddressMethod);    /* Will add the length needed based on the addressing method */

                            break;
                            /*The method gets 1 operand */
                        case FIVE:
                        case SIX:
                        case SEVEN:
                        case EIGHT:
                        case NINE:
                        case TEN:
                        case ELEVEN:
                        case TWELVE:
                        case THIRTEEN:


                            /* Check if we have enough operands for that method*/
                            if (operandsCounter != ONE) {
                                numOfOperandsNotMatch(lineCounter);
                                errorFlag = TRUE;
                                continue;   /* Will continue to the next iteration*/
                            }
                            /* Find the number of the Addressing Methods*/
                            firstOperandAddressMethod = findAddressMethod(commandOperands[ZERO], registerTable, lineCounter);
                            /* Checking for errors*/
                            if (firstOperandAddressMethod == MINUS_ONE) {
                                printf("Line %d-Error - Error in address operand \n", lineCounter);
                                errorFlag = TRUE;
                                continue;
                            }

                            /* Checks whether there is a match between the method and the operand it can receive */
                            if (commandIndex != THIRTEEN && firstOperandAddressMethod == ZERO) {
                                printf("Line %d- this method cannot get immediate address method in the destination operand", lineCounter);
                                errorFlag = TRUE;
                                continue;
                            }

                            if (firstOperandAddressMethod == THREE &&
                                (commandIndex == NINE || commandIndex == TEN || commandIndex == ELEVEN)) {
                                printf("Line %d- this method cannot get register direct address method in the destination operand", lineCounter);
                                errorFlag = TRUE;
                                continue;
                            }

                            /* Adds the number of rows according to the appropriate addressing method */
                            lengthOfCommand += TWO;      /* Two more lines - One for the opceode - Second for funct and  the addresing methods and source and destination registers */
                            lengthOfCommand += addLength(firstOperandAddressMethod);    /* Will add the length needed based on the addressing method */

                            break;

                            /* The method doesn't get any operand */
                        case FOURTEEN:
                        case FIFTEEN:

                            /* Check if we have enough operands for that method */
                            if (operandsCounter != ZERO) {
                                numOfOperandsNotMatch(lineCounter);
                                errorFlag = TRUE;
                                continue; /* Will continue to the next iteration*/
                            }

                            lengthOfCommand = ONE;
                            break;
                    }
                    /* The method will change the RAM to contain the values that are supposed to be inserted to the RAM*/
                    changeMemoryFirstCross(firstOperandAddressMethod, secondOperandAddressMethod, RAM, IC, commandIndex, commands, operandsCounter, registerTable, commandOperands);
                    /* The label length is change only if we have a label */


                    if (labelFlag == START) {
                        (*labels)[(*labelCount) - ONE].length = lengthOfCommand;
                    }
                    IC += lengthOfCommand; /* Setting RAMIndex to the next free index*/
                }
            }
        }
    }


    if (errorFlag == TRUE)   /* If there was en error in one of the lines*/
    {
        return FALSE;
    }
    *DCF = DC;
    ICF = IC;

    for (index = ZERO; index < *labelCount; index++)   /* Setting the new addresses for data labels*/
    {
        if (strcmp((*labels)[index].attributes[ZERO], "data") == ZERO) {
            enterBaseAndOffset(&(*labels)[index], ICF + (*labels)[index].address);
        }
    }

    for (index = ZERO; index < *DCF; index++)   /* Copying all data from DCTable to the end of the RAM*/
    {
        RAM[IC + index] = DCTable[index];
    }
    return ICF;
}
