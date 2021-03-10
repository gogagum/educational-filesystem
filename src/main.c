#include <stdio.h>
#include "fs.h"

int 
main() 
{
    printf("%li\n", sizeof(struct inode));
    printf("%li\n", sizeof(time_t));
    return 0;
}
