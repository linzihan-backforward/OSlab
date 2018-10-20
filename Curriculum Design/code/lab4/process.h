#ifndef PROCESS_H
#define PROCESS_H
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<unistd.h>
#include<dirent.h>
#include<iostream>
#endif // PROCESS_H
using namespace std;
struct PROC_INFO{
    string name;
    string pid;
    string ppid;
    string rss;
    string priority;
}proinfo[16384];
void ReadPro(PROC_INFO* pinfo,const char *pid){
    string temp,pidname;
    string s_pid =pid;
    std::ifstream inputstream("/proc/"+s_pid+"/stat");
    inputstream>>(pinfo->pid)>>pidname>>temp>>(pinfo->ppid)>>temp>>temp;
    inputstream>>temp>>temp>>temp>>temp>>temp>>temp;
    inputstream>>temp>>temp>>temp>>temp>>temp>>(pinfo->priority);
    inputstream>>temp>>temp>>temp>>temp>>temp>>(pinfo->rss);
    pinfo->name=pidname.substr(1,pidname.find(')')-1);
    inputstream.close();
}
int GetProcessInfo(){
    DIR *dir;
    struct dirent *ptr;
    int cnt=0;
    if(!(dir=opendir("/proc")))
        return 0;
    while((ptr=readdir(dir))!=false){
        if(ptr->d_name[0]>='1'&&ptr->d_name[0]<='9'){
            ReadPro(&(proinfo[cnt]),ptr->d_name);
            cnt++;
        }
    }
    closedir(dir);
    return cnt;
}
