#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
int fd, result;

size_t size;
char resstring[14];
char name[] = "fifa.fifo";

(void)umask(0);

if (mknod(name, S_IFIFO | 0666, 0) < 0) {
printf("Can\'t create FIFO\n");
exit(-1); //создаю файл для fifo
}

if ((result = fork()) < 0) {
printf("Can\t fork child\n");
exit(-1); //форкаю
} else if (result > 0) {

//открываю файл фифо для записи
if ((fd = open(name, O_WRONLY)) < 0) {
printf("Can\'t open FIFO for writting\n");
exit(-1);
}
//вписываю данные
size = write(fd, "BRUUUUUUUUUUUU!", 14);

if (size != 14) {
printf("Can\'t write all string to FIFO\n");
exit(-1);
}
//закрываю поток
if (close(fd) < 0) {
printf("parent: Can\'t close FIFO\n"); exit(-1);
}

printf("Parent exit\n");

} else {
//ребенок открывает поток для чтения
if ((fd = open(name, O_RDONLY)) < 0) {
printf("Can\'t open FIFO for reading\n");
exit(-1);
}
//считывает
size = read(fd, resstring, 14);

if (size < 0) {
printf("Can\'t read string from FIFO\n");
exit(-1);
}

printf("Child exit, resstring:%s\n", resstring);

if (close(fd) < 0) {
printf("child: Can\'t close FIFO\n"); exit(-1);
}

}
return 0;
}