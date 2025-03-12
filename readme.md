# cscript

This repository contains useful C-runtime stuff
for implementing interpreters, compilers,
and whatever other cool stuff.

## Structure

`core` contains only the things that are absolutely
necessary to get a working runtime, like the
initialization steps before calling `main()`,
definitions for primitive types,
and wrappers for system calls.

`lib` contains useful abstractions built atop the
core for things like file i/o and basic scripting.

`types` contains implementations of more high-level
structures and functionality for things like
string manipulation, lists, et cetera.

## Purpose

Mainly, this exists to help show what the guts of
a C program actually looks like under it all.
Given I'm writing my own runtimes and compilers and stuff,
I felt like I was gonna be doing this kinda stuff anywho
and I should throw this at GitHub or whatever.

This is not intended for actual, real-world use,
but rather as just a cool look into how
a functional C-like runtime works

## Usage

After building the runtime with the `build` script,
you should be able to link against the `core.o`, `lib.o`,
or any other parts you need alongside their header files.

The main differences between this core and something like
`glibc` are the size and complexity of them.
In our tests folder, we have some comparisons of
simple scripts using our standard library versus
`glibc` and difference is massive.

```bash
clib_core    14 kilobytes 
clib_lib     19 kilobytes 
glibc_core   744 kilobytes
glibc_lib    749 kilobytes
```

> [!NOTE]
> This runtime also isn't done yet,
> so the size will go up a little.
> That noted, I do not expect that I will
> even approach closing the gab between the two.

## Limitations

This is specifically a runtime for
x86-64 Linux, and will not be rewritten
for any other systems.
Additionally, this is not a replacement
for the standard C library --
it does things a little differently,
and also just leaves out a lot of
standard lib stuff.

## Functions Lists

|✓| `core.h` Function | Args | Notes |
|--|--|--|--|
|x| __init | | _program entry point, initializes heap and stack_ |
|x| brk | `void *addr` | _core-implemented bump allocator_ |
|x| read | `u32 fd`, `u8 *buf`, `i32 count` |  |
|x| write | `u32 fd`, `u8 *buf`, `i32 count` |  |
|x| open | `const char *pathname`, `i32 flags`, `i32 mode` |  |
|x| close | `u32 fd` |  |
|x| mmap | `void *addr`, `u32 length`, `i32 prot`, `i32 flags`, `i32 fd`, `u32 offset` |  |
|x| munmap | `void *addr`, `u32 length` |  |
|x| execve | `char *pathname`, `char *argv[]`, `char *envp[]` |  |
|x| exit | `i32 status` |  |
|x| fstat | `u32 fd`, `stat *` | |
|_| fork |  |
|_| socket | `i32 domain`, `i32 type`, `i32 protocol` |
|_| connect | `i32 sockfd`, `const void *addr`, `u32 addrlen` |
|_| bind | `i32 sockfd`, `const void *addr`, `u32 addrlen` |
|_| listen | `i32 sockfd`, `i32 backlog` |
|_| accept | `i32 sockfd`, `void *addr`, `u32 *addrlen` |
|_| send | `i32 sockfd`, `const void *buf`, `u32 len`, `i32 flags` |
|_| recv | `i32 sockfd`, `void *buf`, `u32 len`, `i32 flags` |
|_| poll | `void *fds`, `u32 nfds`, `i32 timeout` |
|_| ioctl | `i32 fd`, `u32 request`, `void *argp` |

|✓| `lib.h` Function | Args | Notes |
|--|--|--|--|
|x| print | `char *` |  |
|x| printf | `char *fmt` `void *[] ...` | `...` is a list of pointers, not values |
|x| strfmt | `char *fmt`, `void *[]...` | |
|x| strlen | `char *`, `int max` | |
|x| strcpy | `char *dest`, `char *src`, `int max` | |
|x| strcat | `char *base`, `char *adder`, `int max` | |
|x| strcmp | `char *base`, `char *adder`, `int max`  | |
|x| memcpy | `void *dest`, `void *src`, `int size` | |
|x| memcmp | `void *base`, `void *adder`, `int size` | |
|x| string_hex32 | `u32` | |
|x| string_u32 | `u32` | |
|x| string_i32 | `i32` | |
|_| string_f32 | `f32`/`f64` | _rename function_ |
|x| string_e32 | `f32`/`f64` | _rename function_ |
|_| string_b32 | `u32` | |
|x| file_read | `char *filename` | _checks size, allocates memory, and returns filled buffer_
|x| file_write | `char *filename`, `char *buffer`, `int size` | |
|_| file_append | `char *filename`, `char *buffer`, `int size` | |
|x| file_size | `char *filename` | |

|✓| `types.h` Function | Args | Notes |
|--|--|--|--|
|_| list_new | `int size` | |
|_| list_push | `list_t *`, `void *` | |
|_| list_pop | `list_t *` | |
|_| list_insert | `list_t *`, `int index`, `void *` | |
|_| list_remove | `list_t *`, `int index` | |
|_| list_merge | `list_t *base`, `list_t *adder` | |
|_| list_memsize | `list_t *` | used to determine size for `list_flatten(*dest)` |
|_| list_flatten | `void *dest`, `list_t *src`, `int newsize` | |
|_| table_new | `int size`, `int spill` | _spill handles collisions & requests a resize if filled_ |
|_| table_get | `table_t *`, `int index` | _interally handles wrapping_ |
|_| table_set | `table_t *`, `int index`, `void *` | |
|_| table_hashid | `char *` | _returns an index_ |
|_| table_merge | `table_t *dest`, `table_t *src` | |
|_| table_memsize | `table_t *` | |
|_| table_flatten | `void *dest`, `table_t *src`, `int newsize` | |

|✓| `rand.h` Function | Args | Notes |
|--|--|--|--|
|x| rand | | _always returns `0` if not initialized_ |
|x| rands | `u32` | _sets seed for `randfn` |
|x| randfn | `function *(void)` | _sets the PRNG function_ |
|x| glibc_rand | `void` | a reimplementation of _glibc_'s `rand` function |