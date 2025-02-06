u8 __strnbuf[11] = { 0 };
u8 __strnums[] = "0123456789ABCDEF";
u8 *__strhex(u32 value){
    u32 i=0; for(i=0;i<8;i++) {
        __strnbuf[(sizeof(__strnbuf-2))-i] = __strnums[ value % 16 ];
        value /= 16;
        if ( value == 0 ) { break; }
    }   return __strnbuf + sizeof(__strnbuf) - i - 3;
}

u8 *__strint(i32 value){
    bool positive = true; if ( value < 0 ) { positive = false; }
    i32 i=0; for(i=0;i<10;i++) {
        __strnbuf[(sizeof(__strnbuf-2))-i] = __strnums[ value % 10 ];
        value /= 10;
        if ( value == 0 ) { break; }
    }   return __strnbuf + sizeof(__strnbuf) - i - 3;
}

void print(u8 *buffer) {
    i32 i=0; while(buffer[i] != 0) { i++; }
    __syscall_write(STDOUT, buffer, i);
    __syscall_write(STDOUT, "\n", 1);
}