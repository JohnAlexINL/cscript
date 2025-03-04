#include "../src/core.h"
#include "../src/lib.h"

int main (int argc, char **argv) {
    void * heap = brk(NULL);
    char subA[] = "Hello\0";
    char subB[] = "World\0";
    printf("Let's test our printf with just some strings\n%s\n%s\n",
        subA, subB
    );
}