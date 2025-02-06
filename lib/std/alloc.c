/*
    The start value will be somewhat arbitrary
    since we aren't basing it off of the executable section,
    so we're just going to pick a good looking value
    and request that mmap gives us something tasty
*/

#define __MMAP_PAGE_FAILED ((void *)-1)
#define __MMAP_PROT_NONE  0x0      /* Page cannot be accessed */
#define __MMAP_PROT_READ  0x1      /* Page can be read */
#define __MMAP_PROT_WRITE 0x2      /* Page can be written */
#define __MMAP_PROT_EXEC  0x4      /* Page can be executed */
#define __MMAP_FLAG_ANONYMOUS 0x20 /* Page is not backed by any file */
#define __MMAP_FLAG_FIXED 0x10     /* Page is fixed in place */
#define __alloc_prot    __MMAP_PROT_WRITE       + __MMAP_PROT_READ
#define __alloc_flags   __MMAP_FLAG_ANONYMOUS   + __MMAP_FLAG_FIXED
#define __alloc_pagesize 0x1000
#define __alloc_requested_start 0x7C000000
void *__alloc_virtual_start = NULL;
u64 __alloc_totalpages =    0;
u64 __alloc_totalsize =     0;

void *__alloc(u64 heapsize);
void *__dealloc(u64 heapsize);

void *__alloc_newpage(){
    void *page = __syscall_mmap(
        __alloc_virtual_start + (__alloc_totalpages * __alloc_pagesize),
        __alloc_pagesize,
        __alloc_prot,
        __alloc_flags,
        -1,
        0
    );
    if(page == __MMAP_PAGE_FAILED){ return NULL; }
    __alloc_totalsize+=__alloc_pagesize;
    __alloc_totalpages++;
    return page;
}

void *__alloc(u64 heapsize) {
    // If asking for **LESS**, then we should call __dealloc
    if(heapsize < __alloc_totalsize){ return __dealloc(heapsize); }
    else if(heapsize == __alloc_totalsize){ return __alloc_virtual_start; }
    // Make sure heapsize is a multiple of the page size
    if(heapsize % __alloc_pagesize != 0){ heapsize += __alloc_pagesize - (heapsize % __alloc_pagesize); }
    // Figure out how many pages are being asked for
    u64 pages = (heapsize - __alloc_totalsize) / __alloc_pagesize;
    // Check if uninitialized
    if ( __alloc_virtual_start == NULL) {
        __alloc_virtual_start = (void *)__alloc_requested_start;
        void * ret = __alloc_newpage();
        if ( ret != NULL ) {
            __alloc_virtual_start = ret;
            pages--;
        }
        // If we can't get the page we asked for, then just ask for whatever
        else {
            ret = __syscall_mmap(
                NULL,
                __alloc_pagesize,
                __alloc_prot,
                __MMAP_FLAG_ANONYMOUS,
                -1,
                0
            );
            if ( ret != __MMAP_PAGE_FAILED ) { __alloc_virtual_start = ret; pages--; }
            // If we **STILL** failed, error out
            else { return NULL; }
        }
    }
    // Then ask for anything else we needed
    while ( pages > 0 ) {
        void * ret = __alloc_newpage();
        if ( ret == NULL ) { return NULL; }
        pages--;
    }
    return __alloc_virtual_start;
}

void *__dealloc(u64 heapsize) {
    // If asking for **MORE**, then we should call __alloc
    if(heapsize > __alloc_totalsize){ return __alloc(heapsize); }
    else if(heapsize == __alloc_totalsize){ return __alloc_virtual_start; }
    // Make sure heapsize is a multiple of the page size
    if(heapsize % __alloc_pagesize != 0){ heapsize += __alloc_pagesize - (heapsize % __alloc_pagesize); }
    // Figure out how many pages are being freed up
    u64 pages = (__alloc_totalsize - heapsize) / __alloc_pagesize;
    // Then start unmapping em one at a time
    while ( pages > 0 ) {
        void * ret = __syscall_munmap(
            __alloc_virtual_start + (__alloc_totalpages * __alloc_pagesize),
            __alloc_pagesize
        );
        if ( ret == __MMAP_PAGE_FAILED ) { return NULL; }
        __alloc_totalsize-=__alloc_pagesize;
        __alloc_totalpages--;
        pages--;
    }
    return __alloc_virtual_start;
}