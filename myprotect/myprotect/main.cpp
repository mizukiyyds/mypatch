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
	int c=protect::add(a,-b,protect::type_default);
	cout<<"�ӷ���"<< c<<endl;
	c=protect::sub(a,b, protect::type_default);
	cout<<"������"<< c<<endl;
	c=1234;
	c=protect::shl_(c,2);
	cout<<"���ƣ�"<< c<<endl;
	c=protect::shr_(c,1);
	cout<<"���ƣ�"<< c<<endl;
}
void func2()
{

}
int main()
{
	system("title mizuki");
	system("color 3e");
	protect::initialize();
	insert_junk_code_1(func1);
	system("pause");
}