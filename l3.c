#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
int fd[2];

size_t size;

if (pipe2(fd,04000) < 0) { //создаем трубу с установленными параметрами для выхода
printf("Can\'t open pipe\n");
exit(-1);
}

if (close(fd[0]) < 0) { // закрываем выход
printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
}

while (write(fd[1], "1", 1) == 1) //пока запись идет без проблем — заполняем стек
{
size += 1;
printf("Pipe size: %d bytes\n", size);
}

return 0;

}