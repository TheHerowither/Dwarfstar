#include "kutils/types.h"
#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "cpu/irq_idt.h"



void kentry(void) {
    term_init();
    print("Installing interrupt service routines (ISRs)...");
    isr_install();
    print(" -> Installed!\n");

    print("Enabling external interrupts (Used for Keyboards, etc.) ...");
    asm volatile("sti");
    print(" -> Enabled!\n");

    print("Initializing keyboard (IRQ 1) ...");
    init_keyboard(IRQ1);
    print(" -> Initialized\n");

    print("\nWelcome to Dwarfstar OS\n");
    while (1);
}
