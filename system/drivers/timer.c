#include "timer.h"
#include "../interrupt/isr.h"
#include "screen.h"

u32 tick = 0;

static void timer_cb(registers_t r){
    tick++;
}

void init_timer(u32 freq){
    register_interrupt_handler(IRQ0, timer_cb);

    u32 divisor = CLK / freq;

    port_byte_out(0x43, 0x36);

    u8 low = low_8(divisor);
    u8 high = high_8(divisor);

    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}