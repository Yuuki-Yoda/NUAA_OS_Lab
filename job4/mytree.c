#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

void tab(int dpt){
	int space=2*dpt;
	for(int i=0;i<space;i++){
		printf(" ");
	}
}


void mytree(char path[],int dpt){
	DIR *dir;
	char spath[256];
	if((dir=opendir(path))==NULL){
		perror("open dir err");
		exit(-1);
	}
	struct dirent *file;
    while((file=readdir(dir))!=NULL){
    	if(strncmp(file->d_name,".",1)==0||strcmp(file->d_name,"..")==0)
    		continue;
    	tab(dpt);
    	printf("%s\n",file->d_name);
    	if(file->d_type==DT_DIR){
    		sprintf(spath,"%s/%s",path,file->d_name);
    		mytree(spath,dpt+1);
    	}
    }
    closedir(dir);
    return;
}



int main(int argc,char *argv[])
{
    char path[256];
    if(argc != 2&&argc!=1)
    {
        printf("operation,error");
        return -1;
    }
	
	if(argc==1){
		getcwd(path,sizeof(path));		
    	mytree(path,0);
	}
	
	if(argc==2){
    	mytree(argv[1],0);
	}

    return 0;
}
