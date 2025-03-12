; C-standard calling convention puts arguments into, in order:
; rdi, rsi, rdx, rcx, r8, r9
; so we just need to make sure we move stuff from them appropriately
%macro syscall 1
    global __syscall_%1
__syscall_%1:
    mov rax, %1
    syscall
    ret
%endmacro

section .text
    global __syscall_read
    global __syscall_write
    global __syscall_open
    global __syscall_close
    global __syscall_fstat
    global __syscall_mmap
    global __syscall_munmap
    global __syscall_execve
    global __syscall_exit

__syscall_read:     syscall 0
__syscall_write:    syscall 1
__syscall_open:     syscall 2
__syscall_close:    syscall 3
__syscall_fstat:    syscall 5
__syscall_mmap:     syscall 9
__syscall_munmap:   syscall 11
__syscall_execve:   syscall 59
__syscall_exit:     syscall 60