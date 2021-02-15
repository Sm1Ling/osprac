#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
  int     fd;
  size_t  size = 4096;
  size_t bytesread;
  char    buffer[4096];
 
  (void)umask(0); //устанавливаю маску для open без ограничений
 
  if ((fd = open("file1.txt", O_RDONLY, 0666)) < 0) { //присваиваю дескриптору открытие файла на чтение
// с полным перечнем прав доступа
    printf("Can\'t open file\n");
    exit(-1);
  }
 
 
  if ((bytesread = read(fd,buffer,size)) < 0) { //записываю в буффер из потока чтения по установленному
// дескриптору указанное количество байт из файла.
    printf("Can\'t read all string\n");
    exit(-1);
  }
 
  printf("Readed from file: %s", buffer); //вывод что считал.
 
  if (close(fd) < 0) { //если закрытие потока падает -- вывести ошибку
    printf("Can\'t close file\n");
    exit(-1);
  }
 
  return 0;
}