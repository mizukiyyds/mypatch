#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>

#include "myprotect.h"
using namespace std;
char info[100];
void func1()
{
	int a=111,b=234;
	__asm{
		nop
		nop
		nop
		nop
	}
	int c=protect::add(a,b,protect::type_rand);
	cout<<"ģ��ӷ���"<< c<<endl;
	c=protect::xor_(a,b, protect::type_rand);
	cout<<"ģ�����"<< c<<endl;
	
}
void func2()
{

}
int main()
{
	//strcpy(info,"initial_begin");
	initialize();
	//strcpy(info,"initial_end");
	
	my_junk_code_begin1(func1,func2);

	system("pause");

}