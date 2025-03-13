i64 file_size(char *filename) {
    const int OFFSET = 0; const int SEEK_END = 2;
    i32 fd = open(filename, 0, 0);
    if ( fd == -1 ) { return -1; }
    i64 size = lseek(fd, OFFSET, SEEK_END);
    close(fd);
    return size;
}

int file_read(char *filename, char *buffer, i64 max) {
    const i32 flags_ro = 0; const i32 modes = 0;
    i64 filesize = file_size(filename);
    if ( filesize <= 0 ) { print("file_read: file nil sized\n"); return -1; }
    if ( filesize >= max ) { filesize = max-1; } // keep within bounds 
    u32 fd = open(filename, flags_ro, modes); if ( fd == -1 ) { return -2; }
    i64 total_read = 0; // if we can't do it in one pass, keep trying
    while (total_read < filesize) {
        i32 rsize = read(fd, buffer + total_read, filesize - total_read); 
        if (rsize <= 0) {  close(fd); return -3; }
        total_read += rsize;
    }   close(fd); *((char *)buffer + filesize) = '\0'; return total_read;
}

int file_write(char *filename, char *content, i64 size) {
    const i32 flags_ctw = 0x201 | 0x401 | 0x40; /* O_CREAT | O_WRONLY | O_TRUNC */
    const i32 modes = 0x1B6; /* 0666 permissions */
    i32 fd = open(filename, flags_ctw, modes);
    if (fd < 0) { return -1; }
    i64 total_written = 0; // just in case we can't write everything in one pass
    while (total_written < size) {
        i32 wsize = write(fd, content + total_written, size - total_written);
        if (wsize <= 0) {  close(fd); return -2; }   
        total_written += wsize;
    }   close(fd); return total_written;
}