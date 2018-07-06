#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <sys/types.h>
#include <string.h>
#include<time.h>
#include <dirent.h>
void printInfoList(char** allFileInfos, int indexForInfos) {
    for (int i = 0; i < indexForInfos; i++) {
        printf("%s", allFileInfos[i]);
    }
}
char* getFileInfo(struct stat* sP, char* filename, int* blocks) {
    char* buf = (char*)malloc(sizeof(char) * 1024);
    struct stat s = *sP;

    switch (s.st_mode & S_IFMT) {
        case S_IFREG:
            sprintf(buf, "-");
            break;
        case S_IFDIR:
            sprintf(buf, "d");
            break;
        case S_IFLNK:
            sprintf(buf, "l");
            break;
        case S_IFBLK:
            sprintf(buf, "b");
            break;
        case S_IFCHR:
            sprintf(buf, "c");
            break;
        case S_IFIFO:
            sprintf(buf, "p");
            break;
        case S_IFSOCK:
            sprintf(buf, "s");
            break;
    }

    for (int i = 8; i >= 0; i--) {
        if (s.st_mode & (1 << i)) {
            switch (i % 3) {
                case 2:
                    strcat(buf,"r");
                    break;
                case 1:
                    strcat(buf,"w");
                    break;
                case 0:
                    strcat(buf,"x");
                    break;
            }
        } else {
            strcat(buf,"-");
        }
    }

    struct passwd* p = getpwuid(s.st_uid);
    struct group* g = getgrgid(s.st_gid);

    char temp[128];
    sprintf(temp, " %d %s %s %6ld", (int)s.st_nlink, p->pw_name, g->gr_name,
            s.st_size);
    strcat(buf, temp);

    struct tm* t = localtime(&s.st_ctime);
    sprintf(temp, " %02dmonth %02d %02d:%02d", (t->tm_mon + 1), t->tm_mday,
            t->tm_hour, t->tm_min);
    strcat(buf, temp);

    sprintf(temp, " %s\n", filename);
    strcat(buf, temp);
    if (s.st_size % 4096 == 0) {
        *blocks = *blocks + s.st_size / 1024;
    } else {
        *blocks = *blocks + s.st_size / 1024 + 4;
    }

    return buf;
}
int showDir(char* dirname) {
    printf("%s:\n", dirname);
    int blocks = 0;

    DIR* dir = opendir(dirname);
    struct dirent* dirDescribe;
    struct stat st;
    char nowDirnameBuf[1024];


    int indexForInfos = 0;
    char** allFileInfos = NULL;
    allFileInfos = (char**)malloc(sizeof(char*) * 100);

    int indexForDirNames = 0;

    char allDirNames[100][100];//printf("%s\n",dirname);
    dirDescribe = readdir(dir);

    while ((dirDescribe = readdir(dir)) != NULL) {

        strcpy(nowDirnameBuf, dirname);
        strcat(nowDirnameBuf, "/");
        strcat(nowDirnameBuf, dirDescribe->d_name);
        //printf("%s\n",nowDirnameBuf);
        if (stat(nowDirnameBuf, &st)) {
            printf("error\n");
            return -1;
        }

        if (dirDescribe->d_name[0] == '.')
            continue;

        if (S_ISDIR(st.st_mode)) {
            char nameBuf[100] ;
            strcpy(nameBuf, nowDirnameBuf);
            strcpy(allDirNames[indexForDirNames] , nameBuf);
            indexForDirNames++;
        }
        char* fileInfo = getFileInfo(&st, dirDescribe->d_name, &blocks);
        allFileInfos[indexForInfos] = fileInfo;
        indexForInfos++;


    }
    printf("total:%d\n", blocks);
    printInfoList(allFileInfos, indexForInfos);
    printf("\n");
    for (int i = 0; i < indexForDirNames; i++)
        showDir(allDirNames[i]);  // enter the dir
    // release the memory
    for (int i = 0; i < indexForInfos; i++)
        free(allFileInfos[i]);
    free(allFileInfos);
    closedir(dir);
    return 0;
}
int main(int argc, char** argv) {
    char* dir = ".";

    if(argc!=2){ //check the param
        printf("param error!\n");
        exit(0);
    }
    dir = argv[1];
    showDir(dir);
    return 0;
}
