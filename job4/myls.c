#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>


void ls(DIR *dir){
	struct dirent *file;
    while((file=readdir(dir))!=NULL){
    	if(strncmp(file->d_name,".",1)==0||strcmp(file->d_name,"..")==0)
    		continue;
    	printf("%s\n",file->d_name);
    }
    closedir(dir);
    return;
}



int main(int argc,char *argv[])
{
    DIR *dir;
    if(argc != 2&&argc!=1)
    {
        printf("operation,error");
        return -1;
    }
	
	if(argc==1){
		char path[256];
		getcwd(path,sizeof(path));		
		if((dir=opendir(path)) == NULL){
        	perror("Fail to opendir");
        	return -1;
    	}
    	ls(dir);
	}
	
	if(argc==2){
		if((dir = opendir(argv[1])) == NULL){
        	perror("Fail to opendir");
        	return -1;
    	}
    	ls(dir);
	}

    return 0;
}
