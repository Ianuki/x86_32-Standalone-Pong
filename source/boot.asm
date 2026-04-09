; This code is buns i'm not gonna lie
; Don't ask me to explain it 100% cuz like i know how to 
; explain some of the stuff but not allovit so yeah pls

[org 0x7c00]
[bits 16]

mov [BOOT_DRIVE], dl

mov ah, 0x02 ; some cursor BS (vga teletype mode)
mov bh, 0x00   
mov dh, 26
mov dl, 0
int 0x10

mov ah, 0x02 ; interrupt 13h my beloved
mov al, 5
mov ch, 0x00
mov dh, 0x00
mov cl, 0x02
mov dl, [BOOT_DRIVE]
mov bx, 0x1000
int 0x13
jc disk_error

cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 0x1
mov cr0, eax

jmp 0x08:init_32bit

[bits 32]
init_32bit:
mov ax, 0x10
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

mov esp, 0x90000 ; le stack
mov ebp, esp
and esp, 0xFFFFFFF0 
sub esp, 16  

jmp 0x1000 ; jmp to the main routine

disk_error:
    mov ah, 0x0e
    mov al, 'E'
    int 0x10
    jmp $

gdt_start:
    dq 0x0
gdt_code:
    dw 0xffff, 0x0, 0x9a00, 0x00cf
gdt_data:
    dw 0xffff, 0x0, 0x9200, 0x00cf
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

BOOT_DRIVE db 0

times 510-($-$$) db 0
dw 0xaa55
