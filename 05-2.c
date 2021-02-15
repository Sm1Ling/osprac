#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
 
int main()
{
  int     fd1[2], result;
  int     fd2[2]; //для организации двусторонней связи
 
  size_t size;
  char  resstring[12];
 
  if (pipe(fd1) < 0 || pipe(fd2) <0) { //создаем трубы для передачи данных
    printf("Can\'t open pipe\n");
    exit(-1);
  }
 
  result = fork(); //создаем подпроцесс
 
  //задача -- записать родителем, считать ребенком, записать ребенком, считать родителем
 
  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  }
  else if (result > 0) { //если процесс -- родитель
       
    printf("Parent enter\n");
 
    //родитель отрабатывает быстрее ребенка, т.к. сначала дадим ему право записать
    if (close(fd1[0]) < 0) {//закрываю вход трубы родителя, выход открыт (труба односторонняя)
      printf("parent: Can\'t close reading side of pipe to child\n");
      exit(-1);
    }
 
    size = write(fd1[1], "To the child", 12); //в выход трубы отправляем строку
 
    if (size != 12) {
      printf("Can\'t write all string to pipe to child\n");
      exit(-1);
    } 
 
    if (close(fd1[1]) < 0) { //обработали выход, закрываем его
      printf("parent: Can\'t close writing side of pipe to child\n"); 
      exit(-1);
    }
    ////////////////////////////////////////////////////////////////////////
    sleep(1);
    if (close(fd2[1]) < 0) { 
        printf("parent: Can\'t close writing side of pipe to parent\n");
        exit(-1);
    }
    //по идее должен ждать, пока ребенок не закроет поток записи
    size = read(fd2[0], resstring, 12);
 
    if (size < 0) {
        printf("Can\'t read string from pipe to parent\n");
        exit(-1);
    }
 
    printf("Parent exit, resstring:%s\n", resstring);
 
    if (close(fd2[0]) < 0) {  //закрываю трубу на чтение для родителя
        printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }
  }
  else {
    printf("Child enter\n");
 
    if (close(fd1[1]) < 0) { //закрываем поток на запись, на всякий. для отрезания конфликтных ситуаций
      printf("child: Can\'t close writing side of pipe to child\n");
      exit(-1);
    }
 
    size = read(fd1[0], resstring, 12); //(я так понял что у каждого процесса свой дескриптор), поэтому траблов с тем что fd1[0] 
    //закрыт после родителя не возникнет
 
    if (size < 0) {
      printf("Can\'t read string from pipe to child\n");
      exit(-1);
    }
 
    printf("Child exit, resstring:%s\n", resstring);
 
    if (close(fd1[0]) < 0) {
      printf("child: Can\'t close reading side of pipe to child\n"); exit(-1);
    }
    ////////////////////////////////////////////////////////////////////
    if (close(fd2[0]) < 0) { 
        printf("parent: Can\'t close reading side of pipe to parent\n");
        exit(-1);
    }
   
    size = write(fd2[1], "To the parent", 13); //в выход трубы отправляем строку
        
    if (size < 0) {
        printf("Can\'t write string from pipe to parent\n");
        exit(-1);
    }
 
    if (close(fd2[1]) < 0) {
        printf("parent: Can\'t close writing side of pipe to parent\n");
        exit(-1);
    }
 
  }
 
  return 0;
}
