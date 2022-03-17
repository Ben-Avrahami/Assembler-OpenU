#include "Project.h"

int main(int argc, char *argv[]) {

    FILE *fp;                    /* Will point to the given file */
    FILE *amFp;                /* Will point to a new file that will contain the program after placing macros*/
    FILE *extFp;                /* Will point to a new file that will contain the externals output*/
    FILE *obfile;                /* Will point to a new file that will contain the object output*/
    FILE *entFp;                /* Will point to a new file that will contain the entry output*/
    int fileIndex = START;            /* Will be used for the index of the current file*/
    char originalFileName[MAX_LINE_SIZE];    /* Will contain the file name without .as*/
    char FileNameWithAm[MAX_LINE_SIZE];    /* Will contain the file name without .as with .am*/
    char FileNameWithExt[MAX_LINE_SIZE];    /* Will contain the file name without .as with .ext*/
    char FileNameWithObj[MAX_LINE_SIZE];    /* Will contain the file name without .as with .obj*/
    char FileNameWithEnt[MAX_LINE_SIZE];    /* Will contain the file name without .as with .ent*/
    char openingFileName[MAX_LINE_SIZE];    /* Will contain the file name with .as*/
    int RAM[RAM_SIZE] = {ZERO};              /* The RAM memory */
    int labelCount = ZERO;                  /* Will be used to count how many labels encountered */
    label *labels;                          /*Will be used as a pointer to dynamically allocated array*/
    int ICF = ZERO;           /*Will be used to signal were the RAM memory use was ended*/
    int firstPassStatus = ZERO;     /*Will contain an int representing the status of the first pass*/
    label *externs;        /*Will be used as a pointer to dynamically allocated array*/
    int externsCount = ZERO;                  /* W      ill be used to count how many extern labels encountered */
    int DCF = ZERO;                             /*Will hold the value of the final DC (DCF) from FirstPass */
    int extFlag = ZERO;                     /*Will be used as an extern flag*/
    int entryFlag = ZERO;                   /*Will be used as an entry flag*/


    command commands[] =                /* An array of structs representing an action and its numerical data */
            {
                    {"mov",  0,  0},
                    {"cmp",  1,  0},
                    {"add",  2,  10},
                    {"sub",  2,  11},
                    {"lea",  4,  0},
                    {"clr",  5,  10},
                    {"not",  5,  11},
                    {"inc",  5,  12},
                    {"dec",  5,  13},
                    {"jmp",  9,  10},
                    {"bne",  9,  11},
                    {"jsr",  9,  12},
                    {"red",  12, 0},
                    {"prn",  13, 0},
                    {"rts",  14, 0},
                    {"stop", 15, 0}
            };
/*
 * An array of registers names
 */
    char registerTable[MAX_LINE_SIZE][MAX_LINE_SIZE] =
            {
                    "r0",
                    "r1",
                    "r2",
                    "r3",
                    "r4",
                    "r5",
                    "r6",
                    "r7",
                    "r8",
                    "r9",
                    "r10",
                    "r11",
                    "r12",
                    "r13",
                    "r14",
                    "r15",
            };


    if (argc < MIN_ARGUMENTS) /* If not enough arguments were given, will print an error and exit the program*/
    {
        puts("not enough arguments were given");
        return EXIT;
    }
    /*
     1.  Opens a file
     2.  Gets his Macros
     3.  Copies the content to a new file and changes the macro contents
     */
    for (fileIndex = LOOP_START; fileIndex < argc; fileIndex++) {         /* Starts the loop */
        externsCount = ZERO;/*Will reset externs count*/
        labelCount = ZERO;/*Will reset label count*/
        strcpy(originalFileName, argv[fileIndex]); /* Will copy the name of the current file */
        strcpy(openingFileName, originalFileName); /* Will copy the name of the current file */
        strcpy(FileNameWithAm, originalFileName); /* Will copy the name of the current file */
        strcpy(FileNameWithExt, originalFileName); /* Will copy the name of the current file */
        strcpy(FileNameWithObj, originalFileName); /* Will copy the name of the current file */
        strcpy(FileNameWithEnt, originalFileName); /* Will copy the name of the current file */
        strcat(openingFileName, ".as");
        fp = fopen(openingFileName, "r+");            /* Opens the file */
        amFp = fopen(strcat(FileNameWithAm, ".am"),
                     "w+");                  /* Creates and opens a new file with the name of the old file + new */

        if (fp == NULL) {                                                 /* If it wasn't able to open the file */
            puts("wasn't able to open the current file, continue to the next file");
            continue;
        }
            /*
             The file opens successfully
             */

        else {
            if ((macrosDeploy(fp, amFp)) == FALSE)      /* Will deploy all macros to newfp*/
            {
                exit(EXIT);      /* Program should exit, wasn't able to deploy macros*/
            }

            fclose(fp);  /* Closes the original file */

            /*
            * Macros have been deployed to the new file newfp!!!!
            */
            firstPassStatus = firstPass(amFp, RAM, &labels, &labelCount, commands, registerTable, &DCF, &extFlag,&entryFlag);
            /* Finished first pass over the code*/
            if (firstPassStatus == FALSE) {
                printf("Error was found in the first reading of the code, therefor will not continue to the second pass \n");
                continue;   /* Will continue to the next file*/
            }

            ICF = firstPassStatus; /* Will save the index where the RAM usage was ended*/

            if (secondPass(amFp, RAM, &labels, labelCount, commands, registerTable, &externs, &externsCount) == MINUS_ONE)/*will do the second pass*/
            {
                continue;/* Will continue to the next file*/
            }

            /*second pass completed*/

            if (extFlag == TRUE) {
                extFp = fopen(strcat(FileNameWithExt, ".ext"),"w+");            /* Creates and opens a new file for the output extern file */
                createExternFile(extFp, externs, externsCount); /* Creating the externals output file*/
            }

            obfile = fopen(strcat(FileNameWithObj, ".ob"),"w+");            /* Creates and opens a new file for the output object file */
            createObjectFile(obfile, DCF, ICF, RAM);

            if (entryFlag == TRUE) {
                entFp = fopen(strcat(FileNameWithEnt, ".ent"),"w+");            /* Creates and opens a new file for the output entry file */
                createEntryFile(entFp, &labels, labelCount); /*creating the externals output file */
            }       /* Creating the object output file */

        }
        free(labels);
        free(externs);
        fclose(amFp); /* Closes the second file */
        fclose(extFp); /* Closes the extern file */
        fclose(obfile); /* Closes the object file */
        fclose(entFp); /* Closes the entry file */
    }
    return ZERO;
}





