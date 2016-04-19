#pragma once
#include "heap.h"
#include <assert.h>
//huffmanTree���Ľṹ
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
//huffmanTree�Ľṹ
template <class T>
class HuffmanTree
{
public:
	//����
	HuffmanTree()
		:_root(NULL)
	{

	}
	template <class T>
	struct NodeCompare
	{
		//����operator����
		bool operator()(const HuffmanTreeNode<T> *left,const HuffmanTreeNode<T> *right)
		{
			return left->_weight<right->_weight;
		}
	};
	//����
	void CreateTree(const T *t,size_t size,T &invalid)
	{
		//����t����Ч��
		assert(t);
		//��ѡ��������С�� 1��for(){.....} 2��Heap
		//1�����е�����Ϊhuffmantreenode 
		//2������ָ�룬���Խ�ʡ�ռ�
		//3����С��
		//<class T,template<class> class compare>
		//��Ϊheap�е�t������huffmanTreeNodeû��������͵ķ�����--adjustup�õ���
		//Ҫ�Լ�дһ��
		Heap<HuffmanTreeNode<T>*,NodeCompare<T>> minHeap;
		for(size_t i=0;i<size;i++)
		{
			//�����
			if(invalid==t[i])
			{
				continue;
			}
			HuffmanTreeNode<T>* node=new HuffmanTreeNode<T>(t[i]);
			//�ŵ���С����ȥ
			minHeap.Push(node);
		}
		//��HuffmanTree
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
