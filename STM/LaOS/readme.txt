This is simple switching mechanism for Cortex-M microcontrollers.
Main features :
 -  Stack overflow protection.
 -  Utilization of separate stack pointes.
 -  Threads running in unprivileged mode - 
    it is up to user to configure MPU to protect memory/peripheral
 -  SVC handler with codes recognition - 
    user can add its codes to process in function 'SVC_Handler_C' 
    in sysCall.cpp, or redefine its completely.
 -  Configuration file 'LaOSconfig.h'

Cause most of modern MCU market is consists of solutions 
based on Cortex-Mx cores, no other ports/MCU support provided.
And I have no plans to support computer history.

Base usage pattern is demonstrated in file 'main.cpp'.
The idea is simple - show your thread functions to
the core ant it will cycle it for you.
Don't forget to call Yeild, when job done.

At the present moment solemn usage of OS 
is to link it in your eclipse-based project.
Just drug folder LaOS inside your project 
and add 'LaOS/Inc' in 'Includes' :
Properties -> C/C++ General -> Path and Symbols -> Includes

To successfully compile project folder 'LaOS/support' 
should be removed from sources. Add a filter:
Properties -> C/C++ General -> Path and Symbols -> Source Location

Inside folder '/support' provided fixed linker script
and '..start.s' files. Main idea behind it to keep code in FLASH,
and execute it from SRAM. Such arrangement makes possible to 
organize hot re-flash of MCU.
In present moment only STM32H575 is supported.

In present momen no 'cmake' or 'make' or other building tools
are supported. The reason is, because most of Cortex-Mx solutions 
developed around some kind of eclipse ports.
No sane system is supported be the MCU providers.
