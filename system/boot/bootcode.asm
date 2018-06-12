[org 0x7c00]
KERNEL_OFFSET equ 0x1000
    mov [BOOT_DRIVE], dl

    mov bp, 0x9000
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print

    call load_kernel

    call switch_to_pm

    jmp $

%include "print/print_string.asm"
%include "print/print_string_pm.asm"
%include "disk/disk_load.asm"
%include "protmode/gdt.asm"
%include "protmode/switch_to_pm.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    
    mov bx, KERNEL_OFFSET
    mov dh, 15
    mov dl, [BOOT_DRIVE]
    call disk_load
    
    ret

[bits 32]

BEGIN_PM:
    mov ebx, MSG_PROT_MODE
    call print_string_pm

    call KERNEL_OFFSET

    jmp $

BOOT_DRIVE      db 0
MSG_LOAD_KERNEL db "Loading kernel into memory.", 0
MSG_REAL_MODE   db "Started in 16-bit mode.", 0
MSG_PROT_MODE   db "Landed in 32-bit mode.", 0

times 510-($-$$) db 0
dw 0xaa55

; ; A boot  sector  that  boots a C kernel  in 32-bit  protected  mode
; [org 0x7c00]
; KERNEL_OFFSET  equ 0x1000   ; This is the  memory  offset  to which  we will  load  our  kernel
; mov [BOOT_DRIVE], dl ; BIOS  stores  our  boot  drive  in DL, so it’s
; ; best to  remember  this  for  later.
; mov bp, 0x9000         ; Set -up the  stack.
; mov sp, bp
; mov bx, MSG_REAL_MODE ; Announce  that we are  starting
; call  print     ; booting  from 16-bit  real  mode
; call  load_kernel       ; Load  our  kernel
; call  switch_to_pm      ; Switch  to  protected  mode , from  which
; ; we will  not  return
; jmp $
; ; Include  our useful , hard -earned  routines
; %include "print/print_string.asm"
; %include "disk/disk_load.asm"
; %include "protmode/gdt.asm"
; %include "print/print_string_pm.asm"
; %include "protmode/switch_to_pm.asm"
; [bits  16]
; ; load_kernel
; load_kernel:
; mov bx, MSG_LOAD_KERNEL    ; Print a message  to say we are  loading  the  kernel
; call print
; mov bx, KERNEL_OFFSET      ; Set -up  parameters  for  our  disk_load  routine , so
; mov dh, 15                   ; that we load  the  first  15  sectors (excluding
; mov dl, [BOOT_DRIVE]       ; the  boot  sector) from  the  boot  disk (i.e.  our
; call disk_load               ;   kernel  code) to  address  KERNEL_OFFSET
; ret
; [bits  32]
; ; This is where we  arrive  after  switching  to and  initialising  protected  mode.

; BEGIN_PM:
; mov ebx , MSG_PROT_MODE ; Use  our 32-bit  print  routine  to
; call  print_string_pm    ; announce  we are in  protected  mode
; call  KERNEL_OFFSET      ; Now  jump to the  address  of our  loaded
; ; kernel  code , assume  the  brace  position ,
; ; and  cross  your  fingers.  Here we go!
; jmp $                      ; Hang.
; ; Global  variables
; BOOT_DRIVE        db 0
; MSG_REAL_MODE    db "Started  in 16-bit  Real  Mode", 0
; MSG_PROT_MODE    db "Successfully  landed  in 32-bit  Protected  Mode", 0
; MSG_LOAD_KERNEL  db "Loading  kernel  into  memory.", 0
; ; Bootsector  padding
; times  510-($-$$) db 0
; dw 0xaa55