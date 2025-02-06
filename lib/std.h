i32 strnlen(u8 *string, i32 max) {
    i32 i;
    for(i=0;i<max;i++) {
        if ( string[i] == 0 ) { return i; }
    }   return max;
}