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
  	VGA_COLOR_LIGHT_GREY = 7,
  	VGA_COLOR_DARK_GREY = 8,
  	VGA_COLOR_LIGHT_BLUE = 9,
  	VGA_COLOR_LIGHT_GREEN = 10,
  	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
  	VGA_COLOR_LIGHT_MAGENTA = 13,
  	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};


uint16_t *vga_ram;

uint32_t strlen(const char *str);

void term_init(void);

static const uint8_t VGA_WIDTH  = 80;
static const uint8_t VGA_HEIGHT = 25;

uint8_t terminal_row    = 0;
uint8_t terminal_column = 0;
uint8_t terminal_color  = VGA_COLOR_WHITE | VGA_COLOR_BLUE << 4;



#endif // VGA_H
