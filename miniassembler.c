/*--------------------------------------------------------------------*/
/* miniassembler.c                                                    */
/* Author: Moses Yang & Nicholas Nguyen                               */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"
#include <assert.h>
#include <stddef.h>

/*--------------------------------------------------------------------*/
/* Modify *puiDest in place,
   setting uiNumBits starting at uiDestStartBit (where 0 indicates
   the least significant bit) with bits taken from uiSrc,
   starting at uiSrcStartBit.
   uiSrcStartBit indicates the rightmost bit in the field.
   setField sets the appropriate bits in *puiDest to 1.
   setField never unsets any bits in *puiDest.                        */
static void setField(unsigned int uiSrc, unsigned int uiSrcStartBit,
                     unsigned int *puiDest, unsigned int uiDestStartBit,
                     unsigned int uiNumBits)
{
   unsigned int i;
   i = 0;
   
   while (i < uiNumBits) {

      /* create a bitmask for the current bit in uiSrc */
      unsigned int currentBit;
      currentBit = 1 << (uiSrcStartBit + i);

      /* if the i'th bit in uiSrc  is set, transfer to *puiDest */
      if ((uiSrc & currentBit) != 0) {
         *puiDest |= 1 << (uiDestStartBit + i);
      }
      
      i++;
   }
   
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_mov(unsigned int uiReg, int iImmed)
{
   /* Base instruction code for mov */
   unsigned int uiInstr;
   uiInstr = 0x52800000;

   /* Destination register */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* Immediate */
   setField(iImmed, 0, &uiInstr, 5, 16);

   return uiInstr;

}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_adr(unsigned int uiReg, unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr;
   unsigned int uiDisp;

   /* Base Instruction Code */
   uiInstr = 0x10000000;

   /* register to be inserted in instruction */
   setField(uiReg, 0, &uiInstr, 0, 5);

   /* displacement to be split into immlo and immhi and inserted */
   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   setField(uiDisp, 0, &uiInstr, 29, 2);
   setField(uiDisp, 2, &uiInstr, 5, 19);

   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_strb(unsigned int uiFromReg,
   unsigned int uiToReg)
{
   /* Base instructions code for strb */
   unsigned int uiInstr;
   uiInstr = 0x39000000;

   /* Source register */
   setField(uiFromReg, 0, &uiInstr, 0, 5);

   /* Destination register */
   setField(uiToReg, 0, &uiInstr, 5, 5);  

   return uiInstr;
}

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_b(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned long ulDisp;
   /* Base instruction code for b */
   unsigned int uiInstr;
   uiInstr = 0x14000000;

   /* Calculate displacement */
   ulDisp = (ulAddr - ulAddrOfThisInstr) >> 2;

   setField((unsigned int) ulDisp, 0, &uiInstr, 0, 26);

   return uiInstr;
  
}
