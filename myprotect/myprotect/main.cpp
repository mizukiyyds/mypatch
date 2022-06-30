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
	cout<<"¼Ó·¨£º"<< c<<endl;
	c=protect::sub(a,b, protect::type_default);
	cout<<"¼õ·¨£º"<< c<<endl;
	c=1234;
	c=protect::shl_(c,2);
	cout<<"×óÒÆ£º"<< c<<endl;
	c=protect::shr_(c,1);
	cout<<"ÓÒÒÆ£º"<< c<<endl;
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