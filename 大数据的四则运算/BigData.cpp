#include "BigData.h"

BigData::BigData()
	:m_llvalue(RANDOM)
	,strData("")
{}
BigData::BigData(INT64 value)
	:m_llvalue(value)
	,strData("")
{
	INT64ToStr();
}
BigData::BigData(const char* data)
{
	//1、判断data的有效性
	assert(NULL!=data);
	//2、为了不改变原始数据用一个临时变量保存data
	char* pData=(char*) data;
	//3、data有可能的形式：+123 123 -123 +12a3 12a3 -12a3 +aaa123 aaa123 -aaa123 + -  00000023
	//对data进行判断：   123 123 -123  12   12   -12    0      0      0       0 0 
	//4、将data的第一位先设定为符号位
	char cSymbol=pData[0];
	if (cSymbol=='-'||cSymbol=='+')
	{
		pData++;
	}
	else if(*pData>='0'||*pData<='9')
	{
		cSymbol='+';
	}
	else
	{
		m_llvalue=0;
		strData="";
		return;
	}
	//5、去掉前置0
	while (*pData=='0')
	{
		pData++;
	}
	//resize开辟了data的实际长度加上\0的位置
	strData.resize(strlen(data)+1);
	//6、向strData保存数据
	m_llvalue=0;
	int iCount=1;
	while (*pData)
	{	
		//7、当data中存在非数字时
		if (*pData<'0'||*pData>'9')
		{
			break;
		}
		strData[0]=cSymbol;
		m_llvalue=m_llvalue*10+*pData-'0';
		strData[iCount++]=*pData++;
	}
	//当data为负值，m_llvalue保存的值
	if (cSymbol=='-')
	{
		m_llvalue=0-m_llvalue;
	}
}
//判断数据是否溢出
bool BigData::IsINT64Overflow()const
{
	string strtmp;
	if ('+'==strData[0])
	{
		strtmp="0x7FFFFFFFFFFFFFFF";
	}
	else
	{
		strtmp="0x8000000000000000";
	}
	if(strData.size()>strtmp.size())
	{
		return true;
	}
	else if(strData.size()==strtmp.size()&&strData>strtmp)
	{
		return true;
	}
	else
		return false;
}
//将数字转变成字符串
void BigData::INT64ToStr()
{
	//字符串由符号位和数字组成
	char cSymbol='+';
	if(m_llvalue<0)
	{
		cSymbol='-';
	}
	//为了不改变数字本身，用一个临时变量保存数字
	INT64 value=m_llvalue;
	//应用append追加字符来改变字符串
	strData.append(1,cSymbol);
	while(value)
	{
		strData.append(1,value%10+'0');
		value=value/10;
	}
	//现在将倒序的字符串正序过来，通过两个指针位置//+321--0-1-2-3-size==4
	char *left=(char *)strData.c_str()+1; //strData[1]
	char *right=(char *)strData.c_str()+strData.size()-1;//strData[3]
	while(left<right)
	{
		char temp=*left;
		*left++=*right;
		*right--=temp;
	}
}
//输出一个BigData
ostream & operator<<(ostream & _cout,const BigData & bigData)
{
	//1、没有溢出，将bigdata的数字形式直接输出
	if (!bigData.IsINT64Overflow())
	{
		_cout<<bigData.m_llvalue;
	}
	//2、溢出，将bigdata的strdata字符串封装形式按照一般字符串输出
	else if(bigData.strData[0]=='-')
	{
		_cout<<bigData.strData;
	}
	else
	{
		char * pData=(char *)bigData.strData.c_str();
		pData++;
		_cout<<pData;
	}
	return _cout;
	
}
//输入一个BigData
istream & operator>>(istream &_cin,BigData & bigData)
{
	string str;
	_cin>>str;
	bigData.strData.resize(str.size());
	swap(str,bigData.strData);
	return _cin;
}
//加法
BigData BigData::operator+(BigData &bigData)
{   
	//1、两个数没溢出
	//1）同号
	//2）异号

	if(!IsINT64Overflow()&&!bigData.IsINT64Overflow())
	{
		//异号---〉一定不会溢出
		if(strData[0]!=bigData.strData[0])
		{
			return BigData(m_llvalue+bigData.m_llvalue);
		}
		//同号 ---〉不会溢出的情况 如果上限10，即大于10 就算溢出：2+7=9 10-2>=7
		//                       如果下限-10 即小于-10 就算溢出：-2+-7=-9 -10+-2=-8<=-7
		else
		{
			if(('+'==strData[0]&&MAXINT64-m_llvalue>=bigData.m_llvalue)||('-'==strData[0]&&MININT64+m_llvalue<=bigData.m_llvalue))
				return BigData(m_llvalue+bigData.m_llvalue);
		}
		
	}
	else
	{	//异号
		if(strData[0]!=bigData.strData[0])
		{
			return BigData(Sub(strData,bigData.strData).c_str());
		}
		//同号
		else
		{
			return BigData(Add(strData,bigData.strData).c_str());
		}
	
	}
	

}
string BigData::Add(string left,string right)
{
	//将位数小的放在右边
	int LSize=left.size();
	int RSize=right.size();
	//如果右边的位数大于左边或者左右位数相同但右边比左边大
	//就将左右交换
	if(RSize>LSize)
	{
		swap(left,right);
		swap(LSize,RSize);
	}
	//用Ret保存要返回的字符串并且为字符串开辟空间
	string Ret;	
	Ret.resize(LSize+1);
	Ret[0]=left[0];
	//循环相加
	char cstep=0;
	for(int Idx=1;Idx<LSize;Idx++)
	{
		//每次相加的位数
		char CRet=left[LSize-Idx]-'0'+cstep;
		if(Idx<RSize)
		{
			CRet=CRet+right[RSize-Idx]-'0';
		}
		Ret[LSize-Idx+1]=CRet%10+'0';
		cstep=CRet/10;
	}
	Ret[1]=cstep+'0';
	return Ret;
}
//减法
//1、两个数均未溢出 同号 异号
BigData BigData::operator-(BigData &bigData)
{
	if (!IsINT64Overflow()&&!bigData.IsINT64Overflow())
	{
		//同号---一定不会溢出
		if (strData[0]==bigData.strData[0])
		{
			return BigData(m_llvalue-bigData.m_llvalue);
		}
		//异号----没有溢出
		else if((strData[0]=='+'&&m_llvalue-MAXINT64<=bigData.m_llvalue)||(strData[0]=='-'&&m_llvalue-MAXINT64>=bigData.m_llvalue))
		{
			return BigData(m_llvalue-bigData.m_llvalue);
		}
		
		
	}
	//溢出----异号 12324455-(-2335435)
	if(strData[0]!=bigData.strData[0])
	{
		return BigData(Add(strData,bigData.strData).c_str());
	}
	//同号
	else
	{
		return BigData(Sub(strData,bigData.strData).c_str());
	}

}

string BigData::Sub(string left,string right)
{
	int LSize=left.size();
	int RSize=right.size();
	if((LSize<RSize)||(LSize==RSize&&left<right))
	{
		swap(left,right);
		swap(LSize,RSize);
		if(left[0]=='-')
		{
			left[0]='+';
		}
		else
		{
			left[0]='-';
		}
	}
	string Ret;
	Ret.resize(LSize);
	Ret[0]=left[0];
	for (int Idx=1;Idx<LSize;Idx++)
	{
		char CRet=left[LSize-Idx]-'0';
		if (Idx<RSize)
		{
			CRet=CRet-(right[RSize-Idx]-'0');
		}
		if (CRet<0)
		{
			Ret[LSize-Idx+1]-=1;
			CRet+=10;
		}
		Ret[LSize-Idx]=CRet+'0';
	}
	return Ret;
}

BigData BigData::operator*(BigData &bigData)
{
	//两个数都没有溢出
	if (!IsINT64Overflow()&&!bigData.IsINT64Overflow())
	{
		//同号
		if(strData[0]==bigData.strData[0])
		{
			//2*7<20 20/2=10>7  -2*-7<20 20/-2=-10<-7
			if (('+'==strData[0]&&MAXINT64/m_llvalue>=bigData.m_llvalue)||
				('-'==strData[0]&&MAXINT64/m_llvalue<=bigData.m_llvalue))
			{
				return BigData(m_llvalue*bigData.m_llvalue);
			}
			else
				return BigData(Mul(strData,bigData.strData).c_str());
		}
		//异号 2*-7
		else
		{	//2*-7=-14 -20/2=-10<-7 -2*7 -20/-2=10>
			if(('+'==strData[0]&&MININT64/m_llvalue<=bigData.m_llvalue)||
				('-'==strData[0]&&MININT64/m_llvalue>=bigData.m_llvalue))
			{
				return BigData(m_llvalue*bigData.m_llvalue);
			}
			else
				return BigData(Mul(strData,bigData.strData).c_str());
		}
	}
	//其中一个数溢出，或者两个数都溢出
	else
	{
		return BigData(Mul(strData,bigData.strData).c_str());
	}
}
string BigData::Mul(string left,string right)
{
		//  2222
		//*33333
		//-------
		//  6666
		// 6666
		//6666
	   //6666
	  //6666
	//----------
	//  74065926---8=4+5-1
	int LSize=left.size();
	int RSize=right.size();
	string Ret;
	int len=LSize+RSize-1;
	Ret.resize(len);
	//同号  
	if(left[0]==right[0])
	{
		Ret[0]='+';
	}
	//异号
	else
	{
		Ret[0]='-';
	}
	if(LSize>RSize)
	{
		swap(left,right);
		swap(LSize,RSize);
	}
	int i=0;
	char CRet=0;
	char cstep=0;
	for(int Idx=1;Idx<RSize;Idx++)
	{
		for(int Idy=1;Idy<LSize;Idy++)
		{
			CRet=(left[LSize-Idy]-'0')*(right[RSize-Idx]-'0');
			cstep=(Ret[len-Idy-i]+CRet)/10;
			Ret[len-Idy-i]=(Ret[len-Idy-i]+CRet)%10;
			Ret[len-Idy-i-1]=Ret[len-Idy-i-1]+cstep;
		}
		Idx++;
		i++;
	}
	for(int i=1;i<len;i++)
	{
		Ret[i]+='0';
	}
	return Ret;
}

//除法
BigData BigData::operator/(BigData &bigData)
{
	//判断除数不为零
	if(bigData.m_llvalue==0||m_llvalue==0)
	{
		return BigData(INT64(0));
	}
	if(!IsINT64Overflow()&&!bigData.IsINT64Overflow())
	{
		return BigData(m_llvalue/bigData.m_llvalue);
	}
	return BigData(Div(strData,bigData.strData).c_str());
}

string BigData::Div(string left,string right)
{
	string Ret;
	//长度
	size_t LSize=left.size();
	size_t RSize=right.size();
	if ((LSize<RSize)||(LSize==RSize&&strncmp(left.c_str()+1,right.c_str()+1,LSize-1)<1))
	{
		return "0";
	}
	char csymbol='+';
	if(left[0]!=right[0])
	{
		csymbol='-';
	}
	else
	{
		if(right=="-1"||right=="+1")
		{
			left[0]=csymbol;
			return left;
		}
	}
	//指针
	char *pleft=(char *)left.c_str()+1;
	char *pright=(char *)right.c_str()+1;
	//99999/333
	int len=1;
	bool addlen=false;
	bool leslen=false;
	//根据被除数为循环
	for (int i=0;i<LSize;)
	{
		//如果当前位为0，保留
		if (*pleft=='0')
		{
			pleft++;
			leslen=true;
			continue;
		}
		if (leslen)
		{
			len=RSize-1;
		}
		//如果被除数的当前状态到了可以被除数相除，就进行循环相减,得出一位
		if(IsLeftStrBig(pleft,len,pright,RSize-1))
		{
			
			Ret.append(1,SubLoop(pleft,len,pright,RSize-1));
			i++;
			addlen=true;

		}
		//否则
		else
		{
			if (addlen==false)
			{	
				Ret.append(1,'0');
			}
			i++;
			len++;
			continue;
		}
	}
	return Ret;
}

bool BigData::IsLeftStrBig(char *pLeft, size_t LSize, char *pRight, size_t RSize)
{
	while(*pLeft=='0')
	{
		pLeft++;
		LSize--;
	}
	if(LSize<RSize)
	{
		return false;
	}
	else
	{
		if(LSize==RSize&&strncmp(pLeft,pRight,LSize)<1)
		{
			return false;
		}
		return true;
	}
}

char BigData::SubLoop(char *pLeft, size_t LSize, char *pRight, size_t RSize)
{
	char cstep=0;
	while(true)
	{
		for(int i=0;i<RSize;i++)
		{
			pLeft[LSize-i-1]=pLeft[LSize-i-1]-'0'-(pRight[RSize-i-1]-'0')+'0';
			if(pLeft[LSize-i-1]<'0')
			{
				pLeft[LSize-i-1]+=10;
				pLeft[LSize-i-2]-=1;
			}
		}
		cstep++;
		if(IsLeftStrBig(pLeft,LSize,pRight,RSize)==false)
		{
			break;
		}
		
	}
	return (cstep+'0');
}
