[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000
BOOT_DRIVE db 0x80

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
    mov ah, 0x02
    mov al, 11
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    mov dl, [BOOT_DRIVE]
    mov bx, KERNEL_OFFSET
    int 0x13
    jc disk_error
    popa
    ret

disk_error:
    mov si, msg_disk_error
    call print_string
    jmp $

load_kernel:
    mov bx, KERNEL_OFFSET
    mov dl, [BOOT_DRIVE]
    
    mov si, msg_drive
    call print_string
    mov al, dl
    call print_hex
    mov si, newline
    call print_string
    
    call disk_load
    ret

; Add hex print function
print_hex:
    pusha
    mov cx, 4
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
msg_drive db "Drive: 0x", 0
newline db 13, 10, 0

; GDT
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
    mov ebp, 0x90000
    mov esp, ebp
    
    call KERNEL_OFFSET
    jmp $

msg_loading db "Booting...", 13, 10, 0
msg_disk_error db "Disk error!", 0
msg_pmode db "PMode...", 0

times 510-($-$$) db 0
dw 0xAA55