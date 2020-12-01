
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    resize(600,200);
    setWindowTitle("HuffmanCompression");
    compression=new Compression();
    connect(compression,&Compression::error,this,&MainWindow::clear);
    connect(compression,SIGNAL(mysignal(double)),this,SLOT(myslot(double)));
    ui->progressBar->setMaximum(100);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
    QMenu* pFile=ui->menuBar->addMenu("帮助");
    QAction* pNew=pFile->addAction("关于");
    connect(pNew,&QAction::triggered,[=](){QMessageBox::about(this,"关于","G01-Huffman压缩实验");});
}

MainWindow::~MainWindow() {
    delete ui;
    delete compression;
}

void MainWindow::clear() {
    ui->lineEdit->clear();
    filepath.clear();
}

void MainWindow::myslot(double per) {
    if(per>ui->progressBar->value())
        ui->progressBar->setValue(per);
}

void MainWindow::on_pushButton_open_clicked() {
    filepath=QFileDialog::getOpenFileName(this,QString("选择文件"));
    ui->lineEdit->setText(filepath);
}

void MainWindow::on_pushButton_compression_clicked() {
    compression->zip(filepath);
    ui->lineEdit->clear();
    this->filepath.clear();
    ui->progressBar->setValue(0);
}

void MainWindow::on_pushButton_decompression_clicked() {
    compression->unzip(filepath);
    ui->lineEdit->clear();
    this->filepath.clear();
    ui->progressBar->setValue(0);
}

