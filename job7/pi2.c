#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define TOTAL 10000
#define CPU 4
#define MISSION (TOTAL/CPU)

struct data{
	int start;
	int end;
};


struct out{
	float num;
};


void *compute(void *arg){
	struct out *result;
	result=malloc(sizeof(struct out));
	struct data *data;
	data=(struct data *)arg;
	for(int i= data->start;i< data->end;i++){
		if(i%2)
        result->num -= 1.0/(i*2+1);
        else
		result->num += 1.0/(i*2+1);
	}
	
	return (void *)result;

}



int main()
{ 
    pthread_t workers[CPU];
	struct data datas[CPU];

    for (int i = 0; i < CPU; i++) {
        struct data *data;
        data = &datas[i];
        data->start = i * MISSION; 
        data->end = (i + 1) * MISSION;
        pthread_create(&workers[i], NULL, compute, data);
    }

    float sum = 0;
    for (int i = 0; i < CPU; i++) {
        struct out *result;
        pthread_join(workers[i], (void **)&result);
        sum += result->num;
    }

    printf("sum = %f\n", 4*sum);
    return 0;
}
