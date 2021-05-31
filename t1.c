#include <signal.h>
#include <unistd.h>

int main(void) {
//
// Set the process response to the SIGINT signal to ignore
//
(void)signal(SIGINT, SIG_IGN);
(void)signal(SIGQUIT, SIG_IGN);//отрубаю обработку ctrl+4
//
// From this point, the process will ignore the occurrence of the SIGINT signal.
//
while(1);
return 0;
}