/* Linker script for LaOS
 * main differences : 
 * - load code from flash to RAM
 * - DMA has separate region
 * - Unprivileged stack region for threads
 * - Unprivileged piece of DTCM
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
/* D1 domain - M7 */
ITCM (xrw)     : ORIGIN = 0x00000000, LENGTH = 64K

/* 2MiB of FLASH in two banks */
FLASHA (rx)    : ORIGIN = 0x08000000, LENGTH = 1024K
FLASHB (rx)    : ORIGIN = 0x08200000, LENGTH = 1024K

DTCM (xrw)     : ORIGIN = 0x20000000, LENGTH = 128K

/* Divide it in two parts */
AXI_SRAM1(xrw) : ORIGIN = 0x24000000, LENGTH = 256K
AXI_SRAM2(xrw) : ORIGIN = 0x24040000, LENGTH = 256K

/* D2 domain - M4, DMA1, DMA2, ETH, SD2, USB */
SRAM1 (xrw)    : ORIGIN = 0x30000000, LENGTH = 128K
SRAM2 (xrw)    : ORIGIN = 0x30020000, LENGTH = 128K
SRAM3 (xrw)    : ORIGIN = 0x30040000, LENGTH = 32K

/* D3 domain - BDMA */
SRAM4 (xrw)    : ORIGIN = 0x38000000, LENGTH = 64K
BUPSRAM (xrw)  : ORIGIN = 0x38800000, LENGTH = 4K
}

/* Define output sections */
SECTIONS
{
  _vtFlash = LOADADDR(.isr_vector);
  .isr_vector :
  {
    . = ALIGN(4);
    _vtStart = .;
    KEEP(*(.isr_vector)) /* Startup code */
    _vtEnd = .;
    . = ALIGN(4);
  } >AXI_SRAM1 AT> FLASHA

  .reset_handler :
  {
    . = ALIGN(4);
    KEEP(*(.reset_handler))
    . = ALIGN(4);
  } >FLASHA
  
  _textFlash = LOADADDR(.text);
  .text :
  {
    . = ALIGN(4);
    _textStart = .;
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _textEnd = .;
  } >AXI_SRAM1 AT> FLASHA

  /* Constant data goes into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } >FLASHA

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASHA
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASHA

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASHA
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASHA
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASHA

    /* Atomic variables accessible by unprivileged threads
     * must appear first in DTCM due to MPU alignment. 
     * Can't be placed in any SRAM - ldrex/strex causes hard fault.
     * 'huart' placed in any SRAM doesn't work.
     *
     * Keep it as little as possible. */
    _upDataFlash = LOADADDR(.unprivileged_data);
    . = ALIGN(4);
    .unprivileged_data :
    {
    PROVIDE (_upDataStart = .);
    *(.unprivileged_data)
    PROVIDE (_upDataEnd = .);
    } >DTCM AT> FLASHA
    
    .unprivileged_bss :
    {
    . = ALIGN(4);
    PROVIDE (_upBssStart = .);
    *(.unprivileged_bss)
    
    . = ALIGN(512);     /* Set allign according mpu region */
                        /* Protection gap until data section 
                         * User should consider it for his application. */
    PROVIDE (_upBssEnd = .);        
    } >DTCM

  /* Initialized data will be copied from FLASH to SRAM */
  _dataFlash = LOADADDR(.data);
  .data :
  {
    . = ALIGN(4);
    _dataStart = .;        /* create a global symbol at data start */
    *(.data)           /* .data sections */
    *(.data*)          /* .data* sections */
    . = ALIGN(4);
    _dataEnd = .;        /* define a global symbol at data end */
  } >DTCM AT> FLASHA

  /* Uninitialized data section */
  . = ALIGN(4);
  .bss :
  {
    PROVIDE( _bssStart = .);
    __bss_start__ = _bssStart;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    PROVIDE( _bssEnd = . );
    __bss_end__ = _bssEnd;
  } >DTCM
  
  /* User_heap_stack section, used to check that there is enough RAM left */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . ); /* Used in _sbrk */
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >DTCM 

    /* h7 specifics */
    .dma_buffer :
    {
      . = ALIGN(4);
      *(.dma_buffer)
    } >SRAM4


   /* !!! Attention !!! 
    The start address of an MPU region 
    must be multiple of the MPU size.
    Heap after stack - 
    heap is known at this point, stack not. */
     
    /* Only for unprivileged threads */
    .unprivileged_stack :
    {
        . = ALIGN(8192);
        PROVIDE ( _upStackStart = . );
        *(.unprivileged_stack)
        PROVIDE ( _upStackEnd = . );
    } > AXI_SRAM2
    
    /* Gap for MPU. It separates stack from heap.
     * Its size will depend. */
    .separator :
    {
        . = . + 32;
    }
    
    .unprivileged_heap :
    {
        . = ALIGN(8192);
        PROVIDE ( _upHeapStart = . );
        . = . + 8192;
        PROVIDE ( _upHeapEnd = . );
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


