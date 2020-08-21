/* Linker script for LaOS
 * main differences : 
 * - load code from flash to RAM
 * - DMA has separate region
 * - Unprivileged stack region for threads
 */

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = 0x20020000;           /* end of RAM */
/* Generate a link error if heap and stack don't fit into RAM */
_Min_Heap_Size = 0x200 ;        /* required amount of heap  */
_Min_Stack_Size = 0x400 ;       /* required amount of stack */

/* Specify the memory areas */
MEMORY
{
FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 1024K
DTCM (xrw)     : ORIGIN = 0x20000000, LENGTH = 128K
ITCM (xrw)     : ORIGIN = 0x00000000, LENGTH = 64K
AXI_SRAM1(xrw) : ORIGIN = 0x24000000, LENGTH = 256K
AXI_SRAM2(xrw) : ORIGIN = 0x24040000, LENGTH = 512K
SRAM4 (xrw)    : ORIGIN = 0x38000000, LENGTH = 64K
}

/* Define output sections */
SECTIONS
{
  _vtram = LOADADDR(.isr_vector);
  .isr_vector :
  {
    . = ALIGN(4);
    _svtable = .;
    KEEP(*(.isr_vector)) /* Startup code */
    _evtable = .;
    . = ALIGN(4);
  } >AXI_SRAM1 AT> FLASH

  .reset_handler :
  {
    . = ALIGN(4);
    KEEP(*(.reset_handler))
    . = ALIGN(4);
  } >FLASH
  
  _tram = LOADADDR(.text);
  .text :
  {
    . = ALIGN(4);
    _stext = .;
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* define a global symbols at end of code */
  } >AXI_SRAM1 AT> FLASH

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASH

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* used by the startup to initialize data */
  _sidata = LOADADDR(.data);

  /* Initialized data sections goes into RAM, load LMA copy after code */
  .data : 
  {
    . = ALIGN(4);
    _sdata = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */

    . = ALIGN(4);
    _edata = .;        /* define a global symbol at data end */
  } >DTCM AT> FLASH

  
  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    /* This is used by the startup in order to initialize the .bss secion */
    _sbss = .;         /* define a global symbol at bss start */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* define a global symbol at bss end */
    __bss_end__ = _ebss;
  } >DTCM

  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >DTCM

    .dma_buffer :
    {
        . = ALIGN(4);
        *(.dma_buffer)
    } >SRAM4

    .unprivileged_heap :
    {
        . = ALIGN(1024);
        PROVIDE ( _uphstart = . );
        . = . + 8192;
        PROVIDE ( _uphend = . );
        . = ALIGN(1024);
    } > AXI_SRAM2
    
    .separator :
    {
        . = . + 32;
    } > AXI_SRAM2
    
    .unprivileged_stack :
    {
        . = ALIGN(1024);
        _sustack = .;
        *(.unprivileged_stack)
        _eustack = .;
    } > AXI_SRAM2
    

    
  /* Remove information from the standard libraries */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}


