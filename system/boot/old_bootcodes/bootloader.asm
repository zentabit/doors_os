; main program

[org 0x7c00] ; define origin address
    mov bx, HELLO_MSG ; set message.
    call print ; print message

    mov bx, GOODBYE_MSG
    call print

    mov dx, 0x1fb6 ; set hex value
    call print_hex ; print hex value

    jmp $

%include "print/print_string.asm"
%include "print/print_hex.asm"

; data
HELLO_MSG:
    db 'Hello, World!', 0

GOODBYE_MSG:
    db 'Goodbye!', 0

HEX_OUT:
    db '0x0000', 0

; filling and the magic number
times 510-($-$$) db 0
dw 0xaa55