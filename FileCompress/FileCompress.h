#pragma once
#include "HuffmanTree.h"

typedef long LongType;
struct FileInfo
{
	unsigned char _ch;//�������ַ�
	LongType _count;//���ִ���
	string _code;//�������ַ���huffman����
	FileInfo(unsigned char ch=0)
		:_ch(ch)
		,_count(0)
	{

	}
	bool operator<(const FileInfo & info)const
	{
		return _count<info._count;
	}
	FileInfo operator+(const FileInfo & info)
	{
		FileInfo temp;
		temp._count= _count+info._count;
		return temp;
	}
	bool operator==(const FileInfo &info)
	{
		return _count==info._count;
	}
};

class FileCompress
{
public:
	//����
	FileCompress()
	{
		for(int i=0;i<256;i++)
		{
			_infos[i]._ch=(unsigned char)i;
		}
	}
	//����
	~FileCompress()
	{

	}
	//�ļ�ѹ��
	void Compress(const char * filename)
	{
		//1�����ļ���ȡ���ַ����ֵĴ���
		assert(filename);
		//�����ƶ����ѹ���ļ�
		FILE * fOut=fopen(filename,"rb");
		assert(fOut);
		int ch=fgetc(fOut);
		while (ch!=EOF)
		{
			_infos[(unsigned char)ch]._count++;
			//_infos[(unsigned char)ch]._ch=(unsigned char)ch;
			ch=fgetc(fOut);
		}
		//2�����ɶ�Ӧ��Huffman����
		HuffmanTree<FileInfo> h;
		FileInfo invalid;
		h.CreateTree(_infos,256,invalid);
		_GenerateHuffCode(h._root);
		//3��ѹ���ļ�
		fseek(fOut,0,SEEK_SET);
		ch=fgetc(fOut);
		string compress=filename;
		compress+=".compress";
		FILE *fIn=fopen(compress.c_str(),"wb");
		assert(fIn);
		unsigned char c=0;
		int index=0;
		while(!feof(fOut))
		{
			string &code=_infos[(unsigned char)ch]._code;
			//��codeһλһλ�Ķ�
			for(size_t i=0;i<code.size();i++)
			{
				c<<=1;
				if(code[i]=='1')
				{
					c|=1;
				}
				index++;
				if(index==8)
				{
					fwrite(&c,1,1,fIn);
					//fputc(c,fIn);
					index=0;
					c=0;
				}
			}
			fread(&ch,1,1,fOut);
		}
		//�������ʱ�򲻹�8λ����0����
		if(index!=0)
		{
			c=c<<(8-index);
			//cout<<"****"<<(int)c<<endl;
			fputc(c,fIn);
		}
		fclose(fIn);
		fclose(fOut);
		//4��д�����ļ������������ѹ��
		string config=filename;
		config+=".config";
		FILE *fInc=fopen(config.c_str(),"wb");
		assert(fInc);
		for(size_t i=0;i<256;i++)
		{
			if(_infos[i]._count!=0)
			{
				fwrite(&_infos[i]._ch,1,1,fInc);
				unsigned char point=',';
				fwrite(&point,1,1,fInc);
				fwrite(&_infos[i]._count,4,1,fInc);
				unsigned char point1='\n';
				fwrite(&point1,1,1,fInc);
			}
		}
		fclose(fInc);
	}
	//��ѹ
	void UnCompress(const char* filename)
	{
		assert(filename);
		string config=filename;
		config+=".config";
		//��ȡ�����ļ�(�����ƶ�)
		FILE *fOut=fopen(config.c_str(),"rb");
		assert(fOut);
		LongType ch=0;
		unsigned char c=0;
		fread(&ch,1,1,fOut);
		long long size=0;
		bool flag=true;
		while(!feof(fOut))
		{
			if(flag)
			{
				c=(unsigned char)ch;
				flag=false;
				fread(&ch,1,1,fOut);
				fread(&ch,4,1,fOut);
				size+=ch;
			}
			else
			{
				_infos[c]._count=ch;
				flag=true;
				fread(&ch,1,1,fOut);
				fread(&ch,1,1,fOut);
			}
		}
		fclose(fOut);
		//����
		HuffmanTree<FileInfo> h1;
		FileInfo invalid;
		h1.CreateTree(_infos,256,invalid);
		//����compress�ļ�ȥ������
		string compress=filename;
		compress+=".compress";
		FILE *fOut1=fopen(compress.c_str(),"rb");
		assert(fOut1);
		//1�����������ļ����ҵ�Ҷ�ӽ��
		//2����Ҷ�ӽ�������д����һ�����ļ�
		string uncompress="uncompress";
		uncompress+=filename;
		FILE *fIn1=fopen(uncompress.c_str(),"wb");
		assert(fIn1);
		//char ch1=fgetc(fOut1);
		int ch1=0;
		fread(&ch1,1,1,fOut1);
		int cha=0;
		HuffmanTreeNode<FileInfo> *cur=h1._root;
		while(!feof(fOut1))
		{ 
			for(int index=0;index<8;index++)
			{
				cha=((ch1>>(7-index))&1);
				if(cur->_left==NULL&&cur->_right==NULL)
				{
					if(size--==0)
					{
						return;
					}
					//fputc(_infos[cur->_weight._ch]._ch,fIn1);
					fwrite(&_infos[cur->_weight._ch]._ch,1,1,fIn1);
					cur=h1._root;
				}
				if (cha==0)
				{
					cur=cur->_left;	
				}
				if (cha==1)
				{
					cur=cur->_right;
				}
			}
			fread(&ch1,1,1,fOut1);
		}
		
	}
protected:
	
	void _GenerateHuffCode(HuffmanTreeNode<FileInfo>* root)
	{
	
		if (root==NULL)
		{
			return;
		}
		_GenerateHuffCode(root->_left);
		_GenerateHuffCode(root->_right);
		//���ҵ�Ҷ�ӽ��
		if(root->_left==NULL&&root->_right==NULL)
		{
			HuffmanTreeNode<FileInfo>*cur=root;
			HuffmanTreeNode<FileInfo>* parent=cur->_parent;
			string &code=_infos[cur->_weight._ch]._code;
			string decode;
			while(parent!=NULL)
			{
				if (parent->_left==cur)
				{
					//��ʾcode
					code+='0';
				}
				if (parent->_right==cur)
				{
					code+='1';
				}
				cur=parent;
				parent=cur->_parent;
			}
			reverse(code.begin(),code.end());
			//cout<<code;
		}
	}
protected:
	FileInfo _infos[256];

};

void testf()
{
	FileCompress f1;
	long start=clock();
	//f1.Compress("f1.txt");
	//f1.Compress("f2.txt");
	//f1.Compress("f3.txt");
	//f1.Compress("f4.txt");
	//f1.Compress("1.png");
	f1.Compress("Input.BIG");
	long end=clock();
	cout<<"f.txt:compress"<<end-start<<endl;
	start=clock();
	//f1.UnCompress("f1.txt");
	//f1.UnCompress("f2.txt");
	//f1.UnCompress("f3.txt");
	//f1.UnCompress("f4.txt");
	//f1.UnCompress("1.png");
	f1.UnCompress("Input.BIG");
	end=clock();
	cout<<"f.txt:uncompress"<<end-start<<endl;
}