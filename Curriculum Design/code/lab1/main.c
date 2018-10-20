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
char *s;
int main(int argc,char *argv[])
{
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
        char get;
        while(read(readfile,&get,sizeof(char))!=0){
            if(write(writefile,&get,1)==-1){
                    printf("%c\n",get);

            }
        }
    printf("copy success!\n");
    close(writefile);
    close(readfile);
    return 0;

}
