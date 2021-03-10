#include <stdio.h>
#include "../src/fs.h"

int
main()
{
    printf("Inode struct size: %li\n", sizeof(struct inode));
    printf("time_t size: %li\n", sizeof(time_t));
    printf("Data struct size (just to see): %li\n", sizeof(struct data));
    return 0;
}
