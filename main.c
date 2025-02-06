#include "lib/core.h"
#include "lib/std.h"

u8 hello[] = "Hello, World!\n";

i32 main(i32 argc, u8 **argv) {
    i32 i;
    i32 len;
    __syscall_write(
        STDOUT,
        __strhex( (i64)(argv) ),
        8
    );
    for(i=0;i<argc;i++) {
        // first, write the address
        __syscall_write(
            STDOUT,
            __strhex( (i64)(argv + i) ),
            8
        );
        __syscall_write(STDOUT, " -> ", 4);
        // then the value
        len = strnlen( (argv[i]), 16 );
        __syscall_write(
            STDOUT,
            argv[i],
            len
        );
        __syscall_write(STDOUT, "\n", 1);
    }
    __syscall_exit(0);
}