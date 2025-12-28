; C-standard calling convention puts arguments into, in order:
; rdi, rsi, rdx, rcx, r8, r9
; so we just need to make sure we move stuff from them appropriately
%macro syscall 1
    global syscall_%1
syscall_%1:
    mov rax, %1
    syscall
    ret
%endmacro

section .text
    global syscall_read
    global syscall_write
    global syscall_open
    global syscall_close
    global syscall_lseek
    global syscall_mmap
    global syscall_munmap
    global syscall_execve
    global syscall_exit

syscall_read:     syscall 0
syscall_write:    syscall 1
syscall_open:     syscall 2
syscall_close:    syscall 3
syscall_lseek:    syscall 8
syscall_mmap:     syscall 9
syscall_munmap:   syscall 11
syscall_execve:   syscall 59
syscall_exit:     syscall 60