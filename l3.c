#include <stdio.h>
#include <unistd.h>

int main( int argc, char* argv[], char* envp[]) {

char** elem;
int cnt = 1 ;

for( elem = argv; *elem; ++elem)
{
    printf("Arguments argv element %d: %s\n",cnt, *elem);
    cnt++;
}
cnt = 1;
for( elem = envp; *elem; ++elem)
{
    printf("Environment envp element %d: %s\n",cnt, *elem);
    cnt++;
}

return 0;
}



