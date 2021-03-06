//Dependencies
#include "isr_support.h"

   .global ethIrqWrapper
   .extern pic32mzEthIrqHandler
   .extern xISRStackTop

   .set nomips16
   .set noreorder
   .set noat

   .ent ethIrqWrapper

//Ethernet interrupt
ethIrqWrapper:
   //Save the current task context
   portSAVE_CONTEXT
   //Call interrupt handler
   jal pic32mzEthIrqHandler
   nop
   //Restore the context of the next task to execute
   portRESTORE_CONTEXT

   .end ethIrqWrapper
