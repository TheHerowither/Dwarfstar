ENTRY(_start)

SECTIONS {
    . = 2M;

    /*First add the multiboot header, then the code*/
    .text BLOCK(4K) : ALIGN(4K) {
        *(.multiboot)
        *(.gdt)
        *(.text)
    }

    /*Read only data*/
    .rodata BLOCK(4K) : ALIGN(4K) {
        *(.rodata)
    }

    /*Initialized read write data*/
    .data BLOCK(4K) : ALIGN(4K) {
        *(.data)
    }

    /*Uninitialized read write data, and the stack*/
    .bss BLOCK(4K) : ALIGN(4K) {
        *(COMMON)
        *(.bss)
    }
}
