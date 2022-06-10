#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 1

typedef struct {                
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

sema_t mutex_s;   
sema_t empty;
sema_t full;

struct task {   
    int is_end;   
    char path[128];
    char string[128];
};
struct task q[CAPACITY];   
int h=0,t=-1;    

void sema_init(sema_t *sema, int value)  
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)             
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)           
{
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

void find_file(char *path, char *target)   
{
	FILE *file = fopen(path, "r");
	char line[256];
	while (fgets(line, sizeof(line), file))
	{
	    if(strstr(line, target))
	        printf("%s: %s", path, line);
	}
	fclose(file);
}

void *worker_entry(void *arg)  
{
    while (1) 
    {
        sema_wait(&full);
        sema_wait(&mutex_s);

        struct task task;
        task=q[h%CAPACITY]; 
        h++;     
        if(task.is_end) 
        	break;       
        find_file(task.path,task.string);

        sema_signal(&mutex_s);
        sema_signal(&empty);
    }
    sema_signal(&mutex_s);     //for break
    sema_signal(&empty);
}

void find_dir(char *path, char *target)  { 
	char bak[255];
	DIR *dir = opendir(path);
	if(dir==NULL) 
		return;
	struct dirent *entry;
	while(entry = readdir(dir))
	{
	    if(strcmp(entry->d_name, ".") == 0)
	        continue;
	    if(strcmp(entry->d_name, "..") == 0)
	        continue;

	    strcpy(bak,path);
	    strcat(bak,"/");
	    strcat(bak,entry->d_name);

	    if (entry->d_type == DT_DIR)
	    	find_dir(bak,target);
	    if (entry->d_type == DT_REG)   
	    {
	    	sema_wait(&empty);
            sema_wait(&mutex_s);
            
		 	t++;
		 	t=t%CAPACITY;
		 	q[t].is_end=0;
		 	strcpy(q[t].path,bak);
		 	strcpy(q[t].string,target);
		 	
		 	sema_signal(&mutex_s);
        	sema_signal(&full);
            }
        }
        closedir(dir);
}
void producespecial()  
{  
	sema_wait(&empty);
    sema_wait(&mutex_s);
            
	t++;
	t=t%CAPACITY;
	q[t].is_end=1;
		 	
	sema_signal(&mutex_s);
    sema_signal(&full);
}

int main(int argc, char *argv[])
{
	if (argc!=3) 
	{
	    puts("Usage: pfind file string");
	    return 0;
	}
	char apath[128];
	char *path= argv[1];
	char *string = argv[2];
	struct stat info;
	stat(path, &info);
	if(S_ISREG(info.st_mode))   
	{                         
	    find_file(path,string);  //deal direct file
	    return 0;
	}
	
	pthread_t consumer1_tid,consumer2_tid;
	
	sema_init(&mutex_s, 1);   //mutex
    
    sema_init(&empty, CAPACITY);  
    sema_init(&full, 0);   
    
	pthread_create(&consumer1_tid, NULL, worker_entry, NULL);
    pthread_create(&consumer2_tid, NULL, worker_entry, NULL);
	
	find_dir(path,string);   //main thread
	
	for(int i=0;i<2;i++) producespecial();  
	  
	pthread_join(consumer1_tid, NULL);
    pthread_join(consumer2_tid, NULL);   
	        
	return 0;
}
