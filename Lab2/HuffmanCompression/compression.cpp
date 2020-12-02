
#include "compression.h"


//哈夫曼结点间比较的依据，作为sort函数的参数
bool compare(HuffmanTreeNode* node1, HuffmanTreeNode* node2) {
    return node1->Weight.second < node2->Weight.second;
}


Compression::Compression(QWidget *parent) : QMainWindow(parent) { }

//yuanString按照哈夫曼编码映射初始化类中的binString
void Compression:: binStringInit() {
    for (int i = 0; i < yuanString.size(); i++) {
        binString+= pwMap[yuanString[i]];
    }
}

//权重映射的初始化，涉及QFile的操作，和C++的fstream相似
void Compression::weightMapInit(QFile& input) {
    QByteArray arr;
    while (!input.atEnd()) {
        arr=input.read(1024);
        string x=arr.toStdString();
        for(int i=0;i<x.size();i++) {
            unsigned char ch=x[i];
            yuanString += ch;
            weightMap[ch]++;
        }
    }    
}

//初始化结点容器
void Compression::HTVectorInit() {
    for (map<unsigned char, int>::iterator it = weightMap.begin(); it != weightMap.end(); it++) {
        HuffmanTreeNode* h = CreatHTNode(it->first);
        container.push_back(h);
    }
 }

//建立哈夫曼树
void Compression::HTInit() {
    while (container.size() != 1) {
        sort(container.begin(), container.end(), compare);
        int sum = container[0]->Weight.second + container[1]->Weight.second;
        HuffmanTreeNode* h = new HuffmanTreeNode;
        h->Weight.second = sum;
        h->Weight.first = 0;
        h->tag = 1;
        //标记为非叶子结点
        h->leftson = container[0];
        h->rightson = container[1];
        container.erase(container.begin());
        //删除后迭代器会顺移到下一位
        container.erase(container.begin());
        container.push_back(h);
    }
 }

//递归更新哈夫曼编码函数,每次递归返回时password进行一次pop操作
void Compression::zipPWInit(HuffmanTreeNode* &root, string& pw) {
    if (root != NULL && !root->tag) {
        root->zipPW = pw;
        pwMap[root->Weight.first] = pw;
    }
    if (root->leftson != NULL) {
        zipPWInit(root->leftson, pw += "0");
        pw.pop_back();
    }
    if (root->rightson != NULL) {
        zipPWInit(root->rightson, pw+="1");
        pw.pop_back();
    }
 }

//建立哈夫曼结点函数：进行初始化操作
HuffmanTreeNode* Compression::CreatHTNode(int i) {
    HuffmanTreeNode* h = new HuffmanTreeNode;
    h->Weight.first = i;
    h->Weight.second = weightMap[i];
    h->tag = 0;
    h->leftson = NULL;
    h->rightson = NULL;
    return h;
}

//删除整颗哈夫曼树
void Compression::deleteHuffmanTree(HuffmanTreeNode*& root) {
    if(root==NULL) return;
    deleteHuffmanTree(root->leftson);
    deleteHuffmanTree(root->rightson);
    delete root;
}


//将二进制串转为int型，即将压缩时每8个二进制位的字符串转化为ASCII码，传入压缩后的文件
int binToInt(string binstr) {
    int sum = 0;
    for (int i = 0; i < binstr.size(); i++) {
        if (binstr[i] == '1') {
            int j = pow(2, binstr.size() - i - 1);
            sum += j;
        }
    }
    return sum;
}

//int转化为string，上面函数的反操作，解压时使用
string intToBin(int v) {
    string binstr;
    while (v > 0) {
        int r = v % 2;
        if (r == 1)binstr.insert(0, 1, '1');
        else binstr.insert(0, 1, '0');
        v = v / 2;
    }
    if (binstr.size() < 8) {
        binstr.insert(0, 8 - binstr.size(), '0');
    }
    return binstr;
}



//压缩函数
void Compression::zip(QString fpath) {
     clock_t star=clock();//记录开始时间
     QFile openfile(fpath);//创建QFile
     //打开文件,若不成功，结束
     if(!openfile.open(QIODevice::ReadOnly)) {
         QMessageBox::information(NULL,QString("警告"),QString("文件打开失败"));
         emit error();
         return;
     }
     //权重映射初始化
     weightMapInit(openfile);
     //更新进度条
     emit mysignal(10);
     //哈夫曼结点容器初始化
     HTVectorInit();
     emit mysignal(20);
     HTInit();//构建哈夫曼树
     emit mysignal(30);
     string empty="";
     zipPWInit(container[0],empty);//哈夫曼编码映射初始化
     emit mysignal(40);
     binStringInit();//获取二进制串
     emit mysignal(50);
     fpath+=".hfc";//压缩后的文件格式后缀
     openfile.close();//关闭openfile
     QFile savefile(fpath);//创建新的Qfile进行压缩文件的写入
     savefile.open(QIODevice::WriteOnly);
     QDataStream out(&savefile);//利用QdataStream操作
     int size = pwMap.size();
         if (size == 256) size = 0;//因为256无法用一个字节保存
         int length = 0;
         out<<size;//传入哈夫曼编码映射数量
         length++;
         double k=1;
         for (map<unsigned char, string>::iterator it = pwMap.begin(); it != pwMap.end(); it++) {
             emit mysignal(50+double(25*k++)/pwMap.size());
             int first = it->first;
             out<<first;//传入key值
             length++;
             string second = it->second;
             int size = second.size();
             out<<size;//传入value值的长度
             length++;
             int n = 8 - second.size() % 8;
             if (n) {
                 second.append(n, '0');//补0
             }
             for (int i = 0; i < second.size(); i += 8) {
                 string k = second.substr(i, 8);
                 int temp = binToInt(k);
                 unsigned char ch = temp;
                 out<<ch;//传入value值
                 length++;
             }
         }
         int n = 8 - binString.size() % 8;
         if (n) {
             binString.append(n, '0');
         }
         length++;
         int totalbitsize = binString.size() / 8;
         for (int i = 0; i < binString.size(); i += 8) {
             emit mysignal(75+double(25*i)/binString.size());
             string k = binString.substr(i, 8);
             int temp = binToInt(k);
             unsigned char ch = temp;
             out<<ch;//分8个字节依次传入
             length++;
         }
         unsigned char temp=n;
         out<<temp;//传入补0数量
         length++;
         emit mysignal(100);
         int newlength=savefile.size();
         savefile.close();
         clock_t end=clock();
         QString tip("理论压缩比为:");
         tip+=QString::number(double(totalbitsize * 100) / yuanString.size());
         tip+="%\n实际压缩比为:";
         tip+=QString::number(double(newlength * 100) / yuanString.size());
         tip+="%\n压缩时间为:";
         tip+=QString::number(double(end-star)/CLOCKS_PER_SEC);
         tip+="s";
         QMessageBox::about(this,"压缩说明",tip);
         weightMap.clear();
         pwMap.clear();
         yuanString.clear();
         binString.clear();
         deleteHuffmanTree(container[0]);
         container.clear();
 }

//解压函数
void Compression::unZip(QString fpath) {
    clock_t star=clock();
    if(fpath.right(4)!=".hfc") {
        QMessageBox::information(NULL,QString("警告"),QString("此文件非哈夫曼压缩文件，打开失败"));
        emit error();
        return;
    }
    QFile openfile(fpath);
    if(!openfile.open(QIODevice::ReadOnly)) {
        QMessageBox::information(NULL,QString("警告"),QString("文件打开失败"));
        emit error();
        return;
    }
    QDataStream in(&openfile);
    int zipmapsize;
    in>>zipmapsize;
    if (zipmapsize == 0) zipmapsize = 256;
    for (int i = 1; i <= zipmapsize; i++) {
        int zipkey;
        in>>zipkey;
        int valuelength;
        in>>valuelength;
        string valuestring;
        for (int i = 1; i <= valuelength / 8 + 1; i++) {
            emit mysignal(double(20*i)/(valuelength/8 +1));
            unsigned char ch;
            in>>ch;
            int zipvalue=ch;
            valuestring += intToBin(zipvalue);
        }
        valuestring.erase(valuelength, valuestring.size() - valuelength + 1);
        zipPW[valuestring] = zipkey;
    }
    string zipstring;
    while (!in.atEnd()) {
        unsigned char ch;
        in>>ch;
        int bit=ch;
        zipstring += intToBin(bit);
    }
    emit mysignal(50);
    int zerosize = binToInt(zipstring.substr(zipstring.size() - 8));
    zipstring.erase(zipstring.size() - zerosize - 8, zerosize + 8);
    openfile.close();
    fpath.chop(4);
    fpath.insert(fpath.lastIndexOf('.'),"(New)");
    QFile savefile(fpath);
    savefile.open(QIODevice::WriteOnly);
    string str;
    for (int i = 0; i < zipstring.size(); i++) {
        emit mysignal(50+double(50*i)/zipstring.size());
        str += zipstring[i];
        map<string, int>::iterator it = zipPW.find(str);
        if (it != zipPW.end()) {
            unsigned char temp=it->second;
            savefile.write(reinterpret_cast<char*>(&temp),1);
            str.clear();
        }
    }
    emit mysignal(100);
    savefile.close();
    clock_t end=clock();
    QString tip="解压用时";
    tip+=QString::number(double(end-star)/CLOCKS_PER_SEC);
    tip+="s";
    QMessageBox::about(this,"解压说明",tip);
    zipPW.clear();
 }
