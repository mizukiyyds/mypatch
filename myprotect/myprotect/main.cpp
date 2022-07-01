#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>

#include "myprotect.h"
using namespace std;

char info[100];

void func1()
{
	__asm {
		nop
		nop
		nop
		nop
	}
	int a = 111, b = 234;
	int c = protect::add(a, b, protect::type_default);
	cout << "加法：" << c << endl;
	c = protect::sub(a, b, protect::type_default);
	cout << "减法：" << c << endl;
	c = protect::mul(a, b);
	cout << "乘法：" << c << endl;
	c = protect::shl_(c, 2);
	cout << "左移：" << c << endl;
	c = protect::shr_(c, 1);
	cout << "右移：" << c << endl;
	protect::print_last_error();
}
void func2()
{
	cout<<"func2"<<endl;
	
}
int main()
{
	protect::initialize(0x20220701);

	system("title mizuki");
	system("color 3e");
	insert_junk_code_1(func1,func2);
	system("pause");
}