#include "BigData.h"

void test1()
{
	//BigData b1("+123456");
	cout<<strlen("+123456")<<endl;
	cout<<sizeof(int)<<endl;
	cout<<sizeof(char)<<endl;
	cout<<sizeof(short)<<endl;
	cout<<sizeof(double)<<endl;
	cout<<sizeof(float)<<endl;
	cout<<sizeof(long)<<endl;
	cout<<sizeof(long long)<<endl;
	
}

void test2()
{
	BigData b1("");
	BigData b2("-123");
	BigData b3("-");
	BigData b4("+");
	BigData b5("+aa123");
	BigData b6("-aa123");
	BigData b7("aa123");
	BigData b8("123");
	BigData b9("+12aa3");
	BigData b10("-12aa3");
	BigData b11("12aa3");
	BigData b13("-000123");
	BigData b14("+000123");
	BigData b15("9999999999999999999999999999999999999999999999999999999");
	BigData b16(123);
	BigData b17(1234);
	cout<<b15;
	cin>>b1;

}
void test3()
{
	BigData b1("12345678901234567890123456789");
	BigData b2("12345");
	cout<<b1+b2<<endl;
	
}
void test4()
{
	BigData b1("9999999999999999999999999999999");
	BigData b2("9999999999999999999999999999991");
	cout<<b1-b2<<endl;
	BigData b3("33333333333333");
	BigData b4("9999999999999999999999999999999");
	cout<<b3-b4<<endl;
	BigData b5("9999999999999999999999999999991");
	BigData b6("9999999999999999999999999999999");
	cout<<b5-b6<<endl;
}
void test5()
{
	BigData b1("22222222222222222222222");
	BigData b2("33333333333333333333");
	cout<<b1*b2<<endl;
}
void test6()
{
	BigData b1("444444444444444444444444444444");
	BigData b2("232423");
	cout<<b1/b2<<endl;
}
int main()
{
	//test1();
	//test2();
	//test3();
	//test4();
	//test5();
	test6();
	return 0;
}