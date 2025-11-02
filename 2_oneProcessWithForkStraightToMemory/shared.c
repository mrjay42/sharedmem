#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

struct MyData {
    int counter;
    char msg[64];
};

int main() {
    // allocate shared mem (no file, no disk)
    struct MyData *data = mmap(NULL, sizeof(struct MyData),
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS,
                               -1, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) {
        // --- child process ---
        sleep(1); // wait a bit for parent to write something
        printf("Child: counter=%d, msg='%s'\n", data->counter, data->msg);

        // change it to prove we see the same mem
        data->counter += 100;
        strcpy(data->msg, "Yo from child process!");

        printf("Child: updated counter=%d, msg='%s'\n", data->counter, data->msg);
    } else {
        // --- parent process ---
        data->counter = 42;
        strcpy(data->msg, "Hello from parent side!");
        printf("Parent: wrote counter=%d, msg='%s'\n", data->counter, data->msg);

        // wait for child to mess with it
        wait(NULL);
        printf("Parent after child: counter=%d, msg='%s'\n", data->counter, data->msg);
    }

    munmap(data, sizeof(struct MyData));
    return 0;
}
