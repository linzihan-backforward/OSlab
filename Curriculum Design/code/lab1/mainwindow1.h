#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>

namespace Ui{
class MainWindow1;

}
class MainWindow1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow1(QWidget *parent = 0);
    ~MainWindow1();
public slots:
    void action();
private:
    Ui::MainWindow1 *ui;
    QTimer *timer ;
};
#endif // MAINWINDOW1_H
