
/*
实现单链表的创建、查找操作。要求：
1）单链表中的元素可以从键盘输入，也可以从文件输入。输入后，分别创建原始单链表和有序的单链表。
2）有序的单链表内容存储到一个新的文本文件中。
3）查找操作分别在原始单链表和有序中完成，返回被找元素的位置（不能找到返回 -1）
4）用模板函数实现。要使用不同的数据类型（整数、实数、字符串等）验证你的模板函数的正确性 。
*/

#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<regex>
#include<cstdlib>
#include<deque>
using namespace std;

//定义
struct List {
    int val;  //存值
    struct List* next;  //存下一个元素的地址
};

//int* input() {
//    int ina[500];
//    int m = 0;
//    while (cin.peek() != '\n') {
//        cin >> ina[m++];
//    }
//    int* a = new int[m];
//    for (int i = 0; i < m; i++)
//        a[i] = ina[i];
//    return a;
//}

// 从控制台链表创建
void init(struct List* L) {
    int cur;
    for (int i = 0; cin.peek() != '\n';i++) {
        cin >> cur;
        struct List* ptr = (struct List*)malloc(sizeof(struct List));
        ptr->val = cur;
        ptr->next = NULL;
        L->next = ptr;
        L = L->next;
    }
}

//清除文件中的空行
int rm_BlankLine(string file) {
    fstream targetFile(file, fstream::in | fstream::out);
    string line;//作为读取的每一行
    string temp;//作为缓存使用
    deque<string> noBlankLineQueue;//双向队列,只在队首和队尾操作时性能较好
        //判断文件是否打开
    if (!targetFile) {
        cerr << "Can't Open File!" << endl;
        return EXIT_FAILURE;
    }
    //记录文件开始位置
    auto StartPos = targetFile.tellp();
    //循环读取判断文件的空行并放入队列
    while (getline(targetFile, line)) {
        //cout << targetFile.tellg() << endl;;
        if (line.empty()) {
           //cout << "此行为空" << endl;
        }
        else {
            //cout << "上一行是空行" << endl;
            noBlankLineQueue.push_back(line);
        }
    }
    targetFile.close();
    //使用ofstream打开再关闭文件是为了清空源文件
    ofstream emptyFile(file);
    emptyFile.close();
    //重新打开文本文件
    fstream target(file, fstream::out | fstream::in);
    if (!target) {
        cerr << "Can't Open File" << endl;
    }
    //获取队首和队尾
    auto begin = noBlankLineQueue.begin();
    auto end = noBlankLineQueue.end();
    //遍历双向队列中的元素
    //并写回文件
    while (begin != end) {
        temp = *begin;
        //cout << temp << endl;
        target << temp << endl;
        ++begin;
    }
    target.close();
    return EXIT_SUCCESS;
}

// 从文件创建列表
void readFile(struct List* L, string filename) {
    int datalen = 0;
    int a[100];
    ifstream file(filename);
    while (!file.eof()) {
        file >> a[datalen++];
    }
    datalen--;
    for (int i = 0; i < datalen; i++) {
        struct List* ptr = (struct List*)malloc(sizeof(struct List));
        ptr->val = a[i];
        ptr->next = NULL;
        L->next = ptr;
        L = L->next;
    }
    file.close();
}

//遍历列表
void show(struct List* L) {
    cout << "链表遍历:";
    while (L->next) {
        cout << L->next->val << " ";
        L = L->next;
    }
    cout << endl;
}

//按值查找列表
int find(struct List* head, int v){
    List* p = head->next;
    int i = 0;
    for (i = 0; p && p->val != v; i ++)//直到p为NULL或p->val为key为止
        p = p->next;
    if (p == NULL) {
        return -2;
    }
    else {
        return i;
    }
}

//列表排序
List* bubbleSort(List* head){
    if (head == NULL || head->next == NULL) return head;
    List* p = NULL;
    bool isChange = true;
    while (p != head->next->next && isChange)    {
        List* q = head->next;
        isChange = false; //标志当前这一轮中又没有发生元素交换，如果没有则表示数组已经有序
        for (; q->next && q->next != p; q = q->next)
            if (q->val > q->next->val)
                swap(q->val, q->next->val), isChange = true;
        p = q;
    }
    return head;
}

//列表写入文件
void toFile(List* L, string filename) {
    int myid = 1, procnum = 64;
    ofstream out(filename, ios::out);//app表示每次操作前均定位到文件末尾
    if (out.fail()) {
        cout << "error\n";
    }
    while (L->next) {
        L = L->next;
        out << L->val << endl;
    }
    out.close();
}

//控制台界面
int window() {
    struct List* head = (struct List*)malloc(sizeof(struct List));  //头结点(不存值)
    struct List* headorder = (struct List*)malloc(sizeof(struct List));  //头结点(不存值)
    head->next = NULL;
    headorder->next = NULL;
    
    int i = 0; int j = 0;
    cout << endl << "输入数字选择操作：" << endl
        << "1：链表元素从控制台输入，链表元素在输入1后一次性输入（即格式为：1 X X X ...）" << endl
        << "2：链表元素从文件list.txt输入" << endl
        << "其他：退出系统" << endl;
    cin >> i;
    if (i == 1) {
        init(head);
        rm_BlankLine("listin.txt");
        toFile(head, "listin.txt");
        readFile(head, "listin.txt");
        readFile(headorder, "listin.txt");
    }  
    else  if (i == 2) {
        rm_BlankLine("list.txt");
        readFile(head, "list.txt");
        readFile(headorder, "list.txt");
    }
    else {
        return 1;
    }

    bubbleSort(headorder);

    cout << "原始";
    show(head);
    cout << "有序";
    show(headorder);

    toFile(headorder, "listout.txt");
    cout << "有序单链表储存到文件listout.txt。" << endl;

    while (j==0) {
        cout << "是(0)否(1)需要查找元素：" << endl;
        cin >> j;
        if (j == 0) {
            int n = 0;
            cout << "输入要查询的元素数值：" << endl;
            cin >> n;
            cout << "元素在原始链表的位置：" << find(head, n) + 1 << endl;
            cout << "元素在有序链表的位置：" << find(headorder, n) + 1 << endl;
        }
    }

    return 0;
}

//程序入口
int main() {
    
    int i = 0;
    while (i==0) {
        i=window();
    }

    //struct List* head = (struct List*)malloc(sizeof(struct List));  //头结点(不存值)
    //head->next = NULL;
    //init(head);
    //bubbleSort(head);
    //show(head);

    return 0;
}