#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid, ppid;

    pid = getpid();
    ppid = getppid();

    printf("Before forked Pid = %d, Ppid = %d\n", (int) pid, (int) ppid);

    pid = fork(); //когда делаем форк создается копия процесса 
    //и теперь дочерний процесс указывает на другого родителя 

    switch(pid)
    { 
	case(-1):
	    printf("Something goes wrong.");
	    exit(1);
	case(0):
	    printf("Child Process Pid = %d, Ppid = %d\n", (int)getpid(), (int)getppid());
	    printf("Call for l3\n ");
	    if(execl("./compiledl3.out","./compiledl3.out",
	    "Argument1","Argument2", NULL) == -1)
	    {
		printf("\t Call goes wrong");
		exit(1);
	    }
	default:
	    printf("Parent Process ChildPid = %d, MyPid = %d\n", (int)pid, (int)getppid());
    }
    return 0;



}

