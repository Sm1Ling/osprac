#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

int num = 0;
int bit = 1;
int bitcounter = 0;
int r_pid = -1;

void nextbit() {
if (bit == 0) { // проверка побитово
bit = 1;
}
else {
bit *= 2;
}
bitcounter++;

if (bit & num) { //если у числа на текущей позиции стоит единичный бит
kill(r_pid, SIGUSR1); //вызываю сигнал SIGUSR1 у указанного процесса
}
else { //если у числа на текущей позиции стоит единичный бит
kill(r_pid, SIGUSR2);
}
}

void is_the_end(int sig) {
if (bitcounter < 32) {
nextbit();
}
else {
exit(0);//прекращаем прогу
}
}


int main() {
int pid;

pid = getpid();
printf("This process(sender) PID: %d\n", (int)pid);

printf("Set PID of Receiver: \n");
scanf("%d", &r_pid);
if (r_pid <= 0) {
exit(0);
}

printf("Set the sending number: \n");
scanf("%d", &num);

(void)signal(SIGUSR1, is_the_end); //так мы сигналом будем проверять надо ли смотреть следующий бит

nextbit();

while (1); //программа не должна завершаться

return 0;
}