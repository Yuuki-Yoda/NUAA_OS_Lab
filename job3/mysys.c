#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/string.h>

void mysys(char *command)
{
    pid_t pid;
    pid=fork();
    if(pid==0){
        execlp("sh","sh","-c",command,NULL);
    }

    wait(NULL);
}

int main()
{
    printf("--------------------------------------------------\n");
    mysys("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    mysys("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}
