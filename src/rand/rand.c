#include "../core.h"

static u32 _rand_seed = 0;
static u32 (*_rand_fn)(void) = NULL;

void rands(u32 seed) { _rand_seed = seed; }
void randfn(u32 (*fn)(void)) { _rand_fn = fn; }
u32 rand(void) { if (!_rand_fn) return 0; return _rand_fn(); }

u32 lcg_rand(void) {
    _rand_seed = _rand_seed * 1103515245 + 12345;
    return (_rand_seed >> 16) & 0x7FFF;
}

int __glibc_rand_r[344]; // https://www.mscs.dal.ca/~selinger/random/
void glibc_rand_init(void) {
    int i; __glibc_rand_r[0] = (int)_rand_seed;
    for (i=1; i<31; i++) {
        __glibc_rand_r[i] = (16807LL * __glibc_rand_r[i-1]) % 2147483647;
        if (__glibc_rand_r[i] < 0) { __glibc_rand_r[i] += 2147483647; }
    }
}

i32 glibc_rand(void) {
    static bool initialized = false;
    static i32 index = 344; // Start at the end of the buffer
    if (!initialized) { glibc_rand_init(); initialized = true; }
    if (index >= 344) {
        for (i32 i = 0; i < 31; i++)    { __glibc_rand_r[i] = __glibc_rand_r[i + 313] + __glibc_rand_r[i + 3]; }
        for (i32 i = 31; i < 344; i++)  { __glibc_rand_r[i] = __glibc_rand_r[i - 31] + __glibc_rand_r[i - 3]; }
        index = 0; // Reset position
    }
    return __glibc_rand_r[index++] >> 1; // Extract and move forward
}