#include "../src/core.h"

int main (int argc, char **argv) {
    void * heap = brk(NULL);
    void * alloc = brk( heap + 0xFFFF );
    exit(0);
}