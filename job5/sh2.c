#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/string.h>
#include<fcntl.h>
struct command{
	int count;
	char *argv[10];
	char *in;
	char *out;
	char *append;
};


int split(char *cmd,char *argv[]){
	int c=0;
	char *token;
	token=strtok(cmd," ");
	while(token!=NULL){
		argv[c]=token;
		token=strtok(NULL," ");
		c++;
	}
	return c;
}

void cmdout(struct command *inst){
	printf("%d\n",inst->count);
	printf("in  %s\n",inst->in);
	printf("out  %s\n",inst->out);
	printf("app  %s\n",inst->append);
	}
	
void cmdhandle(char *cmd,struct command *inst){
	int wordc;
	char *argv[10];
	inst->count=0;
	inst->in=NULL;
	inst->out=NULL;
	inst->append=NULL;
	cmd[strlen(cmd)-1]=0;
	wordc=split(cmd,argv);
	for(int i=0;i<wordc;i++){
		if(strncmp(argv[i],">",1)==0&&strncmp(argv[i],">>",2)!=0){
			inst->out=strtok(argv[i],">");
		}
		else if(strncmp(argv[i],"<",1)==0){
			inst->in=strtok(argv[i],"<");
		}
		else if(strncmp(argv[i],">>",2)==0){
			inst->append=strtok(argv[i],">>");
		}
		else{
			inst->argv[inst->count]=argv[i];
			inst->count++;
		}	
	}
	inst->argv[inst->count]=NULL;

}

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
	
		
    struct command inst;
    cmdhandle(cmd,&inst);
    //cmdout(&inst);
    
    pid_t pid;
    pid=fork();
    if(pid==0){
    	if(inst.in!=NULL){
    		int fdi=open(inst.in,O_RDONLY);
    		dup2(fdi,0);
    		close(fdi);
    	}
    	if(inst.out!=NULL){
    		int fdo=open(inst.out,O_CREAT|O_RDWR);
    		dup2(fdo,1);
    		close(fdo);
    	}
    	if(inst.append!=NULL){
    		int fda=open(inst.append,O_APPEND|O_CREAT|O_RDWR);
    		dup2(fda,1);
    		close(fda);
    	}
    	execvp(inst.argv[0],inst.argv);
    }
    wait(NULL);
}

int main()
{
	while(1){
		printf("> ");
		char cmd[100];
		fgets(cmd,100,stdin);
		mysys(cmd);	
	}
    return 0;
}
