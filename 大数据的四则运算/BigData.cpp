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
	//1���ж�data����Ч��
	assert(NULL!=data);
	//2��Ϊ�˲��ı�ԭʼ������һ����ʱ��������data
	char* pData=(char*) data;
	//3��data�п��ܵ���ʽ��+123 123 -123 +12a3 12a3 -12a3 +aaa123 aaa123 -aaa123 + -  00000023
	//��data�����жϣ�   123 123 -123  12   12   -12    0      0      0       0 0 
	//4����data�ĵ�һλ���趨Ϊ����λ
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
	//5��ȥ��ǰ��0
	while (*pData=='0')
	{
		pData++;
	}
	//resize������data��ʵ�ʳ��ȼ���\0��λ��
	strData.resize(strlen(data)+1);
	//6����strData��������
	m_llvalue=0;
	int iCount=1;
	while (*pData)
	{	
		//7����data�д��ڷ�����ʱ
		if (*pData<'0'||*pData>'9')
		{
			break;
		}
		strData[0]=cSymbol;
		m_llvalue=m_llvalue*10+*pData-'0';
		strData[iCount++]=*pData++;
	}
	//��dataΪ��ֵ��m_llvalue�����ֵ
	if (cSymbol=='-')
	{
		m_llvalue=0-m_llvalue;
	}
}
//�ж������Ƿ����
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
//������ת����ַ���
void BigData::INT64ToStr()
{
	//�ַ����ɷ���λ���������
	char cSymbol='+';
	if(m_llvalue<0)
	{
		cSymbol='-';
	}
	//Ϊ�˲��ı����ֱ�����һ����ʱ������������
	INT64 value=m_llvalue;
	//Ӧ��append׷���ַ����ı��ַ���
	strData.append(1,cSymbol);
	while(value)
	{
		strData.append(1,value%10+'0');
		value=value/10;
	}
	//���ڽ�������ַ������������ͨ������ָ��λ��//+321--0-1-2-3-size==4
	char *left=(char *)strData.c_str()+1; //strData[1]
	char *right=(char *)strData.c_str()+strData.size()-1;//strData[3]
	while(left<right)
	{
		char temp=*left;
		*left++=*right;
		*right--=temp;
	}
}
//���һ��BigData
ostream & operator<<(ostream & _cout,const BigData & bigData)
{
	//1��û���������bigdata��������ʽֱ�����
	if (!bigData.IsINT64Overflow())
	{
		_cout<<bigData.m_llvalue;
	}
	//2���������bigdata��strdata�ַ�����װ��ʽ����һ���ַ������
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
//����һ��BigData
istream & operator>>(istream &_cin,BigData & bigData)
{
	string str;
	_cin>>str;
	bigData.strData.resize(str.size());
	swap(str,bigData.strData);
	return _cin;
}
//�ӷ�
BigData BigData::operator+(BigData &bigData)
{   
	//1��������û���
	//1��ͬ��
	//2�����

	if(!IsINT64Overflow()&&!bigData.IsINT64Overflow())
	{
		//���---��һ���������
		if(strData[0]!=bigData.strData[0])
		{
			return BigData(m_llvalue+bigData.m_llvalue);
		}
		//ͬ�� ---�������������� �������10��������10 ���������2+7=9 10-2>=7
		//                       �������-10 ��С��-10 ���������-2+-7=-9 -10+-2=-8<=-7
		else
		{
			if(('+'==strData[0]&&MAXINT64-m_llvalue>=bigData.m_llvalue)||('-'==strData[0]&&MININT64+m_llvalue<=bigData.m_llvalue))
				return BigData(m_llvalue+bigData.m_llvalue);
		}
		
	}
	else
	{	//���
		if(strData[0]!=bigData.strData[0])
		{
			return BigData(Sub(strData,bigData.strData).c_str());
		}
		//ͬ��
		else
		{
			return BigData(Add(strData,bigData.strData).c_str());
		}
	
	}
	

}
string BigData::Add(string left,string right)
{
	//��λ��С�ķ����ұ�
	int LSize=left.size();
	int RSize=right.size();
	//����ұߵ�λ��������߻�������λ����ͬ���ұ߱���ߴ�
	//�ͽ����ҽ���
	if(RSize>LSize)
	{
		swap(left,right);
		swap(LSize,RSize);
	}
	//��Ret����Ҫ���ص��ַ�������Ϊ�ַ������ٿռ�
	string Ret;	
	Ret.resize(LSize+1);
	Ret[0]=left[0];
	//ѭ�����
	char cstep=0;
	for(int Idx=1;Idx<LSize;Idx++)
	{
		//ÿ����ӵ�λ��
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
//����
//1����������δ��� ͬ�� ���
BigData BigData::operator-(BigData &bigData)
{
	if (!IsINT64Overflow()&&!bigData.IsINT64Overflow())
	{
		//ͬ��---һ���������
		if (strData[0]==bigData.strData[0])
		{
			return BigData(m_llvalue-bigData.m_llvalue);
		}
		//���----û�����
		else if((strData[0]=='+'&&m_llvalue-MAXINT64<=bigData.m_llvalue)||(strData[0]=='-'&&m_llvalue-MAXINT64>=bigData.m_llvalue))
		{
			return BigData(m_llvalue-bigData.m_llvalue);
		}
		
		
	}
	//���----��� 12324455-(-2335435)
	if(strData[0]!=bigData.strData[0])
	{
		return BigData(Add(strData,bigData.strData).c_str());
	}
	//ͬ��
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
	//��������û�����
	if (!IsINT64Overflow()&&!bigData.IsINT64Overflow())
	{
		//ͬ��
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
		//��� 2*-7
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
	//����һ������������������������
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
	//ͬ��  
	if(left[0]==right[0])
	{
		Ret[0]='+';
	}
	//���
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

//����
BigData BigData::operator/(BigData &bigData)
{
	//�жϳ�����Ϊ��
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
	//����
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
	//ָ��
	char *pleft=(char *)left.c_str()+1;
	char *pright=(char *)right.c_str()+1;
	//99999/333
	int len=1;
	bool addlen=false;
	bool leslen=false;
	//���ݱ�����Ϊѭ��
	for (int i=0;i<LSize;)
	{
		//�����ǰλΪ0������
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
		//����������ĵ�ǰ״̬���˿��Ա�����������ͽ���ѭ�����,�ó�һλ
		if(IsLeftStrBig(pleft,len,pright,RSize-1))
		{
			
			Ret.append(1,SubLoop(pleft,len,pright,RSize-1));
			i++;
			addlen=true;

		}
		//����
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
