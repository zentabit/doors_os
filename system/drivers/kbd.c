
#include "kbd.h"
#include "../interrupt/isr.h"

#define BFR_LEN 256

char kbd_buffer[BFR_LEN] = {'\0'};
u8 index = 0;

// Check if first five letters spell "clear", then clear, else print input
void input(char buffer[]){
    char clear[6] = "clear";
    int clear_cmd =1;
    int i = 0;
    while (i<6){
        if (clear[i] != buffer[i]) clear_cmd = 0;
        i++;
    }

    if (clear_cmd){
        clear_screen();
    } else {
        print("\n");
        print(buffer);
    }
}

static void kbd_cb(registers_t r){
    u8 scancode;
    scancode = port_byte_in(REG_KBD_DATA);
    if ((int)scancode > LEN_KBD){
        return;
    }
    if (scancode == ENTR){
        input(kbd_buffer);
        index = 0;
        for(int i = 0; i < BFR_LEN; i++){
            kbd_buffer[i] = '\0';
        }
        print("\nDOORS>");
    } else if (scancode == BKSP) {
        // print_bksp();
    } else {
        char c[2] = {kbd[(int)scancode], '\0'};
        kbd_buffer[index] = kbd[(int)scancode];
        index++;
        print(c);
    }
}

void reg_kbd(){
    register_interrupt_handler(IRQ1, kbd_cb);
}