#ifndef     LIB_H
#define     LIB_H

#ifdef      CORE_H_HEADLIB
    #include "core.h"
#endif

// ------   Functions and Definitions

#define     alloc(type)     __alloc(sizeof(type))
#define printf(fmt, ...)    __printf(fmt, (void *[]){__VA_ARGS__, NULL})
#define strfmt(buffer, max, fmt, ...) __strfmt(buffer, max, fmt, (void *[]){__VA_ARGS__, NULL})

void *      dealloc         (void *);

i64         file_read       (char *filename, char *buffer, i64 max); 
i64         file_write      (char *filename, char *content, i64 size); 
i64         file_size       (char *filename);

void        print           (char *string);
void        print_u32       (unsigned int number);
void        print_i32       (int number);
void        print_float     (float number);
void        print_h32       (unsigned int number);

u8 *        string_hex32    (u32 value);
u8 *        string_u32      (u32 value);
u8 *        string_i32      (i32 value);
u8 *        string_f32      (f32 value);
u8 *        string_e32      (f32 number);

i32         strlen          (char *string, i32 max);   
void        strcpy          (char *dest, char *src, i32 max);
void        strcat          (char *dest, char *src, i32 max);
i32         strcmp          (char *dest, char *src, i32 max);
void        memcpy          (char *dest, char *src, i32 max);
bool        memcmp          (char *dest, char *src, i32 max);

#ifndef print_maxlen
    #define print_maxlen        1024
#endif

// ------   Internal Functions

void *      __alloc         (int size);

// ------   Private Variables

static void * __alloc_ptr;
static u8 __strnbuf[32] = { 0 };
static u8 __strnums[] = "0123456789ABCDEF";

// ------   Implementations

void *      __alloc         (int size)
{
    if (!__alloc_ptr) { __alloc_ptr = brk(NULL); }
    __alloc_ptr += size;
    return brk(__alloc_ptr);
}

void *      dealloc         (void * ref)
{
    if (!ref) { return NULL; }
    __alloc_ptr = brk(ref); return __alloc_ptr;
}

i64         file_size       (char *filename) 
{
    const int OFFSET = 0; const int SEEK_END = 2;
    i32 fd = syscall_open(filename, 0, 0);
    if ( fd == -1 ) { return -1; }
    i64 size = syscall_lseek(fd, OFFSET, SEEK_END);
    syscall_close(fd);
    return size;
}

i64         file_read       (char *filename, char *buffer, i64 max) 
{
    const i32 flags_ro = 0; const i32 modes = 0;
    i64 filesize = file_size(filename);
    if ( filesize <= 0 ) { print("file_read: file nil sized\n"); return -1; }
    if ( filesize >= max ) { filesize = max-1; } // keep within bounds 
    u32 fd = syscall_open(filename, flags_ro, modes); if ( fd == -1 ) { return -2; }
    i64 total_read = 0; // if we can't do it in one pass, keep trying
    while (total_read < filesize) {
        i32 rsize = syscall_read(fd, buffer + total_read, filesize - total_read); 
        if (rsize <= 0) {  syscall_close(fd); return -3; }
        total_read += rsize;
    }   syscall_close(fd); *((char *)buffer + filesize) = '\0'; return total_read;
}

i64         file_write      (char *filename, char *content, i64 size) 
{
    const i32 flags_ctw = 0x201 | 0x401 | 0x40; /* O_CREAT | O_WRONLY | O_TRUNC */
    const i32 modes = 0x1B6; /* 0666 permissions */
    i32 fd = syscall_open(filename, flags_ctw, modes);
    if (fd < 0) { return -1; }
    i64 total_written = 0; // just in case we can't write everything in one pass
    while (total_written < size) {
        i32 wsize = syscall_write(fd, content + total_written, size - total_written);
        if (wsize <= 0) {  syscall_close(fd); return -2; }   
        total_written += wsize;
    }   syscall_close(fd); return total_written;
}

void        print           (char *string)              { i32 len = strlen(string, print_maxlen); if ( len > 0 ) { syscall_write(STDOUT, string, len); } }
void        print_u32       (unsigned int number)       { print( string_u32(number) ); }
void        print_i32       (int number)                { print( string_i32(number) ); }
void        print_float     (float number)              { print( string_e32(number) ); } // TODO: move to f32/64
void        print_h32       (unsigned int number)       { print( string_hex32(number) ); }

void        __printf        (char *fmt, void *args[]) 
{
    int len = strlen(fmt, print_maxlen);
    char *last = fmt;
    char *c = fmt;
    while(*c) {
        if (*c == '%' && *(c+1)) {
            int chars = (uintptr_t)c - (uintptr_t)last;
            syscall_write(STDOUT, last, chars);
            c++; switch(*c) {
                case 's': { char *string = *(char **)args; args++; print(string); break; }
                case 'd': case 'i': { i32 num = **(i32 **)args; args++; print_i32(num); break; }
                case 'u': { u32 num = **(u32 **)args; args++; print_u32(num); break; }
                case 'h': { u16 num = **(u32 **)args; args++; print_u32((u32)num); break; }
                case 'c': { u8 num = **(u8 **)args; args++; print_u32((u32)num); break; }
                case 'x': { u32 num = **(u32 **)args; args++; print_h32(num); break; }
                case 'f': case 'e': { f32 num = **(f32 **)args; args++; print_float(num); break; }
                default: { syscall_write(STDERR, c, 1); break; }
            }   c++; last = c; // again to move past the `%c`
        }
        else if (*c == '\\' && *(c+1)) {
            int chars = (uintptr_t)c - (uintptr_t)last;
            syscall_write(STDOUT, last, chars);
            c++; switch(*c){
                case '\\': syscall_write(STDOUT, "\\", 1); break;
                case 't': syscall_write(STDOUT, "\t", 1); break;
                case 'r': syscall_write(STDOUT, "\r", 1); break;
                case 'n': syscall_write(STDOUT, "\n", 1); break;
                case 'e': syscall_write(STDOUT, "\e", 1); break;
                default: syscall_write(STDOUT, c, 1); break;
            }
            c++; last = c;
        }
        else { c++; } // for actual text, just keep a schmovin'
    }

    if (c != last) {
        int chars = (uintptr_t)c - (uintptr_t)last;
        syscall_write(STDOUT, last, chars);
    }
}

u8 *string_hex32(u32 value)
{
    u32 i=0; __strnbuf[(sizeof(__strnbuf)) - 1] = '\0';
    char *c = __strnbuf + sizeof(__strnbuf) - 2;
    for (i=0; i<8; i++) {
        *c = __strnums[value % 16]; c--;
        value /= 16; if (value == 0) { break; }
    }   return (c + 1);
}

u8 *string_u32(u32 value) 
{
    i32 i = 0; __strnbuf[(sizeof(__strnbuf)) - 1] = '\0';
    char *c = __strnbuf + sizeof(__strnbuf) - 2;
    for (i = 0; i < 10; i++) {
        *c = __strnums[value % 10]; c--;
        value /= 10; if (value == 0) { break; }
    }   return (c + 1);
}

u8 *string_i32(i32 value) 
{
    i32 i = 0; __strnbuf[(sizeof(__strnbuf)) - 1] = '\0';
    bool negative = ( value < 0 );
    if (negative) { value *= -1; }
    char *c = __strnbuf + sizeof(__strnbuf) - 2;
    for (i = 0; i < 10; i++) {
        *c = __strnums[value % 10]; c--;
        value /= 10; if (value == 0) { break; }
    }   if ( negative ) { *c = '-'; return (c); }   
    return (c + 1);
}

// TODO: Implement regular float print
u8 *string_f32(f32 value)
{
    return __strnbuf;
}

u8 *string_e32(f32 number)
{
    char *c = __strnbuf;
    if (number < 0) { *c++ = '-'; number = -number; } 
    else { *c++ = '+'; }
    int exponent = 0; // Normalize number to 1.xxxxxx format
    while (number >= 10.0f) { number /= 10.0f; exponent++; }
    while (number > 0.0f && number < 1.0f) { number *= 10.0f; exponent--; }
    int mantissa = (int)(number * 1000000); // Convert mantissa (6 decimal places)
    c += 7; // Reserve space for mantissa + decimal
    *c = '\0'; // Null-terminate
    for (int i = 6; i > 0; i--) { *--c = '0' + (mantissa % 10); mantissa /= 10; }
    *--c = '.'; // Insert decimal point
    *--c = '0' + mantissa; // First digit
    // Append exponent
    c += 8; // Move to exponent position
    *c++ = 'e';
    *c++ = (exponent < 0) ? '-' : '+';
    if (exponent < 0) exponent = -exponent;
    *c++ = '0' + (exponent / 10);
    *c++ = '0' + (exponent % 10);
    *c = '\0'; // Final null-termination
    return __strnbuf;
}

char *      __strfmt        (char *buffer, u32 max, char *fmt, void *args[])
{
    char *d = buffer;
    char *c = fmt;
    u32 remainder;
    while (*c && (remainder = ((uintptr_t)buffer + max) - (uintptr_t)d) > 0) {
    switch(*c) {
        case '%': {
            if ( *(c+1) ){
                char * ref = c+1;
                switch(*(c+1)) {
                    case 's': ref = *(char**)args; break;
                    case 'd': case 'i': ref = string_i32(**(i32 **)args); break;
                    case 'u': ref = string_u32(**(u32 **)args); break;
                    case 'x': ref = string_hex32(**(u32 **)args); break;
                    case 'f': case 'e': ref = string_e32(**(f32**)args); break;
                }
                int len = strlen(ref, remainder);
                strcpy(d, ref, len);
                d += len; c++;
            }
            else { *d = *c; d++; c++; }
        }
        case '\\': {
            if ( *(c+1) ){
                switch(*(c+1)){
                    case 'n': *d = '\n'; break;
                    case 'r': *d = '\r'; break;
                    case 'e': *d = '\e'; break;
                    case 't': *d = '\t'; break;
                    default: *d = *c; break;
                }   d++; c++;
            }
            else { *d = *c; d++; c++; }
        }
        default: d++; c++;
    }}
    *d = '\0';
    return buffer;
}

i32 strlen  (char *string, i32 max)             { i32 i=0; while(i < max && string[i] != '\0') { i++; } return i; }
void strcpy (char *dest, char *src, i32 max)    { i32 i=0; while(i < max && src[i] != '\0' ) { dest[i] = src[i]; i++;} }
void strcat (char *dest, char *src, i32 max)    { i32 len = strlen(src, max); strcpy( dest+len, src, max - len ); }
i32 strcmp  (char *dest, char *src, i32 max)    { i32 i=0; while(i < max && dest[i] != '\0' && src[i] != '\0' ) { if (dest[i] != src[i]) { return dest[i] - src[i]; } i++; } return 0; }
void memcpy (char *dest, char *src, i32 max)    { i32 i=0; while(i < max ) { dest[i] = src[i]; i++;} }
bool memcmp (char *dest, char *src, i32 max)    { i32 i=0; while(i < max) { if ( dest[i] != src[i] ) { return false; } i++; } return true; }

#endif