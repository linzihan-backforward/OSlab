#ifndef SYS_H
#define SYS_H
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<QString>
#endif // SYS_H
std::string GetHostName(){
    std::ifstream inputstream("/proc/sys/kernel/hostname");
    std::string str;
    inputstream>>str;
    inputstream.close();
    return str;
}
double GetRunTime(){
    std::ifstream inputstream("/proc/uptime");
    double runtime;
    inputstream>>runtime;
    inputstream.close();
    return runtime;
}
std::string GetOsType(){
    std::ifstream inputstream1("/proc/sys/kernel/ostype");
    std::string osname;
    inputstream1>>osname;
    std::ifstream inputstream2("/proc/sys/kernel/osrelease");
    std::string ostype;
    inputstream2>>ostype;
    inputstream1.close();
    inputstream2.close();

    return osname+" "+ostype;
}
std::string GetCpuType(){
    std::ifstream inputstream("/proc/cpuinfo");
    std::string temp,cputype;
    for (int i=1;i<=16;i++)
        inputstream>>temp;
    for(int i=1;i<=6;i++){
        inputstream>>temp;
        cputype+=temp;
        cputype+=" ";
    }
    inputstream.close();
    return cputype;
}
