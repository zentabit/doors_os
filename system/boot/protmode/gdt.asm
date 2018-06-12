gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff ; limit
    dw 0x0 ;base
    db 0x0 ; base
    db 10011010b ; flags
    db 11001111b ; more flags and limit
    db 0x0 ; base

gdt_data:
    dw 0xffff ; limit
    dw 0x0 ; base
    db 0x0 ; base
    db 10010010b ; flags
    db 11001111b ; flags and limit
    db 0x0 ; base

gdt_end:


gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size of the gdt
    dd gdt_start ; gdt start address

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
