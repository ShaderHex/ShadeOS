[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x10000
BOOT_DRIVE db 0x80

KERNEL_SEG  equ KERNEL_OFFSET >> 4 
KERNEL_OFF  equ KERNEL_OFFSET & 0x0F 

start:
    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov si, msg_loading
    call print_string

    call load_kernel

    mov si, msg_pmode
    call print_string

    call switch_to_32bit
    jmp $

print_string:
    pusha
    mov ah, 0x0E
    mov bh, 0x00
.print_char:
    lodsb
    cmp al, 0
    je .done
    int 0x10
    jmp .print_char
.done:
    popa
    ret

disk_load:
    pusha
    mov ax, KERNEL_SEG
    mov es, ax
    mov bx, KERNEL_OFF

    mov si, msg_esbx
    call print_string
    mov ax, es
    call print_hex
    mov si, space
    call print_string
    mov ax, bx
    call print_hex
    mov si, newline
    call print_string

    mov ah, 0x02
    mov al, 20
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    mov dl, [BOOT_DRIVE]
    int 0x13
    jc .disk_err

    mov si, msg_int13_ah_ok
    call print_string
    xor ah, ah
    mov al, 0

    mov al, ah 
    mov si, newline
    call print_string

    popa
    ret

.disk_err:
    mov si, msg_disk_error
    call print_string
    mov al, ah
    xor ah, ah
    call print_hex
    mov si, newline
    call print_string
    jmp $

load_kernel:
    mov dl, [BOOT_DRIVE]

    mov al, dl
    mov ah, 0

    mov si, msg_drive
    call print_string
    call print_hex
    mov si, newline
    call print_string

    call disk_load
    ret

print_hex:
    pusha
    mov cx, 5
.hex_loop:
    rol ax, 4
    mov bx, ax
    and bx, 0x0f
    mov bl, [hex_chars + bx]
    mov ah, 0x0e
    mov al, bl
    int 0x10
    loop .hex_loop
    popa
    ret

hex_chars db '0123456789ABCDEF'

msg_loading    db "Booting...", 13, 10, 0
msg_disk_error db "Disk error! Code: 0x", 0
msg_drive      db "Drive: 0x", 0
msg_pmode      db "PMode...", 0
msg_esbx       db "ES:BX = 0x", 0
msg_int13_ah_ok db "INT13 returned AH=", 0
space          db " ", 0
newline        db 13,10,0

gdt_start:
    dq 0x0
gdt_code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

switch_to_32bit:
    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:init_32bit

[bits 32]
init_32bit:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x200000
    mov esp, ebp

    mov eax, KERNEL_OFFSET
    jmp eax

times 510-($-$$) db 0
dw 0xAA55
