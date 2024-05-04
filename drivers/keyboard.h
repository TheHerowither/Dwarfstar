#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "vga.h"
#include "../cpu/irq_idt.h"

// WIP
const char *LOGO =
"      \\  |  / \n"
"        ***    \n"
"     - *   * - \n"
"        ***    \n"
"      /  |  \\ \n";

char print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            print("ERROR");
            break;
        case 0x1:
            print("ESC");
            break;
        case 0x2:
            print("1");
            break;
        case 0x3:
            print("2");
            break;
        case 0x4:
            print("3");
            break;
        case 0x5:
            print("4");
            break;
        case 0x6:
            print("5");
            break;
        case 0x7:
            print("6");
            break;
        case 0x8:
            print("7");
            break;
        case 0x9:
            print("8");
            break;
        case 0x0A:
            print("9");
            break;
        case 0x0B:
            print("0");
            break;
        case 0x0C:
            print("-");
            break;
        case 0x0D:
            print("+");
            break;
        case 0x0E:
            print("Backspace");
            break;
        case 0x0F:
            print("Tab");
            break;
        case 0x10:
            print("Q");
            break;
        case 0x11:
            print("W");
            break;
        case 0x12:
            print("E");
            break;
        case 0x13:
            print("R");
            break;
        case 0x14:
            print("T");
            break;
        case 0x15:
            print("Y");
            break;
        case 0x16:
            print("U");
            break;
        case 0x17:
            print("I");
            break;
        case 0x18:
            print("O");
            break;
        case 0x19:
            print("P");
            break;
        case 0x1A:
            print("[");
            break;
        case 0x1B:
            print("]");
            break;
        case 0x1C:
            print("ENTER");
            break;
        case 0x1D:
            print("LCtrl");
            break;
        case 0x1E:
            print("A");
            break;
        case 0x1F:
            print("S");
            break;
        case 0x20:
            print("D");
            break;
        case 0x21:
            print("F");
            break;
        case 0x22:
            print("G");
            break;
        case 0x23:
            print("H");
            break;
        case 0x24:
            print("J");
            break;
        case 0x25:
            print("K");
            break;
        case 0x26:
            print("L");
            break;
        case 0x27:
            print(";");
            break;
        case 0x28:
            print("'");
            break;
        case 0x29:
            print("`");
            break;
        case 0x2A:
            print("LShift");
            break;
        case 0x2B:
            print("\\");
            break;
        case 0x2C:
            print("Z");
            break;
        case 0x2D:
            print("X");
            break;
        case 0x2E:
            print("C");
            break;
        case 0x2F:
            print("V");
            break;
        case 0x30:
            print("B");
            break;
        case 0x31:
            print("N");
            break;
        case 0x32:
            print("M");
            break;
        case 0x33:
            print(",");
            break;
        case 0x34:
            print(".");
            break;
        case 0x35:
            print("/");
            break;
        case 0x36:
            print("Rshift");
            break;
        case 0x37:
            print("Keypad *");
            break;
        case 0x38:
            print("LAlt");
            break;
        case 0x39:
            print("Space");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                print("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                print("key up ");
                print_letter(scancode - 0x80);
            } else print("Unknown key up");
            break;
    }
}

void append(char *s, char n) {
    int len = strlen(s);
    s[len] = n;
    s[len + 1] = '\0';
}


static char key_buffer[256];

const char scancode_to_char_low[] = {
  '?', '?', '1', '2', '3', '4', '5',
  '6', '7', '8', '9', '0', '-', '=',
  '?', '?', 'q', 'w', 'e', 'r', 't',
  'y', 'u', 'i', 'o', 'p', '?', '?',
  '?', '?', 'a', 's', 'd', 'f', 'g',
  'h', 'j', 'k', 'l', '?', '?', '\'',
  '?', '<', 'z', 'x', 'c', 'v', 'b',
  'n', 'm', ',', '.', '-', '?', '?',
  '?', ' '
};

const char scancode_to_char_high[] = {
  '?', '?', '!', '"', '#', '?', '%',
  '&', '/', '(', ')', '=', '?', '`',
  '?', '?', 'Q', 'W', 'E', 'R', 'T',
  'Y', 'U', 'I', 'O', 'P', '?', '?',
  '?', '?', 'A', 'S', 'D', 'F', 'G',
  'H', 'J', 'K', 'L', '?', '?', '*',
  '?', '>', 'Z', 'X', 'C', 'V', 'B',
  'N', 'M', ';', ':', '_', '?', '?',
  '?', ' '
};

char input_buffer[1024];
uint16_t input_idx = 0;

bool backspace(char buffer[]) {
    int len = strlen(buffer);
    if (len > 0) {
        buffer[len - 1] = '\0';
        return true;
    } else {
        return false;
    }
}

void print_backspace() {
    terminal_column--;
    vga_ram[terminal_row * VGA_WIDTH + terminal_column] = ' ' | terminal_color << 8;
    vga_set_cursor(get_offset(terminal_column, terminal_row));
}

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define MOD_SHIFT 0x2A
#define MOD_CTRL 0x1D
#define MOD_ALT 0x38

uint32_t strcmp(const char *s1, const char *s2) {
    uint32_t i;
    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0') return 0;
    }
    return s1[i] - s2[i];
}

uint32_t strloc(const char *s1, const char c) {
    uint32_t i;
    for (i = 0; i < strlen(s1); i++) {
        if (s1[i] == c) return i;
    }
    return 0;
}

void exec_cmd(const char *input) {
    uint32_t idx = strloc(input, ' ');
    char *cmd = (char*)input;
    if (idx != 0) cmd[idx] = '\0';
    char *arg = ((char*)input)+idx+1;

    if (strcmp(cmd, "halt") == 0) {
        print("Halting CPU. Hope to see you again!!\n");
        asm volatile("hlt");
    }
    else if (strcmp(cmd, "os") == 0) {
        print(LOGO);
        print("You're on Dwarfstar OS\n");
        print("Developed by: TheHerowither\n");
    }
    else if (strcmp(cmd, "clear") == 0) {
        vga_clear();
    }
    else if (strcmp(cmd, "color") == 0) {
        uint8_t fg = terminal_color & 0xFF;
        uint8_t bg = (terminal_color >> 4) & 0xFF;
        switch (*arg) {
            case 'a':
                fg = VGA_COLOR_GREEN;
                break;
            case 'b':
                fg = VGA_COLOR_LIGHT_GRAY;
                break;
            case 'c':
                fg = VGA_COLOR_BLACK;
                break;
            default:
                break;
        }
        arg++;
        switch (*arg) {
            case 'a':
                bg = VGA_COLOR_GREEN;
                break;
            case 'b':
                bg = VGA_COLOR_LIGHT_GRAY;
                break;
            case 'c':
                bg = VGA_COLOR_BLACK;
                break;
            default:
                break;
        }
        terminal_color = fg | bg << 4;
        vga_update_color();
    }
    else {
        print("Unknown command: ");
        print(cmd);
        print("\n");
    }
    print("> ");
}

static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = pinb(0x60);

    if (scancode > 57) return;

    if (scancode == BACKSPACE) {
        if (backspace(key_buffer)) {
            print_backspace();
       }
    }
    else if (scancode == ENTER) {
        print("\n");
        exec_cmd(key_buffer);
        key_buffer[0] = '\0';
    }
    else {
        char letter = scancode_to_char_low[(int) scancode];
        append(key_buffer, letter);
        char str[2] = {letter, '\0'};
        print(str);
    }
}

void init_keyboard(uint8_t irq_n) {
    register_interrupt_handler(irq_n, keyboard_callback);
}

#endif //KEYBOARD_H
