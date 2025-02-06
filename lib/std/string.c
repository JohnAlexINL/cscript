u32 strnlen(u8 *string, u32 max) {
    u32 i;
    for(i=0;i<max;i++) {
        if ( string[i] == 0 ) { return i; }
    }   return max;
}

void print(u8 *buffer) {
    i32 len = strnlen(buffer, 1024);
    __syscall_write(STDOUT, buffer, len);
}

void println(u8 *buffer) {
    print(buffer);
    __syscall_write(STDOUT, "\n", 1);
}