#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QList<double> cpulist;
    QList<double> memlist;
    void HostName();
    void BootTime();
    void OsType();
    void CpuType();
    void addCpuPoint(double);
    void addMemPoint(double);
    void killProcess(int);
public slots:
    void RunTime();
    void RemovePro();
    void NowTime();
    void CpuRate();
    void MemRate();
    void ProcessInfo();
    void UpdateCpuLine();
    void UpdateMemLine();
private:
    Ui::MainWindow *ui;
    QTimer *timer;
};

#endif // MAINWINDOW_H
