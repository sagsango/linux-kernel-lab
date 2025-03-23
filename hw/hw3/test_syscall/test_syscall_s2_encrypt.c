#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    char *input_str = NULL;
    int key = 0;

    int opt;
    while ((opt = getopt(argc, argv, "s:k:")) != -1) {
        switch (opt) {
            case 's':
                input_str = optarg;
                break;
            case 'k':
                key = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -s <string> -k <key>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (input_str == NULL || key == 0) {
        fprintf(stderr, "Invalid input\n");
        exit(EXIT_FAILURE);
    }

    int ret = syscall(548, input_str, key);
    printf("sys_s2_encrypt returned: %d\n", ret);

    return 0;
}

