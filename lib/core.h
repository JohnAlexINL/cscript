#include "core/primitives.h"
i32 main(i32 argc, u8 **argv);

#define SYSCALL_READ    0
#define SYSCALL_WRITE   1
#define SYSCALL_OPEN    2
#define SYSCALL_CLOSE   3
#define SYSCALL_EXECVE  59
#define SYSCALL_EXIT    60

#define STDIN           0
#define STDOUT          1
#define STDERR          2

extern void __syscall_read      (u32 fd, u8 *buf, i32 count);
extern void __syscall_write     (u32 fd, u8 *buf, i32 count);
extern void __syscall_exit      (i32 code);

#include "core/debug.h"

u32 __init_c1_wsp(u8 *string){ i32 i=0; while(string[i]!=' ' && string[i]!=0) { i++; } return i; }
void __init_c2(i32 argc, u8 *head) {
    i32 i; i32 len; u8 *argv[argc];
    for(i=0;i<argc;i++) {
        len = __init_c1_wsp(head);
        argv[i] = head;
        head[len] = 0;
        head += len+1;
    }
    main(argc, argv);
    // if main doesn't exit correctly...
    __syscall_exit(SYSCALL_EXIT);
}