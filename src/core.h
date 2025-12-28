#ifndef CORE_H
#define CORE_H

// ------   Primitive Typedefinitions

typedef unsigned    char            u8;
typedef unsigned    short           u16;
typedef unsigned    int             u32;
typedef unsigned    long long       u64;
typedef signed      char            i8;
typedef signed      short           i16;
typedef signed      int             i32;
typedef signed      long long       i64;
typedef             float           f32;
typedef             double          f64;

typedef u64 uint64_t;
typedef u32 uint32_t;
typedef u16 uint16_t;
typedef u64 uintptr_t;
typedef u32 size_t;
typedef u32 bool;

// ------   Functions and Definitions

i32             main                (i32 argc, u8 **argv);

void *          brk                 (void *address);
bool            memcheck            (void *ptr, size_t size);

extern i32      syscall_read        (i32 fd, u8 *buf, i32 count);
extern i32      syscall_write       (i32 fd, u8 *buf, i32 count);
extern i32      syscall_open        (u8 *filename, i32 flags, i32 mode);
extern i32      syscall_close       (i32 fd);
extern void *   syscall_mmap        (u8 *addr, u32 length, u32 prot, u32 flags, i32 fd, u32 offset);
extern void *   syscall_munmap      (u8 *addr, u32 length);
extern void     syscall_execve      (u8 *filename, u8 **argv, u8 **envp);
extern void     syscall_exit        (i32 code);
extern i32      syscall_lseek       (i32 fd, i64 offset, u32 whence);

#define PROT_READ                   0x1  // Page can be read
#define PROT_WRITE                  0x2  // Page can be written
#define MAP_PRIVATE                 0x02  // Changes are private
#define MAP_ANONYMOUS               0x20  // Anonymous mapping (not backed by a file)
#define MAP_FIXED                   0x10  // Interpret addr exactly
#define MAP_FAILED                  ((void *)-1)

// ------   Internal Typedefinitions

typedef struct  { void *entry; int size; } brk_bump;

// ------   Internal Functions

i32             __init_nextwsp      (u8 *string);
void            __init_c            (i32 argc, u8 *head, uintptr_t stack_top, uintptr_t stack_base);
extern uintptr_t __getsp            (void);

void *          __brk_mmap_wrapper  (void *entry, size_t size);
bool            __brk_alloc         (void *entry, brk_bump *map, size_t asksize);
bool            __brk_unalloc       (brk_bump *map);

// ------   Global Variables

#define true 1
#define false 0
#define NULL 0
#define SYSCALL_READ    0
#define SYSCALL_WRITE   1
#define SYSCALL_OPEN    2
#define SYSCALL_CLOSE   3
#define SYSCALL_FSTAT   5
#define SYSCALL_EXECVE  59
#define SYSCALL_EXIT    60
#define STDIN           0
#define STDOUT          1
#define STDERR          2

// ------   Private Variables

#define __brk_startaddr 0x10000
static uintptr_t   __brk_stackbp;
static uintptr_t   __brk_stacksp;
void *__brk_startpoint = (void *)__brk_startaddr;
void *__brk_virtbreak  = (void *)__brk_startaddr;
void *__brk_realbreak  = (void *)__brk_startaddr;
static brk_bump __brk_maps[1024];
static int __brk_mapcount = 0;

// ------   Implementations

i32             __init_nextwsp      (u8 *string)
{ 
    i32 i=0; while(string[i]!=' ' && string[i]!=0) { i++; } 
    return i; 
}

void            __init_c            (i32 argc, u8 *head, uintptr_t stack_top, uintptr_t stack_base)
{
    i32 i, len;
    u8 *argv[argc];
    // Process argv
    for(i = 0; i < argc; i++) {
        len = __init_nextwsp(head);
        argv[i] = head;
        head[len] = 0;
        head += len + 1;
    }
    __brk_stackbp = stack_base;
    __brk_stacksp = stack_top;
    main(argc, argv);
    syscall_exit(SYSCALL_EXIT);
}

void *          __brk_mmap_wrapper  (void *entry, size_t size)
{
    size_t mapsize = (size + 4095) & ~4095;
    static int fd = -1; static int file_offset = 0;
    return syscall_mmap (
        entry, 
        mapsize, 
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED,
        fd, file_offset
    );
}

bool            __brk_alloc         (void *entry, brk_bump *map, size_t asksize)
{
    int fd = -1; int file_offset = 0;
    size_t mapsize = (asksize + 4095) & ~4095;
    void *entrypoint = __brk_mmap_wrapper(__brk_realbreak, mapsize);
    if (entrypoint == MAP_FAILED) return false;
    __brk_realbreak += mapsize;
    map->entry = entrypoint; 
    map->size = mapsize;
    __brk_mapcount++;
    return true;
}

bool            __brk_unalloc       (brk_bump *map)
{
    if (syscall_munmap(map->entry, map->size) != 0) { return false; }
    __brk_realbreak -= map->size;
    __brk_mapcount--;
    return true;
}

void *          brk                 (void *address)
{
    if (address == NULL) { return __brk_virtbreak; }
    if ((uintptr_t)address < (uintptr_t)__brk_startpoint) { return NULL; }
    // Initialize bump allocator
    if (__brk_mapcount == 0) {
        size_t memsize = (uintptr_t)address - (uintptr_t)__brk_startpoint;
        bool ok = __brk_alloc(__brk_startpoint, &__brk_maps[0], memsize );
        if (!ok) { return NULL; }
    }
    // Allocate memory
    else if ((uintptr_t)address > (uintptr_t)__brk_realbreak) {
        brk_bump *map = &__brk_maps[__brk_mapcount-1];
        uintptr_t trueBreak = (uintptr_t)map->entry + map->size;
        size_t memsize = (uintptr_t)address - trueBreak;
        if (memsize > 0 ) {
            bool ok = __brk_alloc((void *)trueBreak, map, memsize);
            if (!ok) { return NULL; }
        }
    }
    // Deallocate memory
    else if ( __brk_mapcount > 1 ) {
        // Figure out where the breakpoint _would_ be if we deallocated our current map
        brk_bump *map = &__brk_maps[__brk_mapcount-1];
        brk_bump *backmap = &__brk_maps[__brk_mapcount-2];
        uintptr_t backBreak = (uintptr_t)backmap->entry + backmap->size;
        // Then deallocate memory while its true we'd still be in bounds
        while ( (uintptr_t)address < (uintptr_t)backBreak ) {
            bool ok = __brk_unalloc(map); if (!ok) { return NULL; }
            map = &__brk_maps[__brk_mapcount-1];
            backmap = &__brk_maps[__brk_mapcount-2];
            backBreak = (uintptr_t)backmap->entry + backmap->size;
        }
    }
    // Update the breakpoint before returning
    __brk_virtbreak = address;
    return __brk_virtbreak;
}

bool            memcheck            (void *ptr, size_t size) 
{
    uintptr_t address = (uintptr_t)ptr;
    __brk_stacksp = __getsp();
    if (address >= __brk_stacksp && address < __brk_stackbp) {  return true; }
    if (address >= __brk_startaddr && address + size <= (uintptr_t)__brk_virtbreak) { 
        return true; 
    }
    return false;
}

#endif