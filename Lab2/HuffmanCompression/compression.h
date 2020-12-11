
#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <QDataStream>
#include <QString>
#include <ctime>
#include <QMainWindow>
#include <QMessageBox>
#include <QFile>

using namespace std;

//哈夫曼树结点
struct HuffmanTreeNode
{
    pair<unsigned char, int> Weight; //记录字符和其权重
    string zipPW;                    //等待获取的哈夫曼编码
    HuffmanTreeNode *leftson;        //左儿子指针
    HuffmanTreeNode *rightson;       //右儿子指针
    bool tag;                        //是否为叶子结点的标记，true为非叶子结点
};

class Compression : public QMainWindow
{

    Q_OBJECT

public:
    explicit Compression(QWidget *parent = nullptr);
    void zip(QString path);   //压缩函数
    void unZip(QString path); //解压函数

protected:
    //在每次压缩后需要delete掉申请空间的哈夫曼树指针，采用递归遍历进行delete
    void deleteHuffmanTree(HuffmanTreeNode *&root);
    //二进制字符串的初始化，即从传入文件得到的字符串按照哈夫曼编码获取二进制字符串
    void binStringInit();
    //参数是QFile型，是Qt的文件操作，该函数打开传入文件，遍历，形成一个字符串，并且初始化WeightMap
    void weightMapInit(QFile &in);
    //将创建的哈夫曼结点放入Vector中
    void HTVectorInit();
    //建立哈夫曼树的函数
    void HTInit();
    //对建成的哈夫曼树进行遍历，更新每个结点哈夫曼编码
    void zipPWInit(HuffmanTreeNode *&root, string &password);
    //创建单个哈夫曼树结点
    HuffmanTreeNode *CreatHTNode(int i);

private:
    string yuanString;                   //读取传入文件形成的字符串
    string binString;                    //通过哈夫曼编码映射形成的二进制字符串
    map<unsigned char, int> weightMap;   //权重
    map<unsigned char, string> pwMap;    //哈夫曼编码映射
    map<string, int> zipPW;              //解压时建立的解码映射
    vector<HuffmanTreeNode *> container; //哈夫曼结点储存的容器

signals:
    void error(); //信号
    void mysignal(double per);

public slots:
};

#endif // COMPRESSION_H
