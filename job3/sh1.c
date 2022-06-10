#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/string.h>

void mysys(char *cmd)
{
	if(cmd[0]=='c'&&cmd[1]=='d'){
		char *temp;
		temp=strtok(cmd," ");
		temp=strtok(NULL,"\n");
		int error=chdir(temp);
		if(error==-1)
			return;
		printf("dirctionary changed to %s\n",getcwd(NULL,0));
		return;
	}


	if(cmd[0]=='p'&&cmd[1]=='w'&&cmd[2]=='d'){
		printf("%s\n",getcwd(NULL,0));
		return;
	}
	
	
	if(cmd[0]=='e'&&cmd[1]=='x'&&cmd[2]=='i'&&cmd[3]=='t')
		exit(0);
	
		
    pid_t pid;
    pid=fork();
    if(pid==0){
        execlp("sh","sh","-c",cmd,NULL);
    }

    wait(NULL);
}

int main()
{
	while(1){
		printf(">");
		char cmd[100];
		fgets(cmd,100,stdin);
		mysys(cmd);	
	}
    return 0;
}
