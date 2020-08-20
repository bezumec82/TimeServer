This is a simple switching mechanism for Cortex-M microcontrollers.
Main features :
 -  Stack overflow protection.
 -  Utilization of separate stack pointers.
 -  Threads running in unprivileged mode - 
    it is up to the user to configure MPU to protect memory/peripheral
 -  SVC handler with codes recognition - 
    user can add its codes to process in function 'SVC_Handler_C' 
    in sysCall.cpp, or redefine it completely.
 -  Configuration file 'LaOSconfig.h'

Cause most of the modern MCU market is consists of solutions 
based on Cortex-Mx cores, no other ports/MCU support is provided.
And I have no plans to support computer history.

The base usage pattern is demonstrated in file 'main.cpp'.
The idea is simple - show your thread functions to
the core and it will cycle it for you.
Don't forget to call 'Yeild', when the job is done.

At the present moment solemn usage of OS 
is to link it in your eclipse-based project.
Just drug folder LaOS inside your project 
and add 'LaOS/Inc' in 'Includes' :
Properties -> C/C++ General -> Path and Symbols -> Includes

To successfully compile your project, folder 'LaOS/support' 
should be removed from sources. Add a filter:
Properties -> C/C++ General -> Path and Symbols -> Source Location

Inside folder '/support' provided fixed linker script
and '..start.s' files. The main idea behind it to keep code in FLASH,
and execute it from SRAM. Such an arrangement makes it possible to 
organize the hot re-flash of MCU.
In the present moment only STM32H575 is supported.

In the present moment, no 'cmake' or 'make' or other building tools
are supported. The reason is, that most of the Cortex-Mx solutions, 
developed around some kind of eclipse ports.
No sane system is supported by the MCU providers.