#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>


#define TOTAL 10
#define CPU 2
#define MISSION (TOTAL/CPU)

struct data{
	int start;
	int end;
	int *array;
 };




void *compute(void *arg){
	struct data *data;
	data=(struct data *)arg;
	int *array;
	array=data->array;
	for(int i= data->start;i< data->end -1;i++){
		int index=i;
		int min=array[i];
		for(int j=i+1;j<data->end;j++){
			if(array[j]<min){
				index=j;
				min=array[j];
			}
			
		}
		if(index==i)
			continue;
		array[index]=array[i];
		array[i]=min;
	}
	
	return NULL;
}

void merge(int *in,int *out){
	int i,j,k;
	for(i=0,j=(TOTAL/2),k=0;i<TOTAL/2&&j<TOTAL;){
		if (in[i]<in[j])
			out[k++]=in[i++];
		else
			out[k++]=in[j++];
	}
	while(i<TOTAL/2)
		out[k++]=in[i++];
	while(j<TOTAL)
		out[k++]=in[j++];
}


int main()
{ 
    pthread_t workers[CPU];
	struct data datas[CPU];
	int a[TOTAL];
	int sorted[TOTAL];
	
	 srand(time(NULL));
	 for(int i = 0; i <TOTAL; i++) {
		a[i] = rand() % 100 + 1;
	 }
	 
	 printf("origin array:\n");
	 for(int i = 0; i < TOTAL; i++) {
	 	printf("%d ", a[i]);
	 }
	 printf("\n");


    for (int i = 0; i < CPU; i++) {
        struct data *data;
        data = &datas[i];
        data->start = i * MISSION; 
        data->end = (i + 1) * MISSION;
        data->array=a;
        pthread_create(&workers[i], NULL, compute, data);
    }

    for (int i = 0; i < CPU; i++) {
        pthread_join(workers[i], NULL);
    }
    
    merge(a,sorted);

	printf("sorted array:\n");
	for(int i = 0; i < TOTAL; i++) {
		printf("%d ", sorted[i]);
	}
	printf("\n");
    return 0;
}
