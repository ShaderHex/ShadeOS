[bits 16]
[org 0x7c00]

KERNEL_OFFSET equ 0x1000
BOOT_DRIVE db 0

start:
    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp
    
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
    mov al, 10              ; Load 10 sectors (5KB)
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
    call disk_load
    ret

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
    
    ; Call kernel directly
    call KERNEL_OFFSET
    jmp $

msg_loading db "Booting...", 13, 10, 0
msg_disk_error db "Disk error!", 0
msg_pmode db "PMode...", 0

times 510-($-$$) db 0
dw 0xAA55