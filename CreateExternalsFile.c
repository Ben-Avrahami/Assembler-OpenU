#include "Project.h"

/*
 * Create an output file for each label in the label table that characterized as an extern
 * Each row contains the name of the external directory followed by the word BASE followed by the address of the information word in the machine code in which the base address is required
 * Then, another line containing the name of the external symbol followed by the word OFFSET followed by the address of the information word in the machine code in which the shift is required

 */
void createExternFile(FILE *externFile, label *externs, int externsCount) {

    int index = ZERO;

    fseek(externFile, ZERO, SEEK_SET); /* Returns to the beginning of externFile*/

    /*
     * Go through the places where externals are used and enter the name, the base and the offset
     */
    for (index = ZERO; index < externsCount; index++) {
        fprintf(externFile, "%s BASE %04d\n", externs[index].name, externs[index].base);/*prints the first line with the name and address to the file*/
        fprintf(externFile, "%s OFFSET %04d\n", externs[index].name, externs[index].base + ONE);/*prints the first line with the name and offset to the file*/
    }
}
