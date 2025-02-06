#include "std/alloc.c"
#include "std/string.c"
#include "std/strcast.c"
#include "std/mem.c"

void *brk (u64 heapsize) { return __alloc(heapsize); }
void exit(i32 status){ __syscall_exit(status); }