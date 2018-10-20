#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTimer>
#include<QDateTime>
#include<QDebug>
#include<QPainter>
#include"cpu.h"
#include"memory.h"
#include"sys.h"
#include<stdio.h>
#include<process.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<QPushButton>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    HostName();
    BootTime();
    OsType();
    CpuType();
    timer =new QTimer(this);
    timer->start(1);
    connect(timer,SIGNAL(timeout()),this,SLOT(RunTime()));
    connect(timer,SIGNAL(timeout()),this,SLOT(CpuRate()));
    connect(timer,SIGNAL(timeout()),this,SLOT(NowTime()));
    connect(timer,SIGNAL(timeout()),this,SLOT(MemRate()));
    QTimer* timer1= new QTimer(this);
    timer1->start(5);
    connect(timer1,SIGNAL(timeout()),this,SLOT(ProcessInfo()));
}
void MainWindow::CpuRate(){
    cpuinfo cpu1,cpu2;
    double cpurate;
    GetCpuInfo(&cpu1);
    sleep(1);
    GetCpuInfo(&cpu2);
    cpurate=CalcCpuRate(&cpu1,&cpu2);
    ui->textBrowser_2->setText("CPU使用率:"+QString::number(cpurate,10,2)+"%");
    ui->textBrowser_2->setAlignment(Qt::AlignCenter);
    addCpuPoint(cpurate);
}
void MainWindow::NowTime(){
    QDateTime time = QDateTime::currentDateTime();
    QString nowTime = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->textBrowser->setText(nowTime);
    ui->textBrowser->setAlignment(Qt::AlignCenter);
}
void MainWindow::MemRate(){
    meminfo mem;
    double memRate;
    GetMemInfo(&mem);
    memRate=CalcMemRate(&mem);
    ui->textBrowser_3->setText("memory使用率:"+QString::number(memRate,10,2)+"%");
    ui->textBrowser_3->setAlignment(Qt::AlignCenter);
    addMemPoint(memRate);
}
void MainWindow::HostName(){
    std::string hostname=GetHostName();
    ui->Hostlabel->setText("主机名:    "+QString::fromStdString(hostname));
}
void MainWindow::RunTime(){
    int rtime=(int)GetRunTime();
    int days = (rtime)/(24*3600);
    int hours = (rtime)%(24*3600)/3600;
    int minutes = (rtime)%3600/60;
    int second = (rtime)%60;
    ui->runtimelabel->setText("系统运行时间: "+QString::number(days,10)+"天"+QString::number(hours,10)+"小时"+QString::number(minutes,10)+"分钟"+QString::number(second,10)+"秒");
}
void MainWindow::BootTime(){
    int boottime=QDateTime::currentDateTime().toTime_t()-(int)GetRunTime();

    ui->Bootlabel->setText("系统启动时间: "+QDateTime::fromTime_t(boottime).toString("yyyy-MM-dd hh:mm:ss.zzz"));
}
void MainWindow::OsType(){
    std::string os= GetOsType();
    ui->Oslabel->setText("系统设置: "+QString::fromStdString(os));
}
void MainWindow::CpuType(){
    std::string cputype= GetCpuType();
    ui->cpulabel->setText("cpu型号: "+QString::fromStdString(cputype));
}
void MainWindow::addCpuPoint(double cpurate){
    int size =cpulist.size();
    if(size>120) cpulist.pop_front();
    if(size==0)
        for(int i=0;i<119;i++) cpulist.push_back(0);
    if(size>=1){
        cpulist.push_back(cpurate);
        emit UpdateCpuLine();
    }
    else cpulist.push_back(cpurate);
}
void MainWindow::UpdateCpuLine(){
    int count=0;
    QPixmap pix(600,160);
    QPainter painter(&pix);
    pix.fill(Qt::blue);
    QPen pen0;
    pen0.setColor(Qt::lightGray);
    painter.setPen(pen0);
    for(int i=1;i<4;i++) painter.drawLine(0,i*40,600,i*40);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter.setPen(pen);
    while(count<(cpulist.size()-1)){
        painter.drawLine(5*count,160-1.6*(cpulist.value(count)),5*count+5,160-1.6*(cpulist.value(count+1)));
        count++;
    }
    ui->cpulinelabel->setPixmap(pix);
}
void MainWindow::addMemPoint(double memrate){
    int size =memlist.size();
    if(size>120) memlist.pop_front();
    if(size==0)
        for(int i=0;i<119;i++) memlist.push_back(0);
    if(size>=1){
        memlist.push_back(memrate);
        emit UpdateMemLine();
    }
    else memlist.push_back(memrate);
}
void MainWindow::UpdateMemLine(){
    int count=0;
    QPixmap pix(600,160);
    QPainter painter(&pix);
    pix.fill(Qt::blue);
    QPen pen0;
    pen0.setColor(Qt::lightGray);
    painter.setPen(pen0);
    for(int i=1;i<4;i++) painter.drawLine(0,i*40,600,i*40);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    painter.setPen(pen);
    while(count<(memlist.size()-1)){
        painter.drawLine(5*count,160-1.6*(memlist.value(count)),5*count+5,160-1.6*(memlist.value(count+1)));
        count++;
    }
    ui->memlinelabel_2->setPixmap(pix);
}
void MainWindow::ProcessInfo(){
    ui->tableWidget->setColumnCount(6);
    QStringList headers;
    headers<<"进程名"<<"pid"<<"ppid"<<"内存占用/KB"<<"优先级"<<"操作";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    int proNum=GetProcessInfo();
    ui->tableWidget->setRowCount(proNum);
    for(int i=0;i<proNum;i++){
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(proinfo[i].name));
        ui->tableWidget->setItem(i,0,nameItem);
        nameItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *pidItem = new QTableWidgetItem(QString::fromStdString(proinfo[i].pid));
        ui->tableWidget->setItem(i,1,pidItem);
        pidItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *ppidItem = new QTableWidgetItem(QString::fromStdString(proinfo[i].ppid));
        ui->tableWidget->setItem(i,2,ppidItem);
        ppidItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *rssItem = new QTableWidgetItem(QString::fromStdString(proinfo[i].rss));
        ui->tableWidget->setItem(i,3,rssItem);
        rssItem->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *priorityItem = new QTableWidgetItem(QString::fromStdString(proinfo[i].priority));
        ui->tableWidget->setItem(i,4,priorityItem);
        priorityItem->setTextAlignment(Qt::AlignCenter);
        QPushButton *btn = new QPushButton;
        btn->setText("结束进程");
        btn->setStyleSheet("text-align:center;");
        ui->tableWidget->setCellWidget(i,5,btn);
        connect(btn,SIGNAL(clicked()),this,SLOT(RemovePro()));
    }
}
void MainWindow::RemovePro(){
    QPushButton *pushBtn =dynamic_cast<QPushButton*>(this->sender());
    if(pushBtn ==0) return ;
    int x=pushBtn->frameGeometry().x();
    int y=pushBtn->frameGeometry().y();
    QModelIndex index =ui->tableWidget->indexAt(QPoint(x,y));
    int row=index.row();
    int colum =index.column();
    qDebug()<<QObject::tr("row:%1,colum:%2").arg(row).arg(colum);
    killProcess(row);
}
void MainWindow::killProcess(int i){
    QString command;
    QString id=QString::fromStdString(proinfo[i]. pid);
    command = QString("kill %1").arg(id);
    QByteArray ba= command.toLatin1();
    system(ba.data());
    ProcessInfo();
}
MainWindow::~MainWindow()
{
    delete ui;
}
