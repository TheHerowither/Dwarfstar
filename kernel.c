#include "kutils/types.h"
#include "drivers/vga.h"



void kentry(void) {
    term_init();
    print("Hello, kernel World!\n");
    print("Welcome to Dwarfstar OS\n");
    for (uint32_t i = 0; i <= 100; i++) {
        printuint("", i);
        print("/100\n");
    }
}
