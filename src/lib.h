extern void     print(char *);
extern void     print_i32(i32 number);
extern void     print_u32(u32 number);
extern void     print_float(float number);
extern void     print_h32(u32 number);
extern void     _printf(const char *fmt, void *args[]);
extern i32      strlen(char *string, i32 max);
#define printf(fmt, ...) _printf(fmt, (void *[]){__VA_ARGS__, NULL})