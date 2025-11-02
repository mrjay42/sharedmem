#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

struct MyData {
    int counter;
    char msg[64];
};

int main() {
    const char *filename = "/tmp/mmap_share.bin";
    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    // make sure the file is big enuf for our struct
    ftruncate(fd, sizeof(struct MyData));

    // map it into memory
    struct MyData *data = mmap(NULL, sizeof(struct MyData),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    close(fd); // fd not needed anymore after mmap

    // write something in shared mem
    data->counter = 42;
    strcpy(data->msg, "Hello from writer side!");

    printf("Writer: wrote counter=%d, msg='%s'\n", data->counter, data->msg);

    // keep process alive a bit so reader can open
    printf("sleeping 10 sec...\n");
    sleep(10);

    munmap(data, sizeof(struct MyData));
    return 0;
}
