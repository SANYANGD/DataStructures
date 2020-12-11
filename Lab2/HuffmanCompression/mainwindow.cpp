
#pragma execution_character_set(“utf - 8”)

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *p) : QMainWindow(p), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(600, 200);                                                           //设置窗口大小
    setWindowTitle("HuffmanCompression");                                       //设置窗口名称
    compression = new Compression();                                            //初始化压缩类指针
    connect(compression, &Compression::error, this, &MainWindow::clear);        //异常情况的信号接收
    connect(compression, SIGNAL(mysignal(double)), this, SLOT(myslot(double))); //让QProgressBar随着压缩/解压缩进度更新
    ui->progressBar->setMaximum(100);                                           //QProgressBar的最大值设为100
    ui->progressBar->setMinimum(0);                                             //QProgressBar的最小值设为0
    ui->progressBar->setValue(0);                                               //QProgressBar显示的值初始化为0
    QMenu *pFile = ui->menuBar->addMenu("帮助");                                //添加菜单栏
    QAction *pNew = pFile->addAction("关于");
    connect(pNew, &QAction::triggered, [=]() { QMessageBox::about(this, "关于", "G01-Huffman压缩实验"); }); //点击时弹出的窗口
}

//UI界面“选择”按钮的槽函数，更新path和lineEdit内容
void MainWindow::on_pushButton_open_clicked()
{
    filepath = QFileDialog::getOpenFileName(this, QString("选择文件"));
    ui->lineEdit->setText(filepath);
}

//UI界面“压缩”按钮的槽函数，将path作为参数传给压缩类的压缩函数，压缩完毕后，清除lineEdit，path，progressBar
void MainWindow::on_pushButton_compression_clicked()
{
    compression->zip(filepath);
    ui->lineEdit->clear();
    this->filepath.clear();
    ui->progressBar->setValue(0);
}

// UI界面“解压”按钮的槽函数，思想与前者相同
void MainWindow::on_pushButton_decompression_clicked()
{
    compression->unZip(filepath);
    ui->lineEdit->clear();
    this->filepath.clear();
    ui->progressBar->setValue(0);
}

//在文件打开失败的情况下清空所有
void MainWindow::clear()
{
    ui->lineEdit->clear();
    filepath.clear();
}

//per是进度，信号是由ComPreesion类指针发出，这是槽函数
void MainWindow::myslot(double per)
{
    if (per > ui->progressBar->value())
        ui->progressBar->setValue(per);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete compression;
}
