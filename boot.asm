%include "interrupt.asm"
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

section .gdt
; GDT
; null segment descriptor
gdt_start:
    dq 0x0

; code segment descriptor
gdt_code:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; data segment descriptor
gdt_data:
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10010010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit)
    dd gdt_start ; address (32 bit)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
load32:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:run32

; Code section
section .text
; Entry point
global _start:function (_start.end - _start)
_start:
    mov esp, stack_top ; Initialize the stack pointer to the stack top
    jmp load32
.end:

run32:
    ; Pre-kernel initialization step
    mov ax, DATA_SEG        ; 5. update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; 6. setup stack
    mov esp, ebp


    extern kentry ; Declare the kernel's entry point as extern
    call kentry   ; Call into the kernel's entry point

    cli ; After kernel finish, disable all interrupts -
.hang: hlt ; - and hang the computer
    jmp .hang
