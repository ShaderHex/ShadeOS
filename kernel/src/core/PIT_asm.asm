extern system_timer_fractions
extern system_timer_ms
extern IRQ0_fractions
extern IRQ0_ms
extern IRQ0_frequency
extern PIT_reload_value

IRQ0_handler:
    push rax
    push rbx

    mov rax, [rel IRQ0_fractions]
    mov rbx, [rel IRQ0_ms]
    add [rel system_timer_fractions], rax
    adc [rel system_timer_ms], rbx
 
    mov al, 0x20
    out 0x20, al

    pop rbx
    pop rax
    iretq

init_PIT:
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, rdi
    cmp rax, 18
    jl .freq_min
    cmp rax, 1193181
    jg .freq_max
    jmp .freq_ok

.freq_min:
    mov rax, 18
    jmp .freq_ok

.freq_max:
    mov rax, 1193181

.freq_ok:
    mov rbx, rax

    mov rax, 1193182
    xor rdx, rdx
    div rbx
    mov rbx, rax
    mov [PIT_reload_value], bx
    mov [IRQ0_frequency], rdi

    mov rax, rbx
    mov rcx, 1000
    mul rcx
    shl rax, 32
    mov rcx, 1193182
    xor rdx, rdx
    div rcx
    mov [IRQ0_ms], eax
    mov [IRQ0_fractions], edx

    cli
    mov al, 00110100b
    out 0x43, al

    mov ax, [PIT_reload_value]
    out 0x40, al
    mov al, ah
    out 0x40, al

    sti

    pop rdx
    pop rcx
    pop rbx
    pop rax
    ret

.l1:
    mov rbx,3
    mov rdx,0
    div rbx
    cmp rdx,3 / 2
    jb .l2
    inc rax
.l2:

.gotReloadValue:
    push rax
    mov [PIT_reload_value],ax
    mov rbx,rax
 
    mov rax,3579545
    mov rdx,0
    div rbx
    cmp edx,3579545 / 2
    jb .l3
    inc rax
.l3:
    mov rbx,3
    mov rdx,0
    div rbx
    cmp rdx,3 / 2
    jb .l4
    inc rax
.l4:
    mov [IRQ0_frequency],rax
 
    pop rbx
    mov rax,0xDBB3A062
    mul rbx
    shrd rax,rdx,10
    shr rdx,10
 
    mov [IRQ0_ms],rdx
    mov [IRQ0_fractions],rax
 
    push rax
    push rbx
    push rcx
    push rdx
    cli
 
    mov al,00110100b
    out 0x43, al
 
    mov ax,[PIT_reload_value]
    out 0x40,al
    mov al,ah
    out 0x40,al
 
    pop rax
    pop rbx
    pop rcx
    pop rdx
 
    ret

extern CountDown

section .text
global TimerIRQ
TimerIRQ:
    push rax
    mov rax, [CountDown]
    test rax, rax
    jz TimerDone
    dec rax
    mov [CountDown], rax
TimerDone:
    pop rax
    iretq
