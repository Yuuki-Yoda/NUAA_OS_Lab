#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


typedef struct{
	int num;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sem_t;

sem_t sem0,sem1;

void sem_init(sem_t *sem,int val){
	pthread_mutex_init(&sem->mutex,NULL);
	pthread_cond_init(&sem->cond,NULL);
	sem->num=val;
}

void sem_wait(sem_t *sem){
	pthread_mutex_lock(&sem->mutex);
	while(sem->num<=0)
		pthread_cond_wait(&sem->cond,&sem->mutex);
	sem->num--;
	pthread_mutex_unlock(&sem->mutex);
}

void sem_signal(sem_t *sem){
	pthread_mutex_lock(&sem->mutex);
	sem->num++;
	pthread_cond_signal(&sem->cond);
	pthread_mutex_unlock(&sem->mutex);
	
}


void pping(){
	printf("ping\n");
}

void ppong(){
	printf("pong\n");
}

void *fping(void *arg){
	
	int i=10;
	while(i>0){
		sem_wait(&sem1);
		pping();
		sem_signal(&sem0);	
		i--;
	}
}

void *fpong(void *arg){
	
	int i=10;
	while(i>0){
		sem_wait(&sem0);
		ppong();
		sem_signal(&sem1);		
		i--;
	}
}

int main()
{ 
    pthread_t ping;
    pthread_t pong;

	sem_init(&sem0,0);
	sem_init(&sem1,1);
	
    pthread_create(&ping,NULL,fping,NULL); 
    pthread_create(&pong,NULL,fpong,NULL);

    pthread_join(ping,NULL);
    pthread_join(pong,NULL);
    
    
    return 0;
}
