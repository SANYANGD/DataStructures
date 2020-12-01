#pragma once
#include<vector>
#include<string>
using namespace std;

// 哈夫曼树结点的定义
struct HuffmanNode {	
	// 待编码的符号	
	char data;	
	// 符号出现的频率 
	double weight;		
	// 父结点、左、右孩子结点的位置
	int parent, lchild, rchild;	
};

class HuffmanTree {
private:
	// 树中所有结点的存储空间
	vector<HuffmanNode> hufftree;	
	// 叶子结点数
	int n;		
	//选择一组数据中最小的和次小的数
	void SelectSmall(int &least, int &less, int i);
public:
	//构造函数
	HuffmanTree(vector<HuffmanNode> &leafs);
	//huffman编码算法
	vector<int>GetCode(int i);
	//huffman译码算法
	void Decode(vector<int> &source, vector<char>&result);
};

