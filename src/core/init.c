i32 main (i32 argc, u8 **argv);

i32 __init_nextwsp(u8 *string){ 
    i32 i=0; while(string[i]!=' ' && string[i]!=0) { i++; } 
    return i; 
}

void __init_c(i32 argc, u8 *head, uintptr_t stack_top, uintptr_t stack_base) {
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
    __syscall_exit(SYSCALL_EXIT);
}
