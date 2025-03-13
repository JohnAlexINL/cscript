#include "../src/core.h"
#include "../src/lib.h"

int main (int argc, char **argv) {
    if ( argc != 3 ) { print("This text expects an input and an output filename\n"); exit(-1); }
    i64 fsize = file_size(argv[1]);
    if ( fsize == -1 ) { print("fstat errored\n"); }
    printf("The size of %s is %d\n", argv[1], &fsize);
    char filebuffer[fsize];
    char *content = file_read( argv[1], filebuffer, fsize );
    if ( content == NULL ) { print("file_read returned null\n"); exit(1); }
    bool wsig = file_write( argv[2], content, fsize);
    if ( wsig == false ) { print("file_write returned false\n"); exit(2); }
    exit(0);
}