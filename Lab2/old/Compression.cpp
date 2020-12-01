
#include <iostream>
#include<fstream>
#include<time.h>
#include"huffmantree.h"
using namespace std;
const int MAX = 255;

//计算频率
void Count(int *count,unsigned char *buf,int filelen){
	for (int i = 0; i < MAX; i++)
		count[i] = 0;
	//记录每个8位二进制串出现次数
	for (int i = 0; i < filelen; i++)
		count[buf[i]]++;
}

//将8位01序列转换为ASCII码
char Transinttochar(int* buffer) {
	char x;
	x = buffer[0] * 128 + buffer[1] * 64 + buffer[2] * 32 + buffer[3] * 16 + buffer[4] * 8 + buffer[5] * 4 + buffer[6] * 2 + buffer[7] * 1;
	return x;
}

//将数字转换为二进制
void Transchartoint(int x, int* a) {
	for (int i = 7; i >= 0; i--) {
		a[i] = x % 2;
		x = x / 2;
	}
}

//建立huffman树
HuffmanTree BuildHuffTree(int *count,int filelen,unsigned char *buf){
	for (int i = 0; i < MAX; i++)
		count[i] = 0;
	//记录每个8位二进制串出现次数
	for (int i = 0; i < filelen; i++)
		count[buf[i]]++;
	//建立huffman树
	vector< HuffmanNode> leafs(MAX);//对每个ASCII码进行huffman编码
	for (int i = 0; i < 255; i++) {//每个结点的符号
		leafs[i].data = i;
	}
	for (int i = 0; i < MAX; i++)//每个结点的权值，为其出现的频率
		leafs[i].weight = count[i];
	HuffmanTree tree1(leafs);//建立huffman树
	return tree1;
}

//获得编码
vector<vector<int>> GetandOutputcode(HuffmanTree tree1,int filelen,unsigned char*buf){
	//对文件中字符进行编码
	vector<vector<int>> code(MAX);
	for (int i = 0; i < MAX; i++){
		code[i] = tree1.GetCode(i);
	}
	//输出编码到文件B
	/*ofstream file2("B1.txt");
	if (!file2.is_open())
	{
		cout << "文件B1打开失败！";
		exit(1);
	}
	//展示编码
	for (int i = 0; i < filelen; i++)
	{
		int x;//code下标
		x = buf[i];
		for (unsigned int j = 0; j < code[x].size(); j++)
		{
			file2 << code[x][j];
			//source.push_back(code[x][j]);
		}
	}
	file2.close();*/
	return code;
}
                
//对B进行译码并写入文件C
int Translate(HuffmanTree tree1){
	FILE *file1 = NULL;
	file1 = fopen("B.txt", "rb");
	if (file1 == NULL)	{
		cout << "文件B打开失败！";
		exit(1);
	}

	//获得文件长度
	fseek(file1, 0, SEEK_END); 
	int filelenB = ftell(file1); 
	rewind(file1);
	//新建缓冲区，存储读出的数据
	unsigned char *buf1 = (unsigned char *)malloc(filelenB);
	//将缓冲区的数据设置为0
	for (int i = 0; i < filelenB; i++)
		buf1[i] = 0;
	fread(buf1, 1, filelenB, file1);
	fclose(file1);

	//对编码进行译码
	//读出的编码
	vector<int>source1;
	//储存数字
	int a[8];
	for (int i = 0; i < 8; i++)
		a[i] = 0;
	for (int i = 0; i < filelenB-2; i++){
		//如果遇到回车换行符就略过回车符
		if (buf1[i] == 13 && buf1[i + 1] == 10)
			continue;
		Transchartoint(buf1[i],a);
		for (int j = 0; j < 8; j++)
			source1.push_back(a[j]);
	}
	//倒数第二位是一个字符，最后一位是缺位数，字符的二进制只读到缺位数
	int lackcount = buf1[filelenB - 1];
	Transchartoint(buf1[filelenB-2], a);
	for (int j = 0; j < lackcount; j++)
		source1.push_back(a[j]);
	//输入文件C
	ofstream file3("C.txt", ios::binary);
	if (!file3.is_open()){
		cout << "文件C打开失败！";
		exit(1);
	}
	vector<char> result;
	tree1.Decode(source1, result);
	for (unsigned int i = 0; i < result.size(); i++){
		file3 << result[i];
	}
	file3.close();
	return filelenB;
}

//压缩编码
void Binarycode(int filelen, unsigned char* buf, vector<vector<int>>& code) {
	ofstream file4("B.txt");
	if (!file4.is_open()) {
		cout << "文件B打开失败！";
		exit(1);
	}
	int buffer[8];
	for (int i = 0; i < 8; i++)
		buffer[i] = 0;
	int bufcount = 0;
	//输出编码到文件B
	for (int i = 0; i < filelen; i++) {
		int x;
		x = buf[i];
		for (unsigned int j = 0; j < code[x].size(); j++) {
			if (bufcount == 8) {
				file4 << Transinttochar(buffer);
				bufcount = 0;
			}
			buffer[bufcount] = code[x][j];
			bufcount++;
		}
	}
	//最后一位
	if (bufcount != 0) {
		file4 << Transinttochar(buffer);
	}
	//缺位数，以char的形式放最后
	char lackcount = bufcount;
	file4 << lackcount;
	file4.close();
}

//直接对B编码解码并写入D
/*void Write(HuffmanTree tree1, vector<int> &source){
	//对编码进行译码并输入文件C
	ofstream file3("D.txt", ios::binary);
	if (!file3.is_open()){
		cout << "文件D打开失败！";
		exit(1);
	}
	vector<char> result;
	tree1.Decode(source, result);
	for (unsigned int i = 0; i < result.size(); i++){
		file3 << result[i];
	}
	file3.close();
}*/

void run() {
	//读取文件名
	cout << "请输入要压缩的文件名：";
	char name[MAX];
	cin.getline(name, MAX);

	//计算程序执行时间
	clock_t start, finish;
	double totaltime;
	start = clock();

	//读取文件中的字符
	FILE* file1 = NULL;
	file1 = fopen(name, "rb");
	if (file1 == NULL) {
		cout << "文件打开失败！";
		exit(1);
	}
	cout << "文件打开成功" << endl;

	//获得文件长度
	fseek(file1, 0, SEEK_END);
	int filelen = ftell(file1);
	rewind(file1);
	//新建缓冲区，存储读出的数据
	unsigned char* buf = (unsigned char*)malloc(filelen);
	//将缓冲区的数据设置为0
	for (int i = 0; i < filelen; i++)
		buf[i] = 0;
	fread(buf, 1, filelen, file1);
	fclose(file1);
	cout << "文件读取完毕" << endl;
	//记录每个编码出现次数
	int count[MAX];
	//计算频率
	Count(count, buf, filelen);

	HuffmanTree tree1 = BuildHuffTree(count, filelen, buf);
	cout << "huffman树已建立" << endl;
	//获取编码
	vector<vector<int>>code(MAX);
	for (int i = 0; i < MAX; i++)
		code[i] = tree1.GetCode(i);
	//code= GetandOutputcode(tree1, filelen, buf);//获得并输出编码
	//cout << "编码以01序列的形式储存在B1……" << endl;

	//输出压缩编码
	Binarycode(filelen, buf, code);
	cout << "压缩编码已存入B" << endl;
	//对B1进行译码并写入文件C
	int filelenB = Translate(tree1);
	cout << "译码已存入文件C" << endl;
	double rate = (double)filelenB / filelen;
	cout << "压缩比为：" << rate << endl;
	//Write(tree1, source);
	free(buf);

	//输出程序运行时间
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "此程序的运行时间为" << totaltime << "秒！" << endl;
}


int main() {
	run();
	return 0;
}
