i64 file_size(char *filename) {
    const int OFFSET = 0; const int SEEK_END = 2;
    i32 fd = open(filename, 0, 0);
    if ( fd == -1 ) { return -1; }
    i64 size = lseek(fd, OFFSET, SEEK_END);
    close(fd);
    return size;
}

char *file_read(char *filename) {
    const i32 flags_rw = 2; const i32 modes = 0;
    i64 filesize = file_size(filename); if ( filesize <= 0 ) { return NULL; }
    void *buffer = brk(NULL); brk(buffer + filesize + 1);
    u32 fd = open(filename, flags_rw, modes); if ( fd == -1 ) { return NULL; }
    i32 rsize = read(fd, buffer, filesize); if ( rsize != filesize ) { close(fd); brk(buffer); return NULL; }
    *((char *)buffer + filesize) = '\0';
    close(fd); return buffer;
}

bool file_write(char *filename, char *content, u64 size) {
    const i32 flags_rw = 2; const i32 modes = 0;
    u32 fd = open(filename, flags_rw, modes); if ( fd == -1 ) { return false; }
    i32 wsize = write(fd, content, size); if ( wsize <= 0 ) { close(fd); return false; }
    close(fd); return true;
}