section .text
    global __init_c1

__init_c1:
    mov rdi, [rsp]      ; rdi = argc
    mov rsi, [rsp+8]    ; rsi = ptr to argv[0]
    extern __init_c2
    call __init_c2