#include "Project.h"

/*
 *The file contains the memory image of the machine code in two parts: the image of the first instruction followed by the data image
 *The first row contains a number that represents the total light of the instruction image while the second number represents the length of the data image
 *
 */
void createObjectFile(FILE *objectFile, int DCF, int ICF, int RAM[RAM_SIZE]) {

    int index = ZERO;
    int A = FOUR_BITS_ON;       /* Used to save the value of the bits between 0 - 3 */
    int B = FOUR_BITS_ON;       /* Used to save the value of the bits between 4 - 7 */
    int C = FOUR_BITS_ON;       /* Used to save the value of the bits between 8 - 11 */
    int D = FOUR_BITS_ON;       /* Used to save the value of the bits between 12 - 15 */
    int E = FOUR_BITS_ON;       /* Used to save the value of the bits between 16 - 19 */
    fseek(objectFile, ZERO, SEEK_SET); /* Returns to the beginning of objectFile*/

    /* Print the first line as described above */
    fprintf(objectFile, "\t\t\t\t%d %d\n", ICF - RAM_START, DCF);/*prints the first line with the ICF and DCF values*/

    /*
     * Will run on all memory of the RAM and print according to the given instructions
     */
    for (index = RAM_START; index < ICF + DCF; index++) {

        fprintf(objectFile, "\t%04d ", index);/*prints the address in 4 digits with leading 0 */

        A = FOUR_BITS_ON;       /* Reset the value after changing in the last loop */
        B = FOUR_BITS_ON;       /* Reset the value after changing in the last loop */
        C = FOUR_BITS_ON;       /* Reset the value after changing in the last loop */
        D = FOUR_BITS_ON;       /* Reset the value after changing in the last loop */
        E = FOUR_BITS_ON;       /* Reset the value after changing in the last loop */

        E &= RAM[index];      /* Gets the value of the corresponding bits*/
        D = D << FOUR_BITS;     /* Moves the number to it's right place */
        D &= RAM[index];      /* Gets the value of the corresponding bits*/
        D = D >> FOUR_BITS;     /* Moves the number back to get the value */
        C = C << EIGHT_BITS;    /* Moves the number to it's right place */
        C &= RAM[index];      /* Gets the value of the corresponding bits*/
        C = C >> EIGHT_BITS;    /* Moves the number back to get the value */
        B = B << TWELVE_BITS;   /* Moves the number to it's right place */
        B &= RAM[index];      /* Gets the value of the corresponding bits*/
        B = B >> TWELVE_BITS;   /* Moves the number back to get the value */
        A = A << SIXTEEN_BITS;  /* Moves the number to it's right place */
        A &= RAM[index];      /* Gets the value of the corresponding bits*/
        A = A >> SIXTEEN_BITS;  /* Moves the number back to get the value */
        fprintf(objectFile, "A%x-B%x-C%x-D%x-E%x\n  ", A, B, C, D, E);/* Prints the first line with the name and address to the file*/

    }

}
