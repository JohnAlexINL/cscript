u8 __strnbuf[32] = { 0 };
u8 __strnums[] = "0123456789ABCDEF";

u8 *string_hex32(u32 value){
    u32 i=0; __strnbuf[(sizeof(__strnbuf)) - 1] = '\0';
    char *c = __strnbuf + sizeof(__strnbuf) - 2;
    for (i=0; i<8; i++) {
        *c = __strnums[value % 16]; c--;
        value /= 16; if (value == 0) { break; }
    }   return (c + 1);
}

u8 *string_u32(u32 value) {
    i32 i = 0; __strnbuf[(sizeof(__strnbuf)) - 1] = '\0';
    char *c = __strnbuf + sizeof(__strnbuf) - 2;
    for (i = 0; i < 10; i++) {
        *c = __strnums[value % 10]; c--;
        value /= 10; if (value == 0) { break; }
    }   return (c + 1);
}

u8 *string_i32(i32 value) {
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
u8 *string_f32(f32 value){
    return __strnbuf;
}

u8 *string_e32(f32 number) {
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