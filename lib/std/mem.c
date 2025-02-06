bool memcpy(void *dest, void *src, u64 size) {
    u64 i;
    for(i=0;i<size;i++){
        ((u8 *)dest)[i] = ((u8 *)src)[i];
    }
}