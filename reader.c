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
    int fd = open(filename, O_RDWR, 0666);
    if (fd < 0) {
        perror("open failed");
        return 1;
    }

    struct MyData *data = mmap(NULL, sizeof(struct MyData),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    close(fd);

    printf("Reader: counter=%d, msg='%s'\n", data->counter, data->msg);

    // just to show u can modify it too
    data->counter += 1;
    strcpy(data->msg, "Yo I changed ur stuff 😎");

    printf("Reader: updated counter=%d, msg='%s'\n", data->counter, data->msg);

    munmap(data, sizeof(struct MyData));
    return 0;
}
