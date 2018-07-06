#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include <signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include <sys/wait.h>
int shmid=23333;
int Mykey=7777;

void P(int semid,int index)
{	    struct sembuf sem;
        sem.sem_num = index;
        sem.sem_op = -1;
        sem.sem_flg = 0; //操作标记：0或IPC_NOWAIT等
        semop(semid,&sem,1);	//1:表示执行命令的个数
        return;
}
void V(int semid,int index)
{
    struct sembuf sem;
      sem.sem_num = index;
      sem.sem_op =  1;
      sem.sem_flg = 0;
      semop(semid,&sem,1);
      return;
}
union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *arry;
};
int isempty;
int pid1,pid2;
int isfull;
int mutex;
int  isend;
char *s;
int main(int argc,char *argv[])
{
    int shm;
    if(argc!=3){
        printf("argc number fault!\n");
        exit(0);
    }
    int readfile = open(argv[1],O_RDONLY,S_IRUSR|S_IWUSR);
    int writefile = open(argv[2],O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
    if(readfile==-1||writefile==-1){
        printf("file name error!\n");
        exit(0);
    }
    if((shm=shmget(shmid,10,IPC_CREAT|0666))==-1)
        printf("shm create error\n");
    s = (char *)shmat(shm,NULL,0);
    if((isempty=semget(Mykey,1,IPC_CREAT|0666))==-1)
        printf("semget error\n");
    if((isfull=semget(Mykey+1,1,IPC_CREAT|0666))==-1)
        printf("semget error\n");
    if((mutex=semget(Mykey+2,1,IPC_CREAT|0666))==-1)
        printf("semget error\n");
    if((isend=semget(Mykey+3,1,IPC_CREAT|0666))==-1)
        printf("semget error\n");
    union semun sem_union1;
    sem_union1.val = 8;
    if (semctl(isfull, 0, SETVAL, sem_union1) == -1){//initial the traffic1
        printf("seminit error\n");
    }
    sem_union1.val = 0;
    if (semctl(isempty, 0, SETVAL, sem_union1) == -1){//initial the traffic2
        printf("seminit error\n");
    }
    sem_union1.val = 1;
    if (semctl(mutex, 0, SETVAL, sem_union1) == -1){//initial the mutex
        printf("seminit error\n");
    }
    if (semctl(isend, 0, SETVAL, sem_union1) == -1){//initial the mutex
        printf("seminit error\n");
    }
    int i=0,j=0;
    int flag=0;
    if((pid1=fork())==0){
        char get;
        while(read(readfile,&get,sizeof(char))!=0){

            P(isfull,0);
            P(mutex,0);

            i=i%8;//取余
            s[i]=get;//读内容到缓存
            i++;//循环加1

            V(mutex,0);
            V(isempty,0);
        }
        P(isend,0);
        i=i-1;
        i=i%8;
        s[8]=i;
        flag=1;
        s[9]=flag;
        V(isend,0);
        printf("read end!\n");
        exit(0);
    }
    else{
        if((pid2=fork())==0){

              while(1){

                P(isend,0);
                if(s[9]==1&&s[8]==j){
                    V(isend,0);
                    char g;
                    g=s[j];
                //printf("%d\n",g);
                    if(write(writefile,&g,1)==-1){
                        printf("%d\n",j);
                    //printf("write error!\n");
                    }//将当前缓存中的数据写入文件中
                    printf("write end!\n");
                    //break;
                    exit(0);
                }
                V(isend,0);
                P(isempty,0);
                P(mutex,0);
                char g;
                j=j%8;//取余
                g=s[j];
                //printf("%d\n",g);
                if(write(writefile,&g,1)==-1){
                    printf("%d\n",j);
                    //printf("write error!\n");
                }//将当前缓存中的数据写入文件中
                //printf("%d\n",j);
                j++;//循环加1

                V(mutex,0);
                V(isfull,0);
            }
            printf("write end!\n");
            exit(0);
        }
    }
    int p1state,p2state;
    waitpid(pid1,&p1state,0);
    waitpid(pid2,&p2state,0);



    if (semctl(isempty, 0, IPC_RMID, sem_union1) == -1){
        printf("Failed to delete traffic1\n");
    }
    if (semctl(isfull, 0, IPC_RMID, sem_union1) == -1){
        printf("Failed to delete traffic2\n");
    }
    if (semctl(mutex, 0, IPC_RMID, sem_union1) == -1){
        printf("Failed to delete traffic2\n");
    }
    shmdt((void *)s);
    shmctl(shm,IPC_RMID,0);
    printf(" copy success!\n");
    close(writefile);
    close(readfile);
    return 0;
}
