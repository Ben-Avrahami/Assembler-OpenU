#include "Project.h"

/*
 * Create an output file for each label in the label table that characterized as an entry
 * The name of the icon, the base address and the offset.
 */
void createEntryFile(FILE *entryFile, label **labels, int labelsCount) {

    int index = ZERO;
    int insideIndex = ZERO;

    /*
     * Move to start the file
     */
    fseek(entryFile, ZERO, SEEK_SET); /* Returns to the beginning of externFile*/

    /*
     * Run through the entire table of label and look for the entry type
     */
    for (index = ZERO; index < labelsCount; index++) {
        for (insideIndex = ZERO; insideIndex < (*labels)[index].attributesCount; insideIndex++) {
            /* Print according to the instructions if we found */
            if ((strcmp((*labels)[index].attributes[insideIndex], "entry")) == ZERO) {
                fprintf(entryFile, "%s,%d,%d\n", (*labels)[index].name, (*labels)[index].base, (*labels)[index].offset);/*prints the first line with the name and address to the file*/
            }
        }
    }
}
