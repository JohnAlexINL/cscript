#define strfmt(buffer, max, fmt, ...) __strfmt(buffer, max, fmt, (void *[]){__VA_ARGS__, NULL})
char *__strfmt(char *buffer, u32 max, char *fmt, void *args[]) {
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