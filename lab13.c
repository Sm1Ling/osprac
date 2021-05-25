#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{

char namelast[12];
char namenew[12];
FILE* fw;


sprintf(namelast, "0.fff");
fw = fopen(namelast,"w");
fclose(fw);
printf("Файл %s создан\n",namelast);

for(int i = 1; i< 10000000; i++) {

sprintf(namenew, "%d.fff", i);

symlink(namelast, namenew); 
printf("Связь с файлом %s создана\n",namenew);

fw = fopen(namenew, "r");
if(!fw){
printf("Recursion level: %d\n",i-1);
break;
}
fclose(fw);
;
strcpy(namelast, namenew);

}

return 0;
}
