#include "kbd.h"
#include "../interrupt/isr.h"

static void kbd_cb(registers_t r){
    u8 scancode;
    scancode = port_byte_in(REG_KBD_DATA);
    if (scancode == 0x1e){
        print("a");
    }
    if (scancode == 0x02){
        print("1");
    }
}

void reg_kbd(){
    register_interrupt_handler(IRQ1, kbd_cb);
}