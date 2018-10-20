#include "mainwindow1.h"
#include "ui_mainwindow1.h"
#include<QTimer>
#include<QDateTime>
MainWindow1::MainWindow1(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow1)
{
    timer=new QTimer();
    ui->setupUi(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(action()));
    timer->start(1000);
}
void MainWindow1::action(){
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    ui->label->setText(current_date);
}
MainWindow1::~MainWindow1()
{
    delete ui;
}
