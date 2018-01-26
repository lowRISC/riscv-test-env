OUTPUT_ARCH( "riscv" )
ENTRY(_start)

PHDRS
{
  headers PT_PHDR PHDRS ;
  text PT_LOAD FILEHDR PHDRS ;
  data PT_LOAD ;
  dynamic PT_DYNAMIC ;
}

SECTIONS
{
  . = 0x80200000;
  .text.init : { *(.text.init) } :text
  .text : { *(.text) } :text
  .rodata : { *(.rodata*) } :text
  . = ALIGN(0x1000);
  .data : { *(.data) *(.sdata) } :data
  _bss_start = .;
  .bss : { *(.bss) }
  _bss_end = .;
  _end = .;
}

