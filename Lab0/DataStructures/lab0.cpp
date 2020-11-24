
/*
ʵ�ֵ�����Ĵ��������Ҳ�����Ҫ��
1���������е�Ԫ�ؿ��ԴӼ������룬Ҳ���Դ��ļ����롣����󣬷ֱ𴴽�ԭʼ�����������ĵ�����
2������ĵ��������ݴ洢��һ���µ��ı��ļ��С�
3�����Ҳ����ֱ���ԭʼ���������������ɣ����ر���Ԫ�ص�λ�ã������ҵ����� -1��
4����ģ�庯��ʵ�֡�Ҫʹ�ò�ͬ���������ͣ�������ʵ�����ַ����ȣ���֤���ģ�庯������ȷ�� ��
*/

#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<regex>
#include<cstdlib>
#include<deque>
using namespace std;

//����
struct List {
    int val;  //��ֵ
    struct List* next;  //����һ��Ԫ�صĵ�ַ
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

// �ӿ���̨������
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

//����ļ��еĿ���
int rm_BlankLine(string file) {
    fstream targetFile(file, fstream::in | fstream::out);
    string line;//��Ϊ��ȡ��ÿһ��
    string temp;//��Ϊ����ʹ��
    deque<string> noBlankLineQueue;//˫�����,ֻ�ڶ��׺Ͷ�β����ʱ���ܽϺ�
        //�ж��ļ��Ƿ��
    if (!targetFile) {
        cerr << "Can't Open File!" << endl;
        return EXIT_FAILURE;
    }
    //��¼�ļ���ʼλ��
    auto StartPos = targetFile.tellp();
    //ѭ����ȡ�ж��ļ��Ŀ��в��������
    while (getline(targetFile, line)) {
        //cout << targetFile.tellg() << endl;;
        if (line.empty()) {
           //cout << "����Ϊ��" << endl;
        }
        else {
            //cout << "��һ���ǿ���" << endl;
            noBlankLineQueue.push_back(line);
        }
    }
    targetFile.close();
    //ʹ��ofstream���ٹر��ļ���Ϊ�����Դ�ļ�
    ofstream emptyFile(file);
    emptyFile.close();
    //���´��ı��ļ�
    fstream target(file, fstream::out | fstream::in);
    if (!target) {
        cerr << "Can't Open File" << endl;
    }
    //��ȡ���׺Ͷ�β
    auto begin = noBlankLineQueue.begin();
    auto end = noBlankLineQueue.end();
    //����˫������е�Ԫ��
    //��д���ļ�
    while (begin != end) {
        temp = *begin;
        //cout << temp << endl;
        target << temp << endl;
        ++begin;
    }
    target.close();
    return EXIT_SUCCESS;
}

// ���ļ������б�
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

//�����б�
void show(struct List* L) {
    cout << "�������:";
    while (L->next) {
        cout << L->next->val << " ";
        L = L->next;
    }
    cout << endl;
}

//��ֵ�����б�
int find(struct List* head, int v){
    List* p = head->next;
    int i = 0;
    for (i = 0; p && p->val != v; i ++)//ֱ��pΪNULL��p->valΪkeyΪֹ
        p = p->next;
    if (p == NULL) {
        return -2;
    }
    else {
        return i;
    }
}

//�б�����
List* bubbleSort(List* head){
    if (head == NULL || head->next == NULL) return head;
    List* p = NULL;
    bool isChange = true;
    while (p != head->next->next && isChange)    {
        List* q = head->next;
        isChange = false; //��־��ǰ��һ������û�з���Ԫ�ؽ��������û�����ʾ�����Ѿ�����
        for (; q->next && q->next != p; q = q->next)
            if (q->val > q->next->val)
                swap(q->val, q->next->val), isChange = true;
        p = q;
    }
    return head;
}

//�б�д���ļ�
void toFile(List* L, string filename) {
    int myid = 1, procnum = 64;
    ofstream out(filename, ios::out);//app��ʾÿ�β���ǰ����λ���ļ�ĩβ
    if (out.fail()) {
        cout << "error\n";
    }
    while (L->next) {
        L = L->next;
        out << L->val << endl;
    }
    out.close();
}

//����̨����
int window() {
    struct List* head = (struct List*)malloc(sizeof(struct List));  //ͷ���(����ֵ)
    struct List* headorder = (struct List*)malloc(sizeof(struct List));  //ͷ���(����ֵ)
    head->next = NULL;
    headorder->next = NULL;
    
    int i = 0; int j = 0;
    cout << endl << "��������ѡ�������" << endl
        << "1������Ԫ�شӿ���̨���룬����Ԫ��������1��һ�������루����ʽΪ��1 X X X ...��" << endl
        << "2������Ԫ�ش��ļ�list.txt����" << endl
        << "�������˳�ϵͳ" << endl;
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

    cout << "ԭʼ";
    show(head);
    cout << "����";
    show(headorder);

    toFile(headorder, "listout.txt");
    cout << "���������浽�ļ�listout.txt��" << endl;

    while (j==0) {
        cout << "��(0)��(1)��Ҫ����Ԫ�أ�" << endl;
        cin >> j;
        if (j == 0) {
            int n = 0;
            cout << "����Ҫ��ѯ��Ԫ����ֵ��" << endl;
            cin >> n;
            cout << "Ԫ����ԭʼ�����λ�ã�" << find(head, n) + 1 << endl;
            cout << "Ԫ�������������λ�ã�" << find(headorder, n) + 1 << endl;
        }
    }

    return 0;
}

//�������
int main() {
    
    int i = 0;
    while (i==0) {
        i=window();
    }

    //struct List* head = (struct List*)malloc(sizeof(struct List));  //ͷ���(����ֵ)
    //head->next = NULL;
    //init(head);
    //bubbleSort(head);
    //show(head);

    return 0;
}