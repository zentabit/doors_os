; Prints out the string in bx
; Ends on null-termination

print_string:
    pusha
    mov ah, 0x0e ; Teletype mode

    loop:
        mov al, [bx] ; Copy first char to al
        cmp al, 0x00 ; Check if char is null
        je end ; if the char is null, this is the end of the string
        jmp then ; else, print char
    then:
        int 0x10 ; print contents of al
        add bx, 0x1 ; move one char to the right
        jmp loop
    end:
        popa
        ret

; Prints out a new line
newline:
    pusha
    mov ah, 0x0e
    mov al, 0x0a
    int 0x10
    mov al, 0x0d
    int 0x10
    popa
    ret

; Combines print_string and newline, to make print
print:
    pusha
    call print_string
    call newline
    popa
    ret
