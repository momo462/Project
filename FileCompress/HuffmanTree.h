#pragma once
#include "heap.h"
#include <assert.h>
//huffmanTree结点的结构
template <class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode *_left;
	HuffmanTreeNode *_right;
	HuffmanTreeNode *_parent;
	T _weight;
	//constructor
	HuffmanTreeNode(const T &x)
		:_weight(x)
		,_left(NULL)
		,_parent(NULL)
		,_right(NULL)
	{

	}

};
//huffmanTree的结构
template <class T>
class HuffmanTree
{
public:
	//构造
	HuffmanTree()
		:_root(NULL)
	{

	}
	template <class T>
	struct NodeCompare
	{
		//重载operator（）
		bool operator()(const HuffmanTreeNode<T> *left,const HuffmanTreeNode<T> *right)
		{
			return left->_weight<right->_weight;
		}
	};
	//建树
	void CreateTree(const T *t,size_t size,T &invalid)
	{
		//断言t的有效性
		assert(t);
		//先选出两个最小数 1、for(){.....} 2、Heap
		//1、堆中的类型为huffmantreenode 
		//2、？是指针，可以节省空间
		//3、建小堆
		//<class T,template<class> class compare>
		//因为heap中的t类型是huffmanTreeNode没有这个类型的防函数--adjustup用到的
		//要自己写一个
		Heap<HuffmanTreeNode<T>*,NodeCompare<T>> minHeap;
		for(size_t i=0;i<size;i++)
		{
			//创结点
			if(invalid==t[i])
			{
				continue;
			}
			HuffmanTreeNode<T>* node=new HuffmanTreeNode<T>(t[i]);
			//放到最小堆中去
			minHeap.Push(node);
		}
		//建HuffmanTree
		while (minHeap.Size()>1)
		{
			HuffmanTreeNode<T> *left=minHeap.Top();
			minHeap.Pop();
			HuffmanTreeNode<T> *right=minHeap.Top();
			minHeap.Pop();
			HuffmanTreeNode<T> *parent=new HuffmanTreeNode<T>(left->_weight+right->_weight);
			parent->_left=left;
			parent->_right=right;
			left->_parent=parent;
			right->_parent=parent;
			minHeap.Push(parent);
		}
		_root=minHeap.Top();
	}
	
public:
	HuffmanTreeNode<T> *_root;
};

void testh()
{
	int a[10]={0,1,2,3,4,5,6,7,8,9};
	HuffmanTree<int> h;
	int b=-1;
	h.CreateTree(a,10,b);
}
