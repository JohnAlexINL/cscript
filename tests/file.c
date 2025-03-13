#include "../src/core.h"
#include "../src/lib.h"

int main (int argc, char **argv) {
    if ( argc != 3 ) { print("This text expects an input and an output filename\n"); exit(-1); }
    i64 fsize = file_size(argv[1]);
    if ( fsize == -1 ) { print("fstat errored\n"); }
    printf("The size of %s is %d\n", argv[1], &fsize);
}