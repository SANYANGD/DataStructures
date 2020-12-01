#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QFileDialog>
#include<QMenu>
#include<QAction>

#include"compression.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void clear();

private slots:
    void on_pushButton_open_clicked();
    void on_pushButton_compression_clicked();
    void on_pushButton_decompression_clicked();
    void myslot(double per);

private:
    Ui::MainWindow *ui;
    Compression* compression;
    QString filepath;
    
};

#endif // MAINWINDOW_H
