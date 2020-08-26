This is a simple switching mechanism for Cortex-M microcontrollers.

Main features :
 -  It is not universal - it is using almost all Cortex-M features.
 -  Stack overflow protection. Supervisor check state of the stack
    of all threads at each cycle. MPU protects from other errors.
 -  Utilization of separate stack pointers.
 -  SVC handler with codes recognition - 
    user can add its codes to process in function 'SVC_Handler_C' 
    in sysCall.cpp, or redefine it completely.
 -  MPU support - threads are running in unprivileged mode.
    '.text' section is accessible in read only mode,
    threads stack is removed to separate section '.unprivileged_stack' -
    look '...FLASH.ld' file. Heap has its separate place.
 -  Two types of threads can be created - privileged and unprivileged.
 -  Support 'true' atomic and counting semaphore based on 'ldrex'/'strex'.
 -  Supervisor can work in several modes :
    - with wdg - if you forget 'Yield' or stuck - reset will happen
    - with SysTick - event driven system. 
      Supervisor put system in sleep, SysTick or any other IRQ awakens it.
   
 
Drawbacks :
 -  If you want to use it, you should think.
 -  Cause most of the modern MCU market is consists of solutions
    based on Cortex-Mx cores, no other ports/MCU support is provided.
    And I have no plans to support computer history.
 -  In the present state, no 'cmake' or 'make' or other building tools
    are supported. The reason is, that most of the Cortex-Mx solutions, 
    developed around some kind of eclipse ports/codegenerators.
    No sane system is supported by the MCU providers in the first place.

Specifics :
 -  Logically you can't use 'stdlib' calls inside un-privileged threads,
    cause its functions internally set global variable 'errno'
    on behalf of an unprivileged caller. But on practice it is working,
    until you got an error.
 -  You can use 'stdlib' and other libraries inside privleged threads.
    STM HAL can(must) be used inside unprivileged threads - look tests.
 -  Cause unprivileged stack is separated, its size evaluated at compile-time,
    the user must say how many threads he is willing to create in 'LaOSconfig.h'.
    More than necessary can be declared, but it is wasteful.
