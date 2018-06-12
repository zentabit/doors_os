; load dh sectors to memory
disk_load:
    push dx ; save dx

    mov ah, 0x02 ; sector read
    mov al, dh ; number of sectors
    mov ch, 0x00 ; cylinder 0
    mov dh, 0x00 ; head 0
    mov cl, 0x02 ; second sector

    int 0x13 ; read interrupt
    jc  disk_error ; jump to if carry flag is set

    pop dx ; pop from the stack
    cmp dh, al ; check if actual number of bytes were read
    jne disk_error ; jump if n bytes were not equal
    ret

disk_error:
    mov bx, DISK_ERROR_MSG ; print error message
    call print
    jmp $

; vars
DISK_ERROR_MSG:
    db "Disk read error!", 0
