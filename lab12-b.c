#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
int msqid;
char pathname[] = "lab12-b.c";
key_t key;
int len, maxlen;

struct innerstruct
{
float num;
long pid;
}inner;

struct mymsgbuf // Custom structure for the message
{
long mtype;
struct innerstruct content;
} mybuf;

if ((key = ftok(pathname, 0)) < 0) {
printf("Can\'t generate key\n");
exit(-1);
}

if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
printf("Can\'t get msqid\n");
exit(-1);
}
maxlen = sizeof(inner);


while (1) {

if ((len = msgrcv(msqid, (struct mymsgbuf*)&mybuf, maxlen, 1, 0)) < 0) { //принимает сообщения типа 1
printf("Can\'t receive message from queue\n");
exit(-1);
}

if (mybuf.content.pid == 1) { //клиентские pid не могут быть единицей. Значит установим это как процесс-убийцу
printf("Server shut down\n");
msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
exit(0);
}


printf("got message type = %ld, info = %f pid = %ld\n", mybuf.mtype, mybuf.content.num, mybuf.content.pid);

mybuf.mtype = mybuf.content.pid;
mybuf.content.num = mybuf.content.num * mybuf.content.num;
mybuf.content.pid = getpid();

if (msgsnd(msqid, (struct mymsgbuf*)&mybuf, maxlen, 0) < 0) { //отправляю обратно квадрат числа
printf("Can\'t send message to queue\n");
msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
exit(-1);
}

printf("sent message type = %ld, info = %f pid = %ld\n", mybuf.mtype, mybuf.content.num, mybuf.content.pid);

}


return 0;
}
