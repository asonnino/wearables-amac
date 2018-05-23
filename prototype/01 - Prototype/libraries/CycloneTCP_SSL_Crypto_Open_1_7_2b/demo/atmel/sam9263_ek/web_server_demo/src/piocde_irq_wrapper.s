   ;Dependencies
   include portmacro.inc

   export piocdeIrqWrapper
   import extIntHandler

   arm
   area  |.text|, code, readonly

;PIOCDE interrupt
piocdeIrqWrapper
   preserve8
   ;Save the current task context
   portSAVE_CONTEXT
   ;Call interrupt handler
   ldr   r0, =extIntHandler
   mov   lr, pc
   bx    r0
   ;Restore the context of the next task to execute
   portRESTORE_CONTEXT

   end
