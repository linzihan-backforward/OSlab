#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include<QTimer>
MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2)
{
    timer=new QTimer();
    ui->setupUi(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(action()));
    timer->start(1000);
}
void MainWindow2::action(){

    ui->label->setText(QString::number(cnt%10, 10));
    cnt++;
}
MainWindow2::~MainWindow2()
{
    delete ui;
}
