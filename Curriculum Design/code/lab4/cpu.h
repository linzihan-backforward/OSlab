#ifndef CPU_H
#define CPU_H
#include<stdio.h>
typedef struct CPU{
    char name[20];//cpu name
   unsigned  user ; //从系统启动开始累计到当前时刻，用户态的CPU时间（单位：jiffies） ，不包含 nice值为负进程。1jiffies=0.01秒
   unsigned nice ; //从系统启动开始累计到当前时刻，nice值为负的进程所占用的CPU时间（单位：jiffies）
   unsigned system ; //从系统启动开始累计到当前时刻，核心时间（单位：jiffies）
   unsigned idle;  //从系统启动开始累计到当前时刻，除硬盘IO等待时间以外其它等待时间（单位：jiffies）
   unsigned iowait;  //从系统启动开始累计到当前时刻，硬盘IO等待时间（单位：jiffies） ，
   unsigned irq;  //从系统启动开始累计到当前时刻，硬中断时间（单位：jiffies）
   unsigned softirq;  //从系统启动开始累计到当前时刻，软中断时间（单位：jiffies）
}cpuinfo;
void GetCpuInfo(cpuinfo * s_cpu){
    FILE *f;
    char cpu[200];
    f=fopen("/proc/stat","r");
    fgets(cpu,sizeof(cpu),f);
    sscanf(cpu,"%s %u %u %u %u %u %u %u",s_cpu->name,&s_cpu->user,&s_cpu->nice,&s_cpu->system,&s_cpu->idle,&s_cpu->iowait,&s_cpu->irq,&s_cpu->softirq);
    fclose(f);
}
double CalcCpuRate(cpuinfo *s_cpu1,cpuinfo *s_cpu2){
    int total1=s_cpu1->user+s_cpu1->nice+s_cpu1->system+s_cpu1->idle+s_cpu1->iowait+s_cpu1->irq+s_cpu1->softirq;
    int total2=s_cpu2->user+s_cpu2->nice+s_cpu2->system+s_cpu2->idle+s_cpu2->iowait+s_cpu2->irq+s_cpu2->softirq;
    if(total1!=total2){
        return  (double)(100.0-(s_cpu2->idle-s_cpu1->idle)*100.0/(total2-total1));
    }
    else return 0.0;
}
#endif // CPU_H
