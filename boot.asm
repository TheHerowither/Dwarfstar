MBALIGN  equ 1<<0               ; Align loaded modules on page boundaries
MEMINFO  equ 1<<1               ; Memory map
MBFLAGS  equ MBALIGN | MEMINFO  ; Multiboot flag field
MAGIC    equ  0x1BADB002        ; Header magic number
CHECKSUM equ -(MAGIC + MBFLAGS) ; Checksum of above, to prove Multiboot

; Multiboot header
section .multiboot
align 4
    dd MAGIC
    dd MBFLAGS
    dd CHECKSUM

; Stack info
section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:

; Code section
section .text
global _start:function (_start.end - _start)
_start:
    mov esp, stack_top ; Initialize the stack pointer to the stack top

    ; Pre-kernel initialization step

    extern kentry ; Declare the kernel's entry point as extern
    call kentry   ; Call into the kernel's entry point

    cli ; After kernel finish, disable all interrupts -
.hang: hlt ; - and hang the computer
    jmp .hang

.end:
