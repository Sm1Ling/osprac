#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <string.h>

int main()
{
int fd[2], result;

size_t size;
char resstring[30];
char numhold[3];
char temp[30];


key_t key;

struct sembuf mybuf;
int semid;

if ((key = ftok("lab9-3.c", 0)) < 0) {
printf("Can\'t generate key\n");
exit(-1);
}

if (pipe2(fd, O_NONBLOCK) < 0) {
printf("Can\'t open pipe\n");
exit(-1);
}

if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
printf("Can\'t create semaphore set\n");
exit(-1);
}

mybuf.sem_num = 0;
mybuf.sem_op = 0;
mybuf.sem_flg = 0;

int N;
scanf("%d", &N);

if ((result = fork()) < 0) {
printf("Can\'t fork child\n");
exit(-1); //форкаю
}

if (result > 0) {

//открываю файл фифо для записи
printf("Open Parent Process\n");

mybuf.sem_op = -1;
semop(semid, &mybuf, 1); // блокирую семафор


for (int i = 1; i <= N; i++) {

mybuf.sem_op = 2;
semop(semid, &mybuf, 1); // увеличиваю значение семафора на 2, он все еще заблокирован

sprintf(numhold, "%d", i); //преобразую число в массив чаров
strcpy(temp, "Operation p "); //заполняю массив чаров строкой
strcat(temp, numhold); //образую строку из тескста и числа
size = write(fd, temp, 30); //осуществляю запись

if (size != 30) {
printf("Can\'t write all string to FIFO\n");
exit(-1);
}

printf("Parent Process, Written: %s\n", temp);

mybuf.sem_op = 0;
semop(semid, &mybuf, 1); //родитель ждет открытия семафора (когда ребенок сделает параметр нулем)

size = read(fd, resstring, 30);

if (size < 0) {
printf("Can\'t read string from FIFO\n");
exit(-1);
}

printf("Parent Process, Read: %s\n", resstring);

mybuf.sem_op = -1;
semop(semid, &mybuf, 1); //после того как родитель отработал надо блокнуть семафор, чтоб ребенок не запустился после A(2) на след итерации

}

printf("Parent exit\n");
}
else {

printf("Child Process opened\n");

for (int i = 1; i <= N; i++)
{

mybuf.sem_op = -2;
semop(semid, &mybuf, 1); //ребенок ждет чтобы значение семафора стало положительным, затем он блокает его под себя

size = read(fd, resstring, 30);

if (size < 0) {
printf("Can\'t read string from FIFO\n");
exit(-1);
}

printf("Child Process, resstring:%s\n", resstring);


sprintf(numhold, "%d", i); //преобразую число в массив чаров
strcpy(temp, "Operation c "); //заполняю массив чаров строкой
strcat(temp, numhold); //образую строку из тескста и числа
size = write(fd, temp, 30); //осуществляю запись

if (size != 30) {
printf("Can\'t write all string to FIFO\n");
exit(-1);
}

printf("Child Process, Written: %s\n", temp);

mybuf.sem_op = 1;
semop(semid, &mybuf, 1); //открываю семафор чтоб родитель мог считать
}

printf("Child exit");

}

return 0;
}