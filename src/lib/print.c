#define print_maxlen 1024

i32 strlen(char *string, i32 max) {
    i32 i=0; while(i < max && string[i] != '\0') { i++; }
    return i;
}

void print(char *string){
    i32 len = strlen(string, print_maxlen);
    if ( len > 0 ) { write(STDOUT, string, len); }
}

void print_u32(unsigned int number)     { print( string_u32(number) ); }
void print_i32(int number)              { print( string_i32(number) ); }
void print_float(float number)          { print( string_e32(number) ); } // TODO: move to f32/64
void print_h32(unsigned int number)     { print( string_hex32(number) ); }

#define printf(fmt, ...) _printf(fmt, (void *[]){__VA_ARGS__, NULL}) // redefined in lib.h
void _printf(char *fmt, void *args[]) {
    int len = strlen(fmt, print_maxlen);
    char *last = fmt;
    char *c = fmt;
    while(*c) {
        if (*c == '%' && *(c+1)) {
            int chars = (uintptr_t)c - (uintptr_t)last;
            write(STDOUT, last, chars);
            c++; switch(*c) {
                case 's': { char *string = *(char **)args; args++; print(string); break; }
                case 'd': case 'i': { i32 num = **(i32 **)args; args++; print_i32(num); break; }
                case 'u': { u32 num = **(u32 **)args; args++; print_u32(num); break; }
                case 'h': { u16 num = **(u32 **)args; args++; print_u32((u32)num); break; }
                case 'c': { u8 num = **(u8 **)args; args++; print_u32((u32)num); break; }
                case 'x': { u32 num = **(u32 **)args; args++; print_h32(num); break; }
                case 'f': { f32 num = **(f32 **)args; args++; print_float(num); break; }
                default: { write(STDERR, c, 1); break; }
            }   c++; last = c; // again to move past the `%c`
        }
        else if (*c == '\\' && *(c+1)) {
            int chars = (uintptr_t)c - (uintptr_t)last;
            write(STDOUT, last, chars);
            c++; switch(*c){
                case '\\': write(STDOUT, "\\", 1); break;
                case 't': write(STDOUT, "\t", 1); break;
                case 'r': write(STDOUT, "\r", 1); break;
                case 'n': write(STDOUT, "\n", 1); break;
                case 'e': write(STDOUT, "\e", 1); break;
                default: write(STDERR, c, 1); break;
            }
            c++; last = c;
        }
        else { c++; } // for actual text, just keep a schmovin'
    }

    if (c != last) {
        int chars = (uintptr_t)c - (uintptr_t)last;
        write(STDOUT, last, chars);
    }
}