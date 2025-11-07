[bits 16]

memory_function:
    ; simple example: write 'M' to screen
    mov ah, 0x0E
    mov al, 'M'
    int 0x10
