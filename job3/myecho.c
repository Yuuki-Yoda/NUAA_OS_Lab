#include<stdlib.h>
#include<stdio.h>
int main(int argc,char *argv[])
{
    if(argc==1){
            printf("Require Operation!\n");
            exit(0);
    }
    for(int i=1;i<argc;i++)
        printf("%s ",argv[i]);
    printf("\n");
    return 0;
}
