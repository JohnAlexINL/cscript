i64 file_size(char *filename) {
    const int OFFSET = 0; const int SEEK_END = 2;
    i32 fd = open(filename, 0, 0);
    if ( fd == -1 ) { return -1; }
    i64 size = lseek(fd, OFFSET, SEEK_END);
    close(fd);
    return size;
}

char *file_read(char *filename, char *buffer, i64 max) {
    const i32 flags_rw = 2; const i32 modes = 0;
    i64 filesize = file_size(filename);
    if ( filesize <= 0 ) { print("file_read: file nil sized\n"); return NULL; }
    if ( filesize >= max ) { print("file_read: file smaller than buffer\n"); return NULL; }
    u32 fd = open(filename, flags_rw, modes); if ( fd == -1 ) { return NULL; }
    i32 rsize = read(fd, buffer, filesize); if ( rsize != filesize ) { close(fd); return NULL; }
    *((char *)buffer + filesize) = '\0';
    close(fd); return buffer;
}

bool file_write(char *filename, char *content, i64 size) {
    const i32 flags_ctw = 0x201 | 0x401 | 0x40; /* O_CREAT | O_WRONLY | O_TRUNC */
    const i32 modes = 0x1B6; /* 0666 permissions */
    i32 fd = open(filename, flags_ctw, modes);
    if (fd < 0) { return false; }
    i64 total_written = 0; // just in case we can't write everything in one pass
    while (total_written < size) {
        i32 wsize = write(fd, content + total_written, size - total_written);
        if (wsize <= 0) {  close(fd); return false; }   
        total_written += wsize;
    }   close(fd); return true;
}