#include "../interrupt/isr.h"
#define WELCOME_MSG "\nDoors v0.0.3\nSystem initialised.\nWaiting for input...\nDOORS>"

void init(){
    clear_screen();
    print("Entered protected mode.\n");

    isr_install();
    print("Interrupts registered.\n");

    asm volatile("sti");
    reg_kbd();
    print("Keyboard initialised.\n");

    print("Trying to enable paging...");
    // init_paging();
    print("enabled.");

    init_timer(100);
    print(WELCOME_MSG);
}

void main(){
    init();
}

