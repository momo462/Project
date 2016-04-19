#pragma once
#include <iostream>
#include <vector>
using namespace std;


template <class T,class compare=Less<T>>
//template <class T,template<class> class compare>
class Heap
{
public:
	Heap()
	{
	}
	Heap(T *a,size_t size)
		
	{
		for (size_t i=0;i<size;i++)
		{
			_array.push_back(a[i]);
		}
		for (size_t i=(size-2)/2;i>0;i--)
		{
			AdjustDown(i);
		}
	}
	void Pop()
	{
		swap(_array[0],_array[_array.size()-1]);
		_array.pop_back();
		for (size_t i=0;i<_array.size();i++)
		{
			AdjustDown(i);
		}
	}
	void Push(const T &t)
	{
		_array.push_back(t);
		AdjustUp(_array.size()-1);
	}
protected:
	void AdjustDown(int root)
	{
		compare con;
		int child=root*2+1;
		while(child<_array.size())
		{
			if (child+1<_array.size()&&con(_array[child+1],_array[child]))
			{
				child++;
			}
			if (compare()(_array[child],_array[root]))
			{
				swap(_array[child],_array[root]);
				root=child;
				child=root*2+1;
			}
			else
			{
				break;
			}
		}
	}
	void AdjustUp(int child)
	{
		int root=(child-1)/2;
		while(child>0)
		{
			if (compare()(_array[child],_array[root]))
			{
				swap(_array[child],_array[root]);
				child=root;
				root=(child-1)/2;
			}
			else
			{
				break;
			}
		}
	}
public:
	T& Top()
	{
		return _array.front();
	}
	//升序 1，）2，）3，）4，）5），6），7（排成大堆，堆顶和最后一个叶子交换，size--）
	size_t Size()
	{
		return _array.size();
	}
	
private:
	vector<T> _array;
	
};
template <class T>
struct Less
{
	bool operator()(const T &l,const T &r)
	{
		return l<r;
	}
};
template <class T>
struct Greater
{
	bool operator()(const T &l,const T &r)
	{
		return l>r;
	}
};
//void test()
//{
//	int a[10]={10,16,18,12,11,13,15,17,14,19};
//	//Heap<int,Less<int>> h1(a,10);// tempalte<class T,class compare=Less>---->compare con; con(l,r);/compare()(l,r);
//	//h1.Pop();
//	//h1.Push(6);
//	//Heap<int,Greater<int>> h2(a,10);
//	Heap<int,Less> h1(a,10);
//}
//template <class T>
//void AdjustDown(T *a,size_t size,size_t index)
//{
//	
//		size_t child=index*2+1;
//		while (child<size)
//		{
//			while (child+1<size&&a[child+1]>a[child])
//			{
//				child++;
//			}
//			if (a[child]>a[index])
//			{
//				swap(a[child],a[index]);
//				index=child;
//				child=index*2+1;
//			}
//			else
//			{
//				break;
//			}
//		
//	}
//}
//template <class T>
//void HeapSort(T *a,size_t size)
//{
//	//建大堆
//	for (int i=(size-2)/2;i>=0;i--)
//	{
//		AdjustDown(a,size,i);
//	}
//	//选择排序
//	for(size_t i=0;i<size;i++)
//	{
//		swap(a[0],a[size-1-i]);
//		AdjustDown(a,size-i-1,0);
//	}
//}
//
//void test1()
//{
//	int a[10]={10,16,18,12,11,13,15,17,14,19};
//	HeapSort(a,10);
//
//}


