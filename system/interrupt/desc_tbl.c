#include "desc_tbl.h"

idt_gate_t idt[ENTRIES];
idt_reg_t idt_reg;

void set_idt_gate(int n, u32 handler){
    idt[n].base_lo = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E;
    idt[n].base_hi = high_16(handler);
}

void set_idt(){
    idt_reg.base = (u32) &idt;
    idt_reg.limit = ENTRIES * sizeof(idt_gate_t) - 1;

    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg)); 
}