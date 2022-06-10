#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

int main(int argc, char *argv[]){
	int f;
	if(argc!=3){
		printf("error/n");
	}

	f=open(argv[1],O_RDONLY);
	if(f==-1){
		printf("no such file/n");
		exit(0);
	}
	
	int size=lseek(f,0,SEEK_END);
	lseek(f,0,SEEK_SET);
	
	char *buffer;
	buffer=(char*)malloc((sizeof(char))*(size+1));

	read(f,buffer,size);

	close(f);

	f=open(argv[2],O_WRONLY);
	if(f==-1){
                printf("no such file/n");
                exit(0);
       	}

	write(f,buffer,size);

	close(f);
	

	free(buffer);



	return 0;
}

