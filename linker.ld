OUTPUT_FORMAT("binary")
ENTRY(_start)
SECTIONS {
    . = 0x1000;
    .text : { *(.text._start) *(.text*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) }
    .rodata : { *(.rodata*) }
    _end = .;
}
