[bits 32]
global _start
extern main
extern kernel_load_addr

_start:
    mov dword [kernel_load_addr], 0x42
    call main
    jmp $
