#define print_maxlen 1024
int mulsten[] = { 1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };

i32 strlen(char *string, i32 max) {
    i32 i=0; while(i < max && string[i] != '\0') { i++; }
    return i;
}

void print(char *string){
    i32 len = strlen(string, print_maxlen);
    if ( len > 0 ) { write(STDOUT, string, len); }
}

void print_u32(unsigned int number){}
void print_i32(int number){}
void print_float(float num) {}


void print_h32(unsigned int number) {
    char *str = string_hex32(number);
    int len = strlen(str, 8);
    write(STDOUT, str, len);
}

void __printf_t(char t, void *) {
    
}

void printf(char *fmt, ...) {
    int len = strlen(fmt, print_maxlen);
    char *last = fmt;
    char *c = fmt;
    va_list args;
    va_start(args, fmt);

    while(*c) {
        if (*c == '%' && *(c+1)) {
            int chars = (uintptr_t)c - (uintptr_t)last;
            write(STDOUT, last, chars);
            c++; switch(*c) {
                case 's': {
                    char *string = va_arg(args, char *);
                    print(string); break; }
                case 'd': case 'i': {
                    i32 num = va_arg(args, i32);
                    print_i32(num); break; }
                case 'u': {
                    u32 num = va_arg(args, u32);
                    print_u32(num); break; }
                case 'h': {
                    u16 num = va_arg(args, u16);
                    print_u32((u32)num); break; }
                case 'c': {
                    u8 num = va_arg(args, u8);
                    print_u32((u32)num); break; }
                case 'x': {
                    u32 num = va_arg(args, u32);
                    print_h32(num); break; }
                case 'f': {
                    f32 num = va_arg(args, f32);
                    print_float(num); break; }
                default: {
                    write(STDERR, c, 1); break; }
            }
            c++; last = c; // again to move past the `%c`
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
        else {
            c++; // for actual text, just keep a schmovin'
        }
    }

    if (c != last) {
        int chars = (uintptr_t)c - (uintptr_t)last;
        write(STDOUT, last, chars);
    }
    va_end(args);
}