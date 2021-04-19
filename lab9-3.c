#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <string.h>

int main() {

int fd[2], result;

size_t size;
char resstring[20];
char temp[20];
char nchar[3];

key_t key;
int semid;
struct sembuf mybuf;
char pathname[] = "lab9-3.c";

if (pipe(fd) < 0) { //создаю трубу для передачи данных
printf("Can\'t open pipe\n");
exit(-1);
}

if ((key = ftok(pathname, 0)) < 0) { //создаю ключ для семафора
printf("Can\'t generate semKey\n");
exit(-1);
}

if (((semid = semget(key, 0, 0)) == -1)) {//если семафор по ключу уже есть - беру
// если нет — создаю
if ((semid = semget(key, 1, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
printf("Can\'t create or get semaphore\n");
exit(-1);
}
}

mybuf.sem_num = 0;
mybuf.sem_op = 0;
mybuf.sem_flg = 0;
semop(semid, &mybuf, 1);

int N;
scanf("%d", &N); //ввод количества итераций

if ((result = fork()) < 0) {
printf("Can\'t fork child\n");
exit(-1); //форкаю
}
else if (result > 0) {

printf("Open Parent Process\n");


for (int i = 1; i <= N; i++)
{

mybuf.sem_op = 1;
semop(semid, &mybuf, 1); //Блокирую семафор, значение 1

sprintf(nchar, "%d", i); //преобразую число в массив чаров
strcpy(temp, "Operation p "); //заполняю массив чаров строкой
strcat(temp, nchar); //образую строку из тескста и числа

size = write(fd[1], temp, 20);

if (size != 20) {
printf("Can\'t write all string to pipe\n");
exit(-1);
}

printf("Parent Process, Written: %s\n", temp);

mybuf.sem_op = 2;
semop(semid, &mybuf,1); //увеличиваю на 2, значение = 3. Ребенок может двигать

mybuf.sem_op = 0;
semop(semid, &mybuf, 1); //родитель ждет открытия семафора (когда ребенок сделает параметр нулем)

size = read(fd[0], resstring, 20); 
if (size < 0) {
printf("Can\'t read string from pipe\n");
exit(-1);
}
//родитель читает и идет на следующую итерацию
printf("Parent, Read: %s\n", resstring);

}

if (close(fd[0]) < 0) {
printf("child: Can\'t close reading side of pipe\n");
exit(-1);
}

}
else {

printf("Open Child Process\n");

for (int i = 1; i <= N; i++)
{
mybuf.sem_op = -2;
semop(semid, &mybuf, 1); //ребенок ждет чтобы значение семафора стало >2, затем он продолжает уменьшив его
//значение семафора = 1
size = read(fd[0], resstring, 20);
if (size < 0) {
printf("Can\'t read string from pipe\n");
exit(-1);
}
printf("Child Process, Read: %s\n", resstring);

sprintf(nchar, "%d", i); //преобразую число в массив чаров
strcpy(temp, "Operation c "); //заполняю массив чаров строкой
strcat(temp, nchar); //образую строку из тескста и числа

size = write(fd[1], temp, 20);
if (size != 20) {
printf("Can\'t write all string to pipe\n");
exit(-1);
}

printf("Child Process, Written: %s\n",temp);

mybuf.sem_op = -1;
semop(semid, &mybuf, 1); //делаю семафор равным 0 открывая чтение родителем
}
if (close(fd[1]) < 0) {
printf("child: Can\'t close writing side of pipe\n");
exit(-1);
}
}
return 0;
}