#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

main()
{
    printf("ArifkhanovAzamatkhan BSE198/nUid: %d\tGid:%d",getuid(),getgid());
    return 0;
}