#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

//Recipient
int num = 0;
int bitcount = 0;
int bit = 1;
int s_pid = -1;


void checkend() {
if (bitcount == 32) {
printf("Receiver number: %d\n", num);
exit(0);
}
}

void bit_handler(int sig) {
if (bit == 0) { // проверка побитово
bit = 1;
}
else {
bit *= 2;
}
bitcount++;

if (sig == SIGUSR1) { //если вызван сигнал бит=1
num = num | bit;
}

kill(s_pid, SIGUSR1);//вызываем сигнал у процесса под sender_pid
checkend();

}


int main() {
int r_pid = getpid();

//переопределяем сигналы, которые будем вызывать
(void)signal(SIGUSR1, bit_handler); //бит = 1
(void)signal(SIGUSR2, bit_handler); //бит = 0

printf("This process(Receiver) PID: %d\n", r_pid);

printf("Set the Sender PID:\n");
scanf("%d", &s_pid);
if (s_pid <= 0) {
exit(0);
}

while (1); //прога не должна завершаться

return 0;
}