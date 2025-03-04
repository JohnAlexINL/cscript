section .text
    global __init
    global __getsp

__getsp:
    mov rax, rsp
    ret

__init:
    mov rdi, [rsp]      ; rdi = argc
    mov rsi, [rsp+8]    ; rsi = ptr to argv[0]
    mov rdx, rsp        ; rdx = stack pointer's intial value
    extern __init_c
    call __init_c