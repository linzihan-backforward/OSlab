#include <stdio.h>
#include <stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<linux/sem.h>
#include <unistd.h>
int Mykey=777;
void P(int semid,int index){
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op = -1;
    sem.sem_flg = 0; //操作标记：0或IPC_NOWAIT等
    semop(semid,&sem,1);	//1:表示执行命令的个数
    return;
}
void V(int semid,int index){
    struct sembuf sem;
    sem.sem_num = index;
    sem.sem_op =  1;
    sem.sem_flg = 0;
    semop(semid,&sem,1);
    return;
}
int traffic1,traffic2;
int a=1;
void *p1fun(void *argv){

    for(int i=1;i<100;i++){
        P(traffic2,0);
        printf("p1\n");
        a++;
        V(traffic1,0);
    }
    return NULL;
}
void *p2fun(void *argv){
    for(int j=1;j<=100;j++){
        P(traffic1,0);
        printf("p2:a=%d\n",a);
        V(traffic2,0);
    }
    return NULL;
}
int main()
{
    pthread_t p1,p2;
    if((traffic1=semget(Mykey,1,IPC_CREAT|0666))==-1)
        printf("semget error\n");
    if((traffic2=semget(Mykey+1,1,IPC_CREAT|0666))==-1)
        printf("semget error\n");
    union semun sem_union;
    sem_union.val = 1;
    if (semctl(traffic1, 0, SETVAL, sem_union) == -1){//initial the traffic1
        printf("seminit error\n");
    }
    sem_union.val = 0;
    if (semctl(traffic2, 0, SETVAL, sem_union) == -1){//initial the traffic2
        printf("seminit error\n");
    }

    if(pthread_create(&p1,NULL,p1fun,NULL)!=0)
        printf("create thread1 error\n");
    if(pthread_create(&p2,NULL,p2fun,NULL)!=0)
        printf("create thread2 error\n");
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    if (semctl(traffic1, 0, IPC_RMID, sem_union) == -1){
        printf("Failed to delete traffic1\n");
    }
    if (semctl(traffic2, 0, IPC_RMID, sem_union) == -1){
        printf("Failed to delete traffic2\n");
    }
    printf("Main is over\n");
    return 0;
}
