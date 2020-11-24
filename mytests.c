#include <signal.h>
#include "fastlog.h"

int main (void)

{
fastlog_init();

 kill(0,SIGINT);
int i;
for (i=0; i<50; i++){
    fastlog_write(1, "string");
}
kill(0,SIGUSR1);
for (i=0; i<10; i++){
    fastlog_write(2, "string");
}
kill(0,SIGSEGV);


    return 0;
}