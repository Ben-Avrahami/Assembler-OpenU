#include "Project.h"

/*
 *
 * In this file we will do the secondPass on the text
 *
 */

int secondPass(FILE *newfp, int RAM[RAM_SIZE], label **labels, int labelCount, command commands[],
               char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE], label **externs, int *externsCount) {
    char line[MAX_LINE_SIZE];            /* Will contain the current line*/
    int wordAmount = ZERO;            /* Amount of words in the line*/
    char *wordArray[MAX_LINE_SIZE];        /* Will contain an array of the line divided into words*/
    int lineCounter = ZERO;/*will be used to index the lines*/
    int labelFlag = ZERO;
    int entryLabelIndex = ZERO; /*will hold the index of entry label*/
    int errorFlag = FALSE;
    int commandIndex = ZERO;                /* Will be used to hold the command number */
    int firstOperandAddressMethod = ZERO;
    int secondOperandAddressMethod = ZERO;
    int RAMIndex = RAM_START;  /*will be used for RAM memory allocation*/
    char commandOperands[MAX_LINE_SIZE][MAX_LINE_SIZE];
    int labelIndex = ZERO; /*will hold the index of label*/
    char labelToken[MAX_LINE_SIZE];
    label *tempExterns;    /*will be used to make sure a dynamically allocated externs array is not NULL*/

    fseek(newfp, ZERO, SEEK_SET); /* Returns to the beginning of newfp*/
    while (fgets(line, MAX_LINE_SIZE, newfp))    /* Reads a line from the file */
    {
        labelFlag = ZERO;/*resets labelFlag to equal ZERO*/
        wordAmount = makeItArray(line, wordArray);    /* Divides the line into array containing each word*/
        lineCounter++;

        if (wordArray[ZERO][ZERO] == ';' || strcmp(wordArray[ZERO], "\n") == ZERO ||
            strcmp(wordArray[ZERO], "\t") == ZERO)    /* If it's an empty or a comment line */
        {
            /* Ignores the line */
        } else/*if it wasn't an empty line*/
        {
            if ((wordArray[ZERO][strlen(wordArray[ZERO]) - ONE]) ==
                ':')  /* The last char of the first word is ':' means is a label*/
            {
                labelFlag = ONE;
            }
            if ((strcmp(wordArray[ZERO + labelFlag], ".data")) == ZERO ||
                (strcmp(wordArray[ZERO + labelFlag], ".string")) == ZERO ||
                (strcmp(wordArray[ZERO + labelFlag], ".extern")) == ZERO) /*if the second word is .data or .string */
            {
                continue; /*will continue to the next line*/
            }
            if ((strcmp(wordArray[ZERO + labelFlag], ".entry")) == ZERO) {
                /*This function get label and return it's number in the label array*/

                entryLabelIndex = labelNum(wordArray[ONE + labelFlag], (*labels), labelCount);

                if (entryLabelIndex == MINUS_ONE) {
                    printf("Line %d- Error - Label wasn't found 1 \n", lineCounter);
                    errorFlag = TRUE; /* We encountered an error so errorFlag will be equal to TRUE*/
                    continue;/* Will continue to the next line*/
                }
                (*labels)[entryLabelIndex].attributesCount++;
                strcpy((*labels)[entryLabelIndex].attributes[(*labels)[entryLabelIndex].attributesCount - ONE],
                       "entry"); /*added entry to the label attribute*/
            }
            commandIndex = searchCommand(commands, wordArray[ZERO +
                                                             labelFlag]); /*searching if the command is in the commands table */
            lineForCommand(wordArray, labelFlag, wordAmount, commandOperands,
                                             lineCounter); /*will receive the value returned from the lineForCommand method*/
            switch (commandIndex) {
                /*The methods that get 2 operands */
                case ZERO:
                case ONE:
                case TWO:
                case THREE:
                case FOUR:

                    /* Find the number of the Addressing Methods*/
                    firstOperandAddressMethod = findAddressMethod(commandOperands[ZERO], registerTable, lineCounter);
                    secondOperandAddressMethod = findAddressMethod(commandOperands[STEP], registerTable, lineCounter);
                    RAMIndex += TWO;      /* Two more lines - One for the opcode - Second for funct and  the addresing methods and source and destination*/
                    if (firstOperandAddressMethod == ONE || firstOperandAddressMethod == TWO) {
                        if (firstOperandAddressMethod == ONE) {
                            strcpy(labelToken, commandOperands[ZERO]);
                            labelIndex = labelNum(labelToken, (*labels),labelCount);  /*This function get label and return it's number in the label array*/
                            if (labelIndex == MINUS_ONE) {
                                printf("Line %d- Error - Label wasn't found 2 \n", lineCounter);
                                errorFlag = TRUE; /* We encountered an error so errorFlag will be equal to TRUE*/
                                continue;/* Will continue to the next line*/
                            }
                        }
                        if (firstOperandAddressMethod == TWO) {
                            strcpy(labelToken, strtok(commandOperands[ZERO], "["));
                            labelIndex = labelNum(labelToken, (*labels),labelCount);  /*This function get label and return it's number in the label array*/
                            if (labelIndex == MINUS_ONE) {
                                printf("Line %d- Error - Label wasn't found 3 \n", lineCounter);
                                errorFlag = TRUE; /* We encountered an error so errorFlag will be equal to TRUE*/
                                continue;/* Will continue to the next line*/
                            }
                        }
                        if ((*labels)[labelIndex].base != ZERO) {
                            RAM[RAMIndex] |= (*labels)[labelIndex].base;/*enters the base of the label to the RAM*/
                            RAM[RAMIndex] |= ONE << INT_R;
                            RAMIndex++;
                            RAM[RAMIndex] |= (*labels)[labelIndex].offset;/*enters the base of the label to the RAM*/
                            RAM[RAMIndex] |= ONE << INT_R;
                            RAMIndex++;
                        } else /*if its a extern label will enter this else*/
                        {
                            if ((*externsCount) == ZERO)/*if this is the first label encountered*/
                            {
                                (*externs) = (label *) malloc(sizeof(label));
                                if ((*externs) == NULL) {
                                    puts("Error - wasn't able to dynamically allocate a externs array, will stop the program from running \n");
                                    exit(EXIT);
                                }
                                (*externsCount)++; /* Raise the label count*/
                            } else {
                                (*externsCount)++; /* Raise the label count*/
                                tempExterns = (label *) realloc((*externs), (*externsCount) * sizeof(label));
                                if (tempExterns == NULL) {
                                    puts("Wasn't able to dynamically allocate a externs array, will stop the program from running \n");
                                    exit(EXIT);
                                }
                                (*externs) = tempExterns;/* If realloc worked*/
                            }
                            strcpy((*externs)[(*externsCount) - ONE].name, labelToken);
                            (*externs)[(*externsCount) - ONE].base = RAMIndex;
                            RAM[RAMIndex] |= ONE << INT_E;
                            RAMIndex++;
                            RAM[RAMIndex] |= ONE << INT_E;
                            RAMIndex++;
                        }
                    } else if (firstOperandAddressMethod == ZERO) {
                        RAMIndex++;
                    }

                    if (secondOperandAddressMethod == ONE || secondOperandAddressMethod == TWO) {
                        if (secondOperandAddressMethod == ONE) {
                            strcpy(labelToken, commandOperands[ONE]);
                            labelIndex = labelNum(labelToken, (*labels),labelCount);  /*This function get label and return it's number in the label array*/
                            if (labelIndex == MINUS_ONE) {
                                printf("Line %d- Error - Label wasn't found 4 \n", lineCounter);
                                errorFlag = TRUE; /* We encountered an error so errorFlag will be equal to TRUE*/
                                continue;/* Will continue to the next line*/
                            }
                        }
                        if (secondOperandAddressMethod == TWO) {
                            strcpy(labelToken, strtok(commandOperands[ONE], "["));
                            labelIndex = labelNum(labelToken, (*labels),labelCount);  /*This function get label and return it's number in the label array*/
                            if (labelIndex == MINUS_ONE) {
                                printf("Line %d- Error - Label wasn't found 5 \n", lineCounter);
                                errorFlag = TRUE; /* We encountered an error so errorFlag will be equal to TRUE*/
                                continue;/* Will continue to the next line*/
                            }
                        }
                        if ((*labels)[labelIndex].base != ZERO) {
                            RAM[RAMIndex] |= (*labels)[labelIndex].base;/*enters the base of the label to the RAM*/
                            RAM[RAMIndex] |= ONE << INT_R;
                            RAMIndex++;
                            RAM[RAMIndex] |= (*labels)[labelIndex].offset;/*enters the base of the label to the RAM*/
                            RAM[RAMIndex] |= ONE << INT_R;
                            RAMIndex++;
                        } else /*if its a extern label will enter this else*/
                        {
                            if (*externsCount == ZERO)/*if this is the first label encountered*/
                            {
                                (*externs) = (label *) malloc(sizeof(label));
                                if ((*externs) == NULL) {
                                    puts("Error - wasn't able to dynamically allocate a externs array, will stop the program from running \n");
                                    exit(EXIT);
                                }
                                (*externsCount)++; /* Raise the label count*/
                            } else {
                                (*externsCount)++; /* Raise the label count*/
                                tempExterns = (label *) realloc((*externs), (*externsCount) * sizeof(label));
                                if (tempExterns == NULL) {
                                    puts("Wasn't able to dynamically allocate a externs array, will stop the program from running \n");
                                    exit(EXIT);
                                }
                                (*externs) = tempExterns;/* If realloc worked*/
                            }
                            strcpy((*externs)[(*externsCount) - ONE].name, labelToken);
                            (*externs)[(*externsCount) - ONE].base = RAMIndex;
                            RAM[RAMIndex] |= ONE << INT_E;
                            RAMIndex++;
                            RAM[RAMIndex] |= ONE << INT_E;
                            RAMIndex++;
                        }
                    } else if (secondOperandAddressMethod == ZERO) {
                        RAMIndex++;
                    }

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

                    firstOperandAddressMethod = findAddressMethod(commandOperands[ZERO], registerTable, lineCounter);
                    RAMIndex += TWO;       /* Two more lines - One for the opcode - Second for funct and  the addresing methods and source and destination*/
                    if (firstOperandAddressMethod == ONE || firstOperandAddressMethod == TWO) {
                        if (firstOperandAddressMethod == ONE) {
                            strcpy(labelToken, commandOperands[ZERO]);
                            labelIndex = labelNum(commandOperands[ZERO], (*labels),labelCount);  /*This function get label and return it's number in the label array*/
                            if (labelIndex == MINUS_ONE) {
                                printf("Line %d- Error - Label wasn't found 6 \n", lineCounter);
                                errorFlag = TRUE; /* We encountered an error so errorFlag will be equal to TRUE*/
                                continue;/* Will continue to the next line*/
                            }
                        }
                        if (firstOperandAddressMethod == TWO) {
                            strcpy(labelToken, strtok(commandOperands[ZERO], "["));
                            labelIndex = labelNum(labelToken, (*labels),labelCount);  /*This function get label and return it's number in the label array*/
                            if (labelIndex == MINUS_ONE) {
                                printf("Line %d- Error - Label wasn't found 7 \n", lineCounter);
                                errorFlag = TRUE; /* We encountered an error so errorFlag will be equal to TRUE*/
                                continue;/* Will continue to the next line*/
                            }
                        }
                        if ((*labels)[labelIndex].base != ZERO) {
                            RAM[RAMIndex] |= (*labels)[labelIndex].base;/*enters the base of the label to the RAM*/
                            RAM[RAMIndex] |= ONE << INT_R;
                            RAMIndex++;
                            RAM[RAMIndex] |= (*labels)[labelIndex].offset;/*enters the base of the label to the RAM*/
                            RAM[RAMIndex] |= ONE << INT_R;
                            RAMIndex++;
                        } else /*if it's an extern label will enter this else*/
                        {
                            if ((*externsCount) == ZERO)/*if this is the first label encountered*/
                            {
                                (*externs) = (label *) malloc(sizeof(label));
                                if ((*externs) == NULL) {
                                    puts("Error - wasn't able to dynamically allocate a externs array, will stop the program from running \n");
                                    exit(EXIT);
                                }
                                (*externsCount)++; /* Raise the label count*/
                            } else {
                                (*externsCount)++; /* Raise the label count*/
                                tempExterns = (label *) realloc((*externs), (*externsCount) * sizeof(label));
                                if (tempExterns == NULL) {
                                    puts("Wasn't able to dynamically allocate a externs array, will stop the program from running \n");
                                    exit(EXIT);
                                }
                                (*externs) = tempExterns;/* If realloc worked*/
                            }
                            strcpy((*externs)[(*externsCount) - ONE].name, labelToken);
                            (*externs)[(*externsCount) - ONE].base = RAMIndex;
                            RAM[RAMIndex] |= ONE << INT_E;
                            RAMIndex++;
                            RAM[RAMIndex] |= ONE << INT_E;
                            RAMIndex++;
                        }
                    }
                    if (firstOperandAddressMethod == ZERO) {
                        RAMIndex++;
                    }

                    break;

                    /*The method doesn't get any operand */
                case FOURTEEN:
                case FIFTEEN:

                    RAMIndex++;
                    break;
            }
        }
    }

    if (errorFlag == TRUE) {
        return MINUS_ONE;
    }
    return ONE;
}

