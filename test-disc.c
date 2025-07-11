#include <stdio.h>
/*
 * Copyright 2025 Peter Chubb
 * SPDX-License-Identifier: GPL-3.0
 * 
 * Test a disc for true capacity and fake flash
 * Writes 64k chunks each with its chunk numbe in it until the write failes
 * (which shouldn't happen until the end of the disc), then reads
 * the chunks from the beginning
 * and compares the chunk number with what's expected.
 * Fake flash SSDs reuse the same flash in multiple regions, so
 * if you read a higher number than you expect, it's because the number
 * has been overwritten.
 *
 * Warning: this will destroy all the data on the disc.
 */
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

char buf[64 * 1024] __attribute__ ((__aligned__ (4096)));

#ifndef O_DIRECT
#  define O_DIRECT __O_DIRECT
#endif

int testit(int fd)
{
    unsigned long blockno = 0;
    unsigned long maxblock;

    for(;;) {
        *(unsigned long *)buf = blockno++;
        if (write(fd, buf, sizeof buf) != sizeof buf) {
            fprintf(stderr, "Write failed at block %lu, %s\n", blockno-1, strerror(errno));
            maxblock = blockno;
            break;
        }
        if (blockno % 1024 == 0)
            fprintf(stderr, "Written %lu\r", blockno);
    }

    blockno = 0;
    lseek(fd, 0, SEEK_SET);
    while (blockno <= maxblock) {
        if (blockno % 1024 == 0)
            fprintf(stderr, "Read %lu\r", blockno);
        if (read(fd, buf, sizeof buf) != sizeof buf) {
            fprintf(stderr, "Read failed at block %lu, %s\n",
                    blockno, strerror(errno));
            return 1;
        }
        if (blockno != *(unsigned long *)buf) {
            fprintf(stderr, "Expected block %lu got block %lu\n",
                    blockno, *(unsigned long *)buf);
            return 1;
        }
        blockno++;
    }
    return 0;
}


int main(int ac, const char * const *av)
{
    int fd;
    if (ac != 2) {
        fprintf(stderr, "Usage: %s diskname\n", av[0]);
        return 1;
    }
    
    fd = open(av[1], O_SYNC|O_RDWR|O_DIRECT);
    if (fd == -1) {
        perror(av[1]);
        return 1;
    }
    return testit(fd);
}
