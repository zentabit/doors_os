
#include "kbd.h"
#include "../interrupt/isr.h"



static void kbd_cb(registers_t r){
    u8 scancode;
    scancode = port_byte_in(REG_KBD_DATA);
    if ((int)scancode > LEN_KBD){
        return;
    }
    if (scancode == ENTR){
        // input();
        print("\n");
    } else if (scancode == BKSP) {
        print_bksp();
    } else {
        char c[2] = {kbd[(int)scancode], '\0'};
        print(c);
    }
}

void reg_kbd(){
    register_interrupt_handler(IRQ1, kbd_cb);
}