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

`std` contains useful abstractions built atop the
core lib for things like file i/o and basic scripting.

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
you should be able to link against the `runtime.o`
object alongside the `cs.h` header file
for the necessary C function prototypes and definitions.

A program that just prints whatever commandline arguments
were passed in might look like
```c
#include "cs.h"

i32 main(i32 argc, u8 **argv) {
    i32 i;
    for(i=0;i<argc;i++) {
        print(argv[i]);
    }
}
```
Doing something in "normal" C is only a little different:
```c
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    int i;
    for(i=0;i<argc;i++) {
        printf("%s\n",argv[i]);
    }
}
```
But crucially, when we look at the size difference
between binaries made with `gcc` for these two,
the size difference is staggering

```bash
ours                             8 kilobytes (9096 bytes)
ours_tcc                         3 kilobytes (4040 bytes)
theirs                           744 kilobytes (762,192 bytes)
```

Compiling our runtime with `tcc` compared to
`glibc` using `gcc` gives us a binary
only half a percent the size!

> [!NOTE]
> This runtime also isn't done yet,
> so the size will go up a little.
> That noted, I do not expect that I will
> even approach closing the gab between the two.

## Planned Features

- Wrappers for many Linux syscalls
- A very small memory allocator
- Many of the functions available from `string.h`
- Additional stuff like `list.h` and `table.h`

## Limitations

This is specifically a runtime for
x86-64 Linux, and will not be rewritten
for any other systems.
Additionally, this is not a replacement
for the standard C library --
it does things a little differently,
and also just leaves out a lot of
standard lib stuff.