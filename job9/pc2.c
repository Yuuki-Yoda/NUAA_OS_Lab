#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 4
#define ITEM_COUNT 8
int buffer1[CAPACITY],buffer2[CAPACITY];
int in1,in2;
int out1,out2;

typedef struct{
	int num;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}sem_t;

sem_t sem;

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


int get_item1(){
    int item;
    item = buffer1[out1];
    out1 = (out1 + 1) % CAPACITY;
    return item;
}

int get_item2(){
    int item;
    item = buffer2[out2];
    out2 = (out2 + 1) % CAPACITY;
    return item;
}

void put_item1(int item){
    buffer1[in1] = item;
    in1 = (in1 + 1) % CAPACITY;
}

void put_item2(int item){
    buffer2[in2] = item;
    in2 = (in2 + 1) % CAPACITY;
}

sem_t mutex1_sema,mutex2_sema;
sem_t empty_buffer1_sema,empty_buffer2_sema;
sem_t full_buffer1_sema,full_buffer2_sema;



void *calculator(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
		sem_wait(&full_buffer1_sema);
        sem_wait(&mutex1_sema);

        item = get_item1(); 
        printf("%c:%c\n", item,item-32); 

        sem_signal(&mutex1_sema);
        sem_signal(&empty_buffer1_sema);
       
        sem_wait(&empty_buffer2_sema);
        sem_wait(&mutex2_sema);

        put_item2(item-32);

        sem_signal(&mutex2_sema);
        sem_signal(&full_buffer2_sema);
    }
    return NULL;
}

void *produce(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        sem_wait(&empty_buffer1_sema);
        sem_wait(&mutex1_sema);

        item = 'a' + i;
        put_item1(item);
        printf("%c\n", item); 

        sem_signal(&mutex1_sema);
        sem_signal(&full_buffer1_sema);
    }
    return NULL;
}

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        sem_wait(&full_buffer2_sema);
        sem_wait(&mutex2_sema);

        item = get_item2(); 
        printf("%c\n", item); 

        sem_signal(&mutex2_sema);
        sem_signal(&empty_buffer2_sema);
    }
    return NULL;
}

int main()
{ 
    pthread_t consumer_tid,producer_tid,calc_tid;

    sem_init(&mutex1_sema, 1);
    sem_init(&mutex2_sema, 1);
    
    sem_init(&empty_buffer1_sema, CAPACITY );
    sem_init(&full_buffer1_sema, 0);
    
    sem_init(&empty_buffer2_sema, CAPACITY );
    sem_init(&full_buffer2_sema, 0);

    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&producer_tid, NULL, produce, NULL);
    pthread_create(&calc_tid, NULL, calculator, NULL);
    pthread_join(consumer_tid, NULL);
    pthread_join(producer_tid, NULL);
    pthread_join(calc_tid, NULL);
    return 0;
}
