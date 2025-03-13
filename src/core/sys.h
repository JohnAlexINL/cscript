uintptr_t __brk_stackbp;
uintptr_t __brk_stacksp;

extern i32      __syscall_read      (u32 fd, u8 *buf, i32 count);
extern i32      __syscall_write     (u32 fd, u8 *buf, i32 count);
extern i32      __syscall_open      (u8 *filename, i32 flags, i32 mode);
extern i32      __syscall_close     (u32 fd);
extern void *   __syscall_mmap      (u8 *addr, u32 length, u32 prot, u32 flags, u32 fd, u32 offset);
extern void *   __syscall_munmap    (u8 *addr, u32 length);
extern void     __syscall_execve    (u8 *filename, u8 **argv, u8 **envp);
extern void     __syscall_exit      (i32 code);
extern i32      __syscall_lseek     (u32 fd, i64 offset, u32 whence);