#include "Project.h"

/*
 *
 * In this file we will deploy the macros
 *
 */
int macrosDeploy(FILE *fp, FILE *newfp) {

    char line[MAX_LINE_SIZE];            /* Will contain the current line*/
    int flagMacro = ZERO;                /* Represents a number indicating whether we have encountered a macro in the code so far */
    macro *macros;                        /* A pointer to an array of macros */
    int macrosCount = ZERO;            /* Number of macros encountered */
    macro *tempMacros;                    /* Will be used to make sure the realloc didn't return a NULL */
    int wordAmount = ZERO;            /* Amount of words in the line*/
    char *wordArray[MAX_LINE_SIZE];        /* Will contain an array of the line divided into words*/
    char copyLine[MAX_LINE_SIZE];        /* Will be used to save the line before making changes to it*/
    int macrosRunIndex = ZERO;        /* Will be used to index macros*/
    int endmFlag = ZERO;                /* Indicates whether we encountered an end of macro in the line */
    int currentIndex = ZERO;            /* Indicates an index until we reach end of line */
    char newWord[MAX_LINE_SIZE];        /* The word after removing spaces*/

    /*
     1.  Opens a file
     2.  Gets his Macros
     3.  Copies the content to a new file and changes the macro contents
     */

    while (fgets(line, MAX_LINE_SIZE, fp))    /* Reads a line from the file */
    {
        strcpy(copyLine, line);                          /* Save the line in to a var */
        /*
         Separate the line into words separated by spaces and insert them into the array
        */
        makeItArray(line, wordArray);
        /*
        If the first word is macro then it's a start of macro
         */

        if (strcmp(wordArray[ZERO], "macro") == ZERO) {
            /* The endm flag should be 0 after we have found a macro */
            endmFlag = ZERO;
            macrosCount++;
            /* If this is the first macro observed in the texts*/
            if (flagMacro == ZERO) {
                flagMacro = START;
                macros = (macro *) malloc(sizeof(macro));       /* Dynamically allocates an array of macros array */
                if (macros == NULL) {
                    puts("wasn't able to dynamically allocate en array, will stop the program from running");
                    return FALSE;
                }

                /*
                Inserting the macro's name into struct
                 */

                strcpy((macros[macrosCount - STEP].name), wordArray[START]);

                /*
                We will add lines until we reached the end of the macro
                 */
                while (endmFlag != START) {
                    fgets(line, MAX_LINE_SIZE, fp);       /* Reads a line from the file */

                    /*
                    Split the line into words in array
                     */
                    wordAmount = makeItArray(line, wordArray);
                    currentIndex = ZERO;
                    removeNewLine(wordArray[currentIndex],newWord);/*  Removes the '\n' char at the end of the word if there is one*/

                    /*
                    Run on the words in a row until you encounter the end of a macro or until we reach the end of the line
                    */

                    while ((strcmp(newWord, "endm")) != ZERO && currentIndex < wordAmount) {
                        strcat((macros[macrosCount - STEP].content), wordArray[currentIndex]);
                        currentIndex++;
                        strcat((macros[macrosCount - STEP].content), " ");

                    }

                    /*
                    Check if we have reached the endm which means we have reached the end of the macro
                     and from now on it will not be the first macro
                     */

                    if ((strcmp(newWord, "endm")) == ZERO) {
                        endmFlag = START;
                    }
                }

            }
                /*
                 * If this is not the first macro we encountered
                */
            else {
                tempMacros = (macro *) realloc(macros, macrosCount * sizeof(macro));
                if (tempMacros != NULL) {
                    /*
                    Inserting the macro's name into struct
                    */
                    macros = tempMacros;
                    strcpy((macros[macrosCount - STEP].name), wordArray[START]);
                } else {
                    puts("wasnt able to dynamically allocate en array, will stop the program from running");
                    return FALSE;
                }

                while (endmFlag != START) {
                    fgets(line, MAX_LINE_SIZE, fp);       /* Reads a line from the file */

                    /*
                    Split the line in to words in array
                     */
                    wordAmount = makeItArray(line, wordArray);

                    currentIndex = ZERO;
                    removeNewLine(wordArray[currentIndex], newWord);

                    /*
                    Run on the words in a row until encounter the end of a macro or until we reach the end of the line
                     */
                    while (strcmp(newWord, "endm") != ZERO && currentIndex < wordAmount) {
                        strcat((macros[macrosCount - STEP].content), wordArray[currentIndex]);
                        currentIndex++;
                        strcat((macros[macrosCount - STEP].content), " ");
                    }

                    /*
                    If we come across a macro end we will raise the flag that says we have reached the end macro
                     and from now on it will not be the first macro
                     */
                    if (strcmp(newWord, "endm") == ZERO) {
                        endmFlag = START;
                        flagMacro = START;

                    }
                }
            }

        }
            /*
            If this is not a macro definition we will check if it is macro use
           */
        else {
            for (macrosRunIndex = ZERO; macrosRunIndex < macrosCount && macrosRunIndex >= ZERO; macrosRunIndex++)
                /* Will enter the loop and be chacked if it is a macro, if it is will copy its content to the new file */
            {
                if (strcmp(macros[macrosRunIndex].name, wordArray[ZERO]) == ZERO) {
                    fputs(macros[macrosRunIndex].content, newfp);  /* Copy the macro contents to the new file */
                    macrosRunIndex = MINUS_TWO;     /* Changes the macrosRunIndex to indicate that we have reached the macro */
                }

            }
            /*
             * If it's not a macro name then we will print the line to the file
             */
            if (macrosRunIndex >= ZERO)        /* Will enter if it is not a macro*/
            {
                fputs(copyLine, newfp); /* Will copy the line to the new file*/
            }
        }

    }
    free(macros);
    return TRUE;
}

