#ifndef BIGDATA_H
#endif BIGDATA_H
#include <iostream>
#include <string>
#include <assert.h>
using namespace std;
typedef long long INT64;
#define RANDOM 0xcccccccccccccccc
#define MAXINT64 0x7fffffffffffffff
#define MININT64 0x8000000000000000
class BigData
{
private:
	INT64 m_llvalue;
	string strData;
	bool IsINT64Overflow()const;
	string Add(string left,string right);
	string Sub(string left,string right);
	string Mul(string left,string right);
	string Div(string left,string right);

	bool IsLeftStrBig(char *pLeft, size_t LSize, char *pRight, size_t RSize);
	char SubLoop(char *pLeft, size_t LSize, char *pRight, size_t RSize);
public:
	BigData();
	BigData(INT64 value);
	BigData(const char * data);
	void INT64ToStr();
	//��Ϊ<<��˫Ŀ��������Ϊ�˱�����ʽ�ϵ�һ�£�����Ԫ��������ɷǳ�Ա����������
	//�Ϳ��Խ�coutд��<<����ߣ��γ�cout<<data;������data<<cout;
	friend ostream & operator<<(ostream & _cout,const BigData & bigData);
	friend istream & operator>>(istream &_cin,BigData & bigData);
	//
	BigData operator+(BigData &bigData);
	BigData operator-(BigData &bigData);
	BigData operator*(BigData &bigData);
	BigData operator/(BigData &bigData);


};