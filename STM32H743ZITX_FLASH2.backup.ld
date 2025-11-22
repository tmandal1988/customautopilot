/*
******************************************************************************
**
**  File        : STM32H743xx_FLASH.ld
**
**  Author      : STM32CubeIDE + Custom Simulink edits
**
**  Target      : STM32H743 with Simulink model "stateEstimatorAutocode"
**
*****************************************************************************
*/

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM_D1) + LENGTH(RAM_D1);    /* end of RAM */

/* Define heap and stack size */
_Min_Heap_Size = 0x10000;  /* Increase if needed */
_Min_Stack_Size = 0x400;

/* Specify the memory areas */
MEMORY
{
  FLASH     (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
  DTCMRAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 128K
  RAM_D1    (xrw) : ORIGIN = 0x24000000, LENGTH = 512K
  RAM_D2    (xrw) : ORIGIN = 0x30000000, LENGTH = 288K
  RAM_D3    (xrw) : ORIGIN = 0x38000000, LENGTH = 64K
  ITCMRAM   (xrw) : ORIGIN = 0x00000000, LENGTH = 64K
}

/* Define output sections */
SECTIONS
{
  /* Interrupt vector table */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } >FLASH

  /* Code section */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    KEEP(*(.init))
    KEEP(*(.fini))

    . = ALIGN(4);
    _etext = .;
  } >FLASH

  /* Read-only data */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  .ARM.extab (READONLY) : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
  .ARM (READONLY) :
  {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >FLASH

  .preinit_array (READONLY) :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >FLASH

  .init_array (READONLY) :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >FLASH

  .fini_array (READONLY) :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >FLASH

  /* Initialized data section (copied from FLASH) */
  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    *(.RamFunc)
    *(.RamFunc*)
    . = ALIGN(4);
    _edata = .;
  } >RAM_D1 AT > FLASH

  /* ======= SIMULINK-DIRECTED SECTION (NEW) ======= */
  /*.RAM_D2_SIMULINK (NOLOAD) :
  {
    . = ALIGN(8);
    *stateEstimator*.o(.bss*)
    *stateEstimator*.o(.data*)
    *const_params*.o(.bss*)
    *const_params*.o(.data*)
    *stateEstimatorAutocode*.o(.bss*)
    *stateEstimatorAutocode*.o(.data*)
    *stateEstimatorAutocode_data.o(.bss*)
    *stateEstimatorAutocode_data.o(.data*)
    *rt*.o(.bss*)
    *rt*.o(.data*)
    . = ALIGN(8);
  } >RAM_D2*/

  /* Uninitialized data (default BSS section) */
  . = ALIGN(4);
  .bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } >RAM_D1

  /* Heap and stack area */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE(end = .);
    PROVIDE(_end = .);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM_D1

  /* Discard unused standard libs */
  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}