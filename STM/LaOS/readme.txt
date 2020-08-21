This is a simple switching mechanism for Cortex-M microcontrollers.
Main features :
 -  Stack overflow protection. Supervisor check state of stack
    of all threads at each cycle.
 -  Utilization of separate stack pointers.
 -  SVC handler with codes recognition - 
    user can add its codes to process in function 'SVC_Handler_C' 
    in sysCall.cpp, or redefine it completely.
 -  MPU support - threads are running in unprivileged mode.
    .text section is accessible in read only mode,
    threads stack is removed to separate section .unprivileged_stack -
    look '...FLASH.ld' file.

Drawback :
 -  If you want to use it, you should think
 -  Cuse most of the modern MCU market is consists of solutions 
    based on Cortex-Mx cores, no other ports/MCU support is provided.
    And I have no plans to support computer history.
 -  In the present state, no 'cmake' or 'make' or other building tools
    are supported. The reason is, that most of the Cortex-Mx solutions, 
    developed around some kind of eclipse ports - codegenerators.
    No sane system is supported by the MCU providers.

Specifics :
 -  Technically you can't use 'stdlib' calls, cause
    its functions internally set global variable 'errno'
    on behalf of an un-privileged caller. 


