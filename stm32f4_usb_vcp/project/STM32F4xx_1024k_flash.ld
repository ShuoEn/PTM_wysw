/* 
  Martin Thomas, December 2012
  Copyright (c) 2012, 3BSD license
  
  This is based on information from ARM (launchpad), Attolic (STM32F4 examples)
  the GNU linker manual and the default linker-script (arm-none-eabi-ld --verbose)
*/

ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx)        : ORIGIN = 0x08000000, LENGTH = 1024K
  RAM1 (xrw)        : ORIGIN = 0x20000000, LENGTH = 112K
  RAM2(xrw)         : ORIGIN = 0x2001C000, LENGTH = 16K
  CCMRAM(xrw)       : ORIGIN = 0x10000000, LENGTH = 64K
  FSMC_BANK1_2(rx)  : ORIGIN = 0x60000000, LENGTH = 512M
  FSMC_BANK3_4(rx)  : ORIGIN = 0x80000000, LENGTH = 512M
  FSMC_BANK5(rx)    : ORIGIN = 0xA0000000, LENGTH = 512M
}

SECTIONS
{
	.text :
	{
		CREATE_OBJECT_SYMBOLS
		. = ALIGN(4);
		___SVECTOR = .;
		KEEP(*(.isr_vector))
		ASSERT (. != ___SVECTOR, "No isr_vector input-section!");
		. = ALIGN(4);
		*(.text .text.* .gnu.linkonce.t.*)

		*(.plt)
		*(.gnu.warning)
		*(.glue_7)         /* glue arm to thumb code */
		*(.glue_7t)        /* glue thumb to arm code */
		*(.vfp11_veneer)

		*(.gcc_except_table)
		KEEP(*(.eh_frame_hdr*))
		KEEP(*(.eh_frame*))
		KEEP(*(.init))
		KEEP(*(.fini))

		/* .ctors */
		*crtbegin.o(.ctors)
		*crtbegin?.o(.ctors)
		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
		*(SORT(.ctors.*))
		*(.ctors)

		/* .dtors */
 		*crtbegin.o(.dtors)
 		*crtbegin?.o(.dtors)
 		*(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
 		*(SORT(.dtors.*))
 		*(.dtors)

		*(.rodata .rodata.* .gnu.linkonce.r.*)

	} > FLASH

	.ARM.extab : 
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH
	__exidx_end = .;

	.text.align_etext :
	{
		. = ALIGN(8);
		_etext = .;
	} >FLASH

	_sidata = .;

	.data : AT (_sidata)
	{
		. = ALIGN(4);
		_sdata = .;
		*(vtable)
		*(.got.plt) *(.got)
		*(.shdata)
		*(.data) *(.data.*) *(.gnu.linkonce.d.*)

		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);

		. = ALIGN(4);
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

		. = ALIGN(4);
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);

		. = ALIGN(4);
		*(.ram_function*)

		. = ALIGN(4);
		/* All data end */
		_edata = .;

	} > RAM1

	. = ALIGN(4);
	.bss :
	{
		. = ALIGN(4);
		__bss_start__ = .;
		_sbss = __bss_start__;
		*(.bss .bss.* .gnu.linkonce.b.*)
		*(COMMON)
		. = ALIGN(8);
		__bss_end__ = .;
		_ebss = __bss_end__;
	} > RAM1
	
	.heap (NOLOAD):
	{
		. = ALIGN(4);
		__end__ = .;
		end = __end__;
		_end = __end__;
		*(.heap*)
		__HeapLimit = .;
	} > RAM1

	/* .stack_dummy section doesn't contains any symbols. It is only
	 * used for linker to calculate size of stack sections, and assign
	 * values to stack symbols later */
	.stack_dummy (NOLOAD) :
	{
		*(.stack*)
	} > RAM1

	/* Set stack top to end of RAM, and stack limit move down by
	 * size of stack_dummy section */
	__StackTop = ORIGIN(RAM1) + LENGTH(RAM1);
	_estack = __StackTop;
	__StackLimit = __StackTop - SIZEOF(.stack_dummy);
	PROVIDE(__stack = __StackTop);
	
	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")

  .stab 0 (NOLOAD) : { *(.stab) }
  .stabstr 0 (NOLOAD) : { *(.stabstr) }
  /* DWARF debug sections.
   * Symbols in the DWARF debugging sections are relative to the beginning
   * of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }

  .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
  .ARM.attributes 0 : { KEEP (*(.ARM.attributes)) KEEP (*(.gnu.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }

}