i32 strlen  (char *string, i32 max) { i32 i=0; while(i < max && string[i] != '\0') { i++; } return i; }
void strcpy (char *dest, char *src, i32 max) { i32 i=0; while(i < max && src[i] != '\0' ) { dest[i] = src[i]; i++;} }
void strcat (char *dest, char *src, i32 max) { i32 len = strlen(src, max); strcpy( dest+len, src, max - len ); }
i32 strcmp  (char *dest, char *src, i32 max) { i32 i=0; while(i < max && dest[i] != '\0' && src[i] != '\0' ) { if (dest[i] != src[i]) { return dest[i] - src[i]; } i++; } return 0; }
void memcpy (char *dest, char *src, i32 max) { i32 i=0; while(i < max ) { dest[i] = src[i]; i++;} }
bool memcmp (char *dest, char *src, i32 max) { i32 i=0; while(i < max) { if ( dest[i] != src[i] ) { return false; } i++; } return true; }