#include <stdio.h>
#include <unistd.h>

int main( int argc, char* argv[], char* envp[]) {

char** elem;

for( elem = argv; *elem; ++elem)
{
    printf("Arguments argv element: %s\n", *elem);
}
for( elem = envp; *elem; ++elem)
{
    printf("Environment envp element: %s\n", *elem);
}

return 0;
}



