#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid, ppid;

    int a = 0;

    pid = getpid();
    ppid = getppid();

    printf("Before forked Pid = %d, Ppid = %d, Res = %d\n", (int) pid, (int) ppid, a);

    fork(); //когда делаем форк создается копия процесса 
    //и теперь дочерний процесс указывает на другого родителя 

    a = a + 1; 
    pid = getpid();
    ppid = getppid();

    printf("Pid = %d, Ppid = %d, Res = %d\n", (int) pid, (int) ppid, a);

    return 0;



}