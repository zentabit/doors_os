#ifndef IDT_H
#define IDT_H

#include "types.h"
#define KERNEL_CS 0x08
#define ENTRIES 256

struct idt_gate_struct {
    u16 base_lo;
    u16 sel;
    u8 always0;
    u8 flags;
    u16 base_hi; 
} __attribute__((packed));
typedef struct idt_gate_struct idt_gate_t;

struct idt_ptr_struct {
    u16 limit;
    u32 base;
} __attribute__((packed));
typedef struct idt_ptr_struct idt_reg_t;

void set_idt_gate(int n, u32 handler);
void set_idt();

#endif