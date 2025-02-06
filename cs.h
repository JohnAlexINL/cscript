// PRIMITIVES
    #define u8  unsigned char
    #define u16 unsigned short
    #define u32 unsigned int
    #define u64 unsigned long long
    #define i8  signed char
    #define i16 signed short
    #define i32 signed int
    #define i64 signed long long
    #define f32 float
    #define f64 double
    #define bool u32
    #define true 1
    #define false 0
    #define NULL 0

// MAIN
    i32 main(i32 argc, u8 **argv);

// SYSTEM CALLS
    void *brk(u64 heapsize);
    void exit(i32 status);

// MEMORY MANIPULATION
    void memcpy(u8 *dest, u8 *src, u32 count);

// STRING
    u8 *strHex(u32 value);
    u8 *strInt(i32 value);
    u32 strnlen(u8 *string, u32 max);
    void print(u8 *buffer);
    void println(u8 *buffer);