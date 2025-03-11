#include <stdio.h>
#include <stdlib.h>

int main (int argc, char **argv) {
    void * heap;
    char subA[] = "Hello\0";
    char subB[] = "World\0";
    printf("Let's test our printf with just some strings\n%s %s\n",
        subA, subB
    );
    float pi = 3.14;
    int number = 0x8000;
    int negative = -1024;
    printf("float %f\nint %d\nnegative int %d\nhex seed %x\n", 
        pi, number, negative, number);
    printf("Now let's test out our random function:\n");
    srand(0x8000);
    int i; for(i=0;i<6;i++) {
        int random = rand();
        printf("%d ", random);
    }   printf("\n");
}