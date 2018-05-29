OUTPUT_ARCH( "riscv" )
ENTRY(_start)

PHDRS
{
  text PT_LOAD ;
  data PT_LOAD ;
}

SECTIONS
{
  . = 0x40000000;
  .text.init : { *(.text.init) } :text
  .text : { *(.text) } :text
  .rodata : { *(.rodata*) } :text
  . = ALIGN(0x8000);
  .data : { *(.data) *(.sdata) *(.sbss) } :data
  . = ALIGN(0x8);
  _bss_start = .;
  .bss : { *(.bss) }
  _bss_end = .;
  _end = .;
  . = 0x80000000;
  _sbrk = .;
}
