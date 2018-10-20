#ifndef MAINWINDOW3_H
#define MAINWINDOW3_H

#include <QMainWindow>

namespace Ui {
class MainWindow3;
}

class MainWindow3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow3(QWidget *parent = 0);
    ~MainWindow3();
public slots:
    void action();
private:
    Ui::MainWindow3 *ui;
    QTimer *timer ;
    int i;
    int sum;
};
