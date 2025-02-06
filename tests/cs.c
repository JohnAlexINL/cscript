#include "../cs.h"

i32 main(i32 argc, u8 **argv) {
    void * mem = brk(0x4000);
    if ( mem == NULL ) { exit(1); }
    // fill up the memory with a bunch o' hello worlds!!
    u8 helloWorld[] = "Hello, World! How are you Today\n";
    for(u32 i=0;i<0x4000;i+=sizeof(helloWorld)) {
        print("iteration 0x"); print(strHex(i)); print(" out of 0x4000\n");
        memcpy(mem+i, helloWorld, sizeof(helloWorld));
    }
    exit(0);
}