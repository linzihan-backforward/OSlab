#ifndef MEMORY_H
#define MEMORY_H
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<string>
typedef struct MEM{
    unsigned long total;
    unsigned long free;
    unsigned long buffers;
    unsigned long cached;
}meminfo;
void GetMemInfo(meminfo *mem){
    std::string str;
    unsigned long memtotal,memfree,memav,membuffer,memcached;
    std::ifstream inputstream("/proc/meminfo");
    inputstream>>str>>memtotal>>str;
    inputstream>>str>>memfree>>str;
    inputstream>>str>>memav>>str;
    inputstream>>str>>membuffer>>str;
    inputstream>>str>>memcached>>str;
    mem->buffers=membuffer;
    mem->cached=memcached;
    mem->free=memfree;
    mem->total=memtotal;
    inputstream.close();
}
double CalcMemRate(meminfo *mem){
    return (double)((mem->total-mem->buffers-mem->cached-mem->free)*100.0/mem->total);
}
#endif // MEMORY_H
