#include "../src/core.h"
#include "../src/lib.h"
#include "../src/rand.h"

int main (int argc, char **argv) {
    void * heap = brk(NULL);
    char subA[] = "Hello\0";
    char subB[] = "World\0";
    printf("Let's test our printf with just some strings\n%s %s\n",
        subA, subB
    );
    float pi = 3.14;
    i32 number = 0x8000;
    i32 negative = -1024;
    printf("float %f\nint %d\nnegative int %d\nhex seed %x\n", 
        &pi, &number, &negative, &number);
    print("Now let's test out our random function:\n");
    randfn(glibc_rand);
    rands(0x8000);
    u32 i; for(i=0;i<6;i++) {
        i32 random = rand();
        printf("%d ", &random);
    }   print("\n");
}