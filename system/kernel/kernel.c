#include "../interrupt/isr.h"
#define WELCOME_MSG "\nDoors v0.0.2\nSystem initialised.\nWaiting for input...\nDOORS>"



void init(){
    clear_screen();
    print("Entered protected mode.\n");

    isr_install();
    print("Interrupts registered.\n");

    asm volatile("sti");
    reg_kbd();
    print("Keyboard initialised.\n");

    // init_paging();
    // print("Paging is now enabled.");

    init_timer(100);
    print(WELCOME_MSG);
}

void main(){
    init();

    

    // u32 *ptr = (u32*)0xA0000000;
    // u32 do_page_fault = *ptr;
}

