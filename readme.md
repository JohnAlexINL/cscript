# cscript

This repository contains useful C-runtime stuff
for implementing interpreters, compilers,
and whatever other cool stuff.

## Structure

`core` contains things that are absolutely essential,
like the memory allocator, basic primitive definitions,
and the implementation of branching logic.

`std` contains useful abstractions built atop the
core lib for things like file i/o and basic scripting.

`types` contains implementations of more high-level
structures and functionality for things like
string manipulation, lists, et cetera.