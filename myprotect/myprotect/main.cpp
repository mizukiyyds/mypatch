#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>

#include "myprotect.h"
using namespace std;
char info[100];
void msg1()
{
	cout<<"���ǵ�һ������\n";
	MessageBoxA(0,"���ǵ�һ������","����",MB_TOPMOST|MB_OK);
	
}
void msg2()
{
	cout<<"���ǵڶ�������\n";
	MessageBoxA(0,"���ǵ�һ������","����",MB_TOPMOST|MB_OK);
}
int main()
{
	strcpy(info,"initial_begin");
	//initialize();
	strcpy(info,"initial_end");
	my_junk_code_begin1(msg1,msg2);
	system("pause");

}