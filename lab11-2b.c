#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
int msqid;
char pathname[]="lab11-2a.c";
key_t key;
int len, maxlen;

struct innerstruct
{
char mtext[64];
int num;
}inner;

struct mymsgbuf // Custom structure for the message
{
long mtype;
struct innerstruct content;
} mybuf;

if ((key = ftok(pathname,0)) < 0) {
printf("Can\'t generate key\n");
exit(-1);
}

if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
printf("Can\'t get msqid\n");
exit(-1);
}
maxlen = sizeof(inner);
while (1) {

if (( len = msgrcv(msqid, (struct mymsgbuf*) &mybuf, maxlen, 0, 0)) < 0) {
printf("Can\'t receive message from queue\n");
exit(-1);
}

if (mybuf.mtype == LAST_MESSAGE) {
printf("Got LAST MESSAGE from A\n");
break;
}

printf("message type = %ld, info = %d %s\n", mybuf.mtype, mybuf.content.num, mybuf.content.mtext);
}


printf("All messages from A were received\n");

for (int i = 1; i <= 5; i++) {
//
// Fill in the structure for the message and
// determine the length of the informative part.
//
mybuf.mtype = 2;
mybuf.content.num = i;
strcpy(mybuf.content.mtext, "Message from B");

//
// Send the message. If there is an error,
// report it and delete the message queue from the system.
//
if (msgsnd(msqid, (struct mymsgbuf*)&mybuf, maxlen, 0) < 0) {
printf("Can\'t send message to queue\n");
msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
exit(-1);
}
printf("Sent message: %d %s\n",mybuf.content.num, mybuf.content.mtext);
}

/* Send the last message */

mybuf.mtype = LAST_MESSAGE;
maxlen = 0;

if (msgsnd(msqid, (struct mymsgbuf*)&mybuf, maxlen, 0) < 0) {
printf("Can\'t send message to queue\n");
msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
exit(-1);
}



return 0;
}