#ifndef VGA_H
#define VGA_H

#include "../kutils/types.h"




enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
  	VGA_COLOR_MAGENTA = 5,
  	VGA_COLOR_BROWN = 6,
  	VGA_COLOR_LIGHT_GRAY = 7,
  	VGA_COLOR_DARK_GRAY = 8,
  	VGA_COLOR_LIGHT_BLUE = 9,
  	VGA_COLOR_LIGHT_GREEN = 10,
  	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
  	VGA_COLOR_LIGHT_MAGENTA = 13,
  	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

uint16_t *const vga_ram = (uint16_t *const) 0xB8000;

uint32_t strlen(const char *str);

void term_init(void);
void print(const char *str);

uint8_t pinb(uint16_t port);
void poutb(uint16_t port, uint8_t data);

void vga_set_cursor(uint32_t offset);
uint32_t vga_get_cursor();

static const uint8_t VGA_WIDTH  = 80;
static const uint8_t VGA_HEIGHT = 25;

uint8_t terminal_row    = 0;
uint8_t terminal_column = 0;
uint8_t terminal_color  = VGA_COLOR_LIGHT_GRAY | VGA_COLOR_BLACK << 4;

void term_init(void) {
    for (unsigned int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_ram[i] = ' ' | terminal_color << 8;
    }
}

int get_offset(int col, int row) {
    return 2 * (row * VGA_WIDTH + col);
}

uint32_t strlen(const char *str) {
    uint32_t len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

void print(const char *str) {
    uint32_t offset = vga_get_cursor();
    for (size_t i = 0; i < strlen(str); i++) {
        switch (str[i]) {
            case '\n':
                terminal_row++;
                terminal_column=0;
                break;
            case '\r':
                terminal_column = 0;
                offset = 0;
                break;
            case '\t':
                terminal_column += 4;
                offset+=(2*4);
                break;
            default:
                vga_ram[terminal_row * VGA_WIDTH + terminal_column++] = str[i] | terminal_color << 8;
                offset+=2;
                break;
        }
        if (terminal_column >= VGA_WIDTH - 1) {
            terminal_column = 0;
            terminal_row++;
        }
    }
    vga_set_cursor(get_offset(terminal_column, terminal_row));
}

uint8_t pinb(uint16_t port) {
    uint8_t res;
    __asm__("in %%dx, %%al" : "=a" (res) : "d" (port));
    return res;
}

void poutb(uint16_t port, uint8_t data) {
    __asm__("out %%al, %%dx" : :"a" (data), "d" (port));
}

void vga_set_cursor(uint32_t offset) {
    offset /= 2;
    poutb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    poutb(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
    poutb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    poutb(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

uint32_t vga_get_cursor() {
    poutb(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    uint32_t offset = pinb(VGA_DATA_REGISTER) << 8;
    poutb(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += pinb(VGA_DATA_REGISTER);
    return offset * 2;
}


#endif // VGA_H
