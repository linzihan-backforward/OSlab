#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
int pid1,pid2;
int pipe_fd[2];
void my_func(int sig_no) {
     if(sig_no == SIGINT){
        printf("Receive SIGINT.\n");
        kill(pid1,SIGUSR1);
        kill(pid2,SIGUSR1);
     }

}
void fun1(int sig_no) {
     if(sig_no == SIGUSR1){
        printf("Child Process l is Killed by Parent!\n");
        exit(0);
     }
}

void fun2(int sig_no) {
     if(sig_no == SIGUSR1){
        printf("Child Process 2 is Killed by Parent!\n");
        exit(0);
     }
}
int main(){
    if(pipe(pipe_fd)<0){
        printf("pipe create error ");
        return -1;
    }
    if(signal(SIGINT, my_func) == SIG_ERR)
        printf("can't catch SIGINT.\n'");
    pid1=fork();
    if(pid1==0){
        signal(SIGINT,SIG_IGN);
        printf("process1 pid:%d\n",getpid());
        if(signal(SIGUSR1,fun1) ==SIG_ERR)
            printf("can't catch SIGUER1\n");
        int x=1;
        char w_buf[]={'I',' ','s','e','n','d',' ','y','o','u',' ','x',' ','t','i','m','e','s','.','\n','\0'};
        while(1){
            w_buf[11]=x-0+'0';
            if(write(pipe_fd[1],w_buf,strlen(w_buf))==-1)
                printf("pipe write error\n");
            x++;
            sleep(1);
        }

    }
    else{
        pid2=fork();
        if(pid2==0){
            signal(SIGINT,SIG_IGN);
            printf("process2 pid:%d\n",getpid());
            if(signal(SIGUSR1,fun2) ==SIG_ERR)
                printf("can't catch SIGUER1\n");
            char r_buf[50];
            while(1){
                read(pipe_fd[0],r_buf,50);
                printf("%s",r_buf);
            }
        }
    }
    int p1state,p2state;
    waitpid(pid1,&p1state,0);
    waitpid(pid2,&p2state,0);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    printf("Parent Process is Killed!\n");
    return 0;
}

