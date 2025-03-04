#define PROT_READ   0x1  // Page can be read
#define PROT_WRITE  0x2  // Page can be written
#define MAP_PRIVATE   0x02  // Changes are private
#define MAP_ANONYMOUS 0x20  // Anonymous mapping (not backed by a file)
#define MAP_FIXED     0x10  // Interpret addr exactly
#define MAP_FAILED ((void *)-1)

typedef struct { void *entry; int size; } brk_bump;
#define __brk_startaddr 0x10000
void *__brk_startpoint = (void *)__brk_startaddr;
void *__brk_virtbreak  = (void *)__brk_startaddr;
void *__brk_realbreak  = (void *)__brk_startaddr;
brk_bump __brk_maps[1024]; int __brk_mapcount = 0;

void *__brk_mmap_wrapper(void *entry, size_t size){
    size_t mapsize = (size + 4095) & ~4095;
    static int fd = -1; static int file_offset = 0;
    return __syscall_mmap (
        entry, 
        mapsize, 
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED,
        fd, file_offset
    );
}

bool __brk_alloc(void *entry, brk_bump *map, size_t asksize) {
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

bool __brk_unalloc(brk_bump *map) {
    if (__syscall_munmap(map->entry, map->size) != 0) { return false; }
    __brk_realbreak -= map->size;
    __brk_mapcount--;
    return true;
}

void * brk(void *address){
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

extern uintptr_t __getsp();

bool memcheck(void *ptr, size_t size) {
    uintptr_t address = (uintptr_t)ptr;
    __brk_stacksp = __getsp();
    if (address >= __brk_stacksp && address < __brk_stackbp) {  return true; }
    if (address >= __brk_startaddr && address + size <= (uintptr_t)__brk_virtbreak) { 
        return true; 
    }
    return false;
}