#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
int msqid; // IPC descriptor for the message queue
char pathname[] = "lab12-b.c"; // The file name used to generate the key.
// A file with this name must exist in the current directory.
key_t key; // IPC key
int i, len, maxlen; // Cycle counter and the length of the informative part of the message

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
//
// Trying to get access by key to the message queue, if it exists,
// or create it, with read & write access for all users.
//
if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
printf("Can\'t get msqid\n");
exit(-1);
}



//Get float

float number;
printf("Введите число с плавающей точкой\n");
while (1)
{
if (scanf("%f", &number) != 1) {
printf("Введено некорректное число. Попробуйте еще раз\n");
continue;
}
break;
}

/* Send information */
maxlen = sizeof(inner);

mybuf.mtype = 1;
mybuf.content.num = number;
long pid = getpid();
mybuf.content.pid = pid;

//
// Send the message. If there is an error,
// report it and delete the message queue from the system.
//
if (msgsnd(msqid, (struct mymsgbuf*)&mybuf, maxlen, 0) < 0) {
printf("Can\'t send message to queue\n");
msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
exit(-1);
}

//ждем возвращения сообщения типа pid
if ((len = msgrcv(msqid, (struct mymsgbuf*)&mybuf, maxlen, pid, 0)) < 0) {
msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
printf("Can\'t receive message from queue\n");
exit(-1);
}

printf("get message type = %ld, info = %f pid = %ld\n", mybuf.mtype, mybuf.content.num, mybuf.content.pid);

return 0;
}