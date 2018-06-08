; Prints out the hex value of dx
print_hex:
    pusha
    mov bx, HEX_OUT ; move the string template into mem
    add bx, 5 ; place us at the end of the string
    mov cx, 0 ; loop counter initialisation

    loop_hex:
        cmp cx, 4 ; end loop if we've rotated four times
        je  end_hex
        
        mov ax, dx ; copy dx into our working register
        and ax, 0x000f ; mask out all values except the last
        add al, 0x30 ; convert it into ascii numbers

        cmp al, 0x39 ; if number > 0x39, add 39 to make it into a-f
        jle then_hex
        add al, 39

    then_hex:
        mov [bx], al ; move the char into the correct position
        ror dx, 4 ; rotate dx, so we can extract the next value
        sub bx, 1 ; next position
        add cx, 1 ; loop
        jmp loop_hex
    
    end_hex:
        sub bx, 1 ; move down to the origin
        call print ; print the string
        popa
        ret