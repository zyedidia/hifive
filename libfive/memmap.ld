/* based on page 22 of the FE310-G002 Manual */
OUTPUT_ARCH("riscv")

ENTRY(_start)

MEMORY {
  flash(rx) : ORIGIN = 0x20010000, LENGTH = 0x400000
  ram(rwx) : ORIGIN = 0x80000000, LENGTH = 0x4000
}

SECTIONS {
  __stack_size = 2K;

  .text : {
    __code_start__ = .;
    KEEP(*(.text.boot))
    *(.text*)
    *(.rodata*)
    *(.srodata*)
    . = ALIGN(4);
    __code_end__ = .;
  } >flash

  . = ALIGN(4);
  .rodata : {
    *(.rodata*)
    *(.srodata*)
    *(.eh*)
  } >flash AT>flash

  . = ALIGN(4);
  .data 0x80000000 : AT (ADDR(.rodata) + SIZEOF(.rodata) ) {
    __data_start_flash__ = LOADADDR(.data);
    __data_start__ = .;
    *(.data*)
     __global_pointer$ = . + 0x800;
    *(.sdata*)
    *(.data*)
    __data_end__ = .;
  }

  . = ALIGN(4);
  .bss ADDR(.data) + SIZEOF(.data) : {
    __bss_start__ = .;
    *(.bss*)
    *(.sbss*)
    *(COMMON)
    __bss_end__ = .;
    . = ALIGN(8);
    __heap_start__ = .;
  } > ram 

  /DISCARD/ : {
    *(.comment)
    *(.riscv.attributes)
  }

  .stack ORIGIN(ram) + LENGTH(ram) - __stack_size (NOLOAD) :
  {
    __heap_end__ = .;
    __stack_bottom__ = .;
    . = __stack_size;
    __stack_top__ = .;
  } > ram AT>ram

  PROVIDE(__data_lma__ = LOADADDR(.data));
  PROVIDE(__code_end__ = ADDR(.text) + SIZEOF(.text));
}

