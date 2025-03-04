#define va_list char*  // We treat va_list as a pointer to a char

// Macro to start va_list - setup the pointer to the first argument
#define va_start(vl, num) (vl = (char *)&num + sizeof(num))

// Macro to retrieve the next argument of the specified type
#define va_arg(vl, type) (*(type *)(vl += sizeof(type) - sizeof(char*)))

// Macro to clean up the va_list
#define va_end(vl) (void)0  // No action needed for this implementation