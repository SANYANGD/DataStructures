
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMenu>
#include<QAction>
#include<QMainWindow>
#include<QFileDialog>

#include"compression.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //出现文件打开错误情况时让QLineEdit清空的槽函数
    void clear();

private slots:
    void myslot(double per);
    //3个按钮的Click槽函数加一个让QprogressBar动起来的槽函数
    void on_pushButton_decompression_clicked();
    void on_pushButton_open_clicked();
    void on_pushButton_compression_clicked();

private:
    Ui::MainWindow *ui;
    Compression* compression; //一个类指针，压缩类
    QString filepath; //读取文件路径，作为压缩函数和解压缩函数的参数
    
};

#endif // MAINWINDOW_H
