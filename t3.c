#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int curpid;

void my_handler(int nsig) {
int status;
pid_t pid;

int temp = getpid();
for (int i = 0; i < 10000 * (temp - curpid); i++); //ожидание, чтоб процессы не склеивались

if ((pid = waitpid(-1, &status, 0)) < 0) { //на этой точке мы узнаем какой процесс прошелся по этой строке кода
printf("Some error on waitpid errno = %d\n", errno);
} else {
if ((status & 0xff) == 0) {
printf("Process %d was exited with status %d\n", pid, status >> 8);
return;
} else if ((status & 0xff00) == 0) {
printf("Process %d killed by signal %d %s\n", pid, status &0x7f,
(status & 0x80) ? "with core file" : "without core file");
return;
}
}
}

int main(void) {
pid_t pid;
int i,j;

curpid = getpid();

(void) signal(SIGCHLD, my_handler);

for (i=0; i<10; i++) {
if ((pid = fork()) < 0) {
printf("Can\'t fork child 1\n");
exit(1);
} else if (pid == 0) {
/* Child */
for (j=1; j < 10000000; j++);
exit(200 + i); //убивая этот процесс процесс-родитель будет выводить инфу из-за переопределенного SIGCHLD
}
}

while(1);
return 0;
}