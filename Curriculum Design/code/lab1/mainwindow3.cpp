#include "mainwindow3.h"
#include "ui_mainwindow3.h"
#include<QTimer>
MainWindow3::MainWindow3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow3)
{
    timer=new QTimer();
    i=1;
    ui->setupUi(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(action()));
    timer->start(1000);
}
void MainWindow3::action(){
    sum+=i;
    ui->label->setText("now:"+QString::number(i, 10)+",sum:"+QString::number(sum, 10));
    i++;
    if(i==1001) i=0;
}
MainWindow3::~MainWindow3()
{
    delete ui;
}
