#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t wait_ping;
pthread_cond_t wait_pong;
int flag=0;


void pping(){
	printf("ping\n");
}

void ppong(){
	printf("pong\n");
}

void *fping(void *arg){
	
	int i=10;
	while(i>0){
		pthread_mutex_lock(&mutex);
		while(flag)
			pthread_cond_wait(&wait_pong,&mutex);
		pping();
		flag=1;
		pthread_cond_signal(&wait_ping);
		pthread_mutex_unlock(&mutex);		
		i--;
	}
}

void *fpong(void *arg){
	
	int i=10;
	while(i>0){
		pthread_mutex_lock(&mutex);
		while(!flag)
			pthread_cond_wait(&wait_ping,&mutex);
		ppong();
		flag=0;
		pthread_cond_signal(&wait_pong);
		pthread_mutex_unlock(&mutex);		
		i--;
	}
}

int main()
{ 
    pthread_t ping;
    pthread_t pong;
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&wait_ping,NULL);
    pthread_cond_init(&wait_pong,NULL);
    pthread_create(&ping,NULL,fping,NULL); 
    pthread_create(&pong,NULL,fpong,NULL);

    pthread_join(ping,NULL);
    pthread_join(pong,NULL);
    
    
    return 0;
}
